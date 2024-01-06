#include <sysexits.h>
#include <array>
#include <cmath>
#include <fstream>
#include <iostream>

#include <mpi.h>

#include "benchmark.h"
#include "graham_scan.h"
#include "io.h"
#include "orientation.h"
#include "point.h"
#include "point_cloud.h"

namespace {

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
int g_comm_rank{};
int g_comm_size{};

benchmark::RepeatingTimer g_comm_timer;
MPI_Datatype g_mpi_point;
MPI_Op g_mpi_min_point;
MPI_Op g_mpi_max_angle;
Point g_last_point_in_hull;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void PrintUsage(const char* prog_name) {
  PrintRed(std::cerr, "Usage: ", prog_name,
           " <input_file> <output_file>\n"
           "Parameters:\n"
           "  input_file: path of the file containing point cloud data\n"
           "  output_file: path of the file where the calculated convex hull "
           "will be saved\n"
           "  benchmark_file: path of the file where benchmark metrics will be "
           "written\n");
}

template <typename... Args>
void LogMaster(Args&&... args) {
  PrintGreen(std::cout, "[master] ", std::forward<Args>(args)...);
}

template <typename... Args>
void LogSlave(Args&&... args) {
  PrintYellow(std::cout, "[slave ", g_comm_rank, "] ",
              std::forward<Args>(args)...);
}

template <typename... Args>
void LogError(Args&&... args) {
  PrintRed(std::cerr, "[error] ", std::forward<Args>(args)...);
}

void MpiMinPointOp(void* in_vec,
                   void* in_out_vec,
                   int* /* len */,
                   MPI_Datatype* /* type */) {
  // Vectors of points.
  const auto* in = static_cast<Point*>(in_vec);
  auto* in_out = static_cast<Point*>(in_out_vec);

  // First elements.
  if (*in < *in_out) {
    *in_out = *in;
  }
}

void MpiMaxAngleOp(void* in_vec,
                   void* in_out_vec,
                   int* /* len */,
                   MPI_Datatype* /* type */) {
  // Vectors of points.
  const auto* in = static_cast<Point*>(in_vec);
  auto* in_out = static_cast<Point*>(in_out_vec);

  // First elements.
  if (AngleLess(g_last_point_in_hull, *in_out, *in)) {
    *in_out = *in;
  }
}

void InitMpiRuntime() {
  constexpr std::array<int, 1> block_counts{2};
  constexpr std::array<MPI_Aint, 1> offsets{0};
  const std::array<MPI_Datatype, 1> types{MPI_LONG_LONG};

  MPI_Type_create_struct(1, block_counts.data(), offsets.data(), types.data(),
                         &g_mpi_point);
  MPI_Type_commit(&g_mpi_point);

  MPI_Op_create(&MpiMinPointOp, 1, &g_mpi_min_point);
  MPI_Op_create(&MpiMaxAngleOp, 1, &g_mpi_max_angle);
}

void BroadcastCloudSize(size_t& cloud_size) {
  benchmark::TimerGuard guard{g_comm_timer};
  MPI_Bcast(&cloud_size, 1, MPI_LONG, 0, MPI_COMM_WORLD);
}

void SetupScatterParams(const size_t cloud_size,
                        std::vector<int>& sizes,
                        std::vector<int>& offsets) {
  if (cloud_size > static_cast<size_t>(std::numeric_limits<int>::max())) {
    LogError("Too big cloud\n");
    exit(EXIT_FAILURE);
  }

  const int chunk_size = ceil(static_cast<double>(cloud_size) / g_comm_size);
  const int last_chunk_size =
      static_cast<int>(cloud_size) - chunk_size * (g_comm_size - 1);

  for (int k = 0; k < g_comm_size; ++k) {
    sizes[k] = chunk_size;
    offsets[k] = k * chunk_size;
  }

  sizes.back() = last_chunk_size;
}

PointCloud ScatterCloud(const PointCloud& cloud, const size_t cloud_size) {
  std::vector<int> chunk_sizes(g_comm_size);
  std::vector<int> offsets(g_comm_size);

  SetupScatterParams(cloud_size, chunk_sizes, offsets);

  PointCloud sub_cloud(chunk_sizes[g_comm_rank]);

  benchmark::TimerGuard guard{g_comm_timer};
  MPI_Scatterv(cloud.data(), chunk_sizes.data(), offsets.data(), g_mpi_point,
               sub_cloud.data(), static_cast<int>(sub_cloud.size()),
               g_mpi_point, 0, MPI_COMM_WORLD);

  return sub_cloud;
}

void FintBottom(const Point local_point, Point& bottom) {
  benchmark::TimerGuard guard{g_comm_timer};
  MPI_Allreduce(&local_point, &bottom, 1, g_mpi_point, g_mpi_min_point,
                MPI_COMM_WORLD);
}

Point FindRightTangent(const PointCloud& cloud, const Point reference) {
  const auto cloud_size = cloud.size();

  size_t start = 0;
  size_t end = cloud_size;

  auto start_prev = Orientation(reference, cloud[start], cloud[end - 1]);
  auto start_next =
      Orientation(reference, cloud[start], cloud[(start + 1) % end]);

  while (start < end) {
    const auto pivot = (start + end) / 2;
    const auto pivot_prev =
        Orientation(reference, cloud[pivot], cloud[(pivot - 1) % cloud_size]);
    const auto pivot_next =
        Orientation(reference, cloud[pivot], cloud[(pivot + 1) % cloud_size]);

    const auto pivot_side = Orientation(reference, cloud[start], cloud[pivot]);

    if (pivot_prev != Direction::kCounterclockwise &&
        pivot_next != Direction::kCounterclockwise) {
      start = pivot;
      break;
    }

    if ((pivot_side == Direction::kClockwise &&
         start_next == Direction::kCounterclockwise) ||
        (start_prev == start_next) ||
        (pivot_side == Direction::kCounterclockwise &&
         pivot_prev == Direction::kCounterclockwise)) {
      end = pivot;
    } else {
      start = pivot + 1;
      start_prev = static_cast<Direction>(-static_cast<int>(pivot_next));
      start_next =
          Orientation(reference, cloud[start], cloud[(start + 1) % cloud_size]);
    }
  }

  if (cloud[start] == reference) {
    start = (start + 1) % cloud_size;
  }

  return cloud[start];
}

void FindNextPointInHull(const Point local_point,
                         const Point last_found,
                         Point& next_hull_point) {
  g_last_point_in_hull = last_found;

  benchmark::TimerGuard guard{g_comm_timer};
  MPI_Allreduce(&local_point, &next_hull_point, 1, g_mpi_point, g_mpi_max_angle,
                MPI_COMM_WORLD);
}

int MpiConvexHullMaster(const char* input_filename,
                        const char* output_filename,
                        const char* benchmark_filename) {
  std::ifstream cloud_file{input_filename};
  if (!cloud_file) {
    LogError("Cannot open file ", input_filename, '\n');
    return EX_IOERR;
  }

  PointCloud cloud;
  cloud_file >> cloud;
  cloud_file.close();
  LogMaster("Loaded point cloud with ", cloud.size(), " points\n");

  auto total_timer = benchmark::OnceTimer::Started();
  InitMpiRuntime();
  auto cloud_size = cloud.size();
  BroadcastCloudSize(cloud_size);

  auto scatter_timer = benchmark::OnceTimer::Started();
  auto sub_cloud = ScatterCloud(cloud, cloud_size);
  scatter_timer.Stop();
  LogMaster("Received point cloud chunk of size ", sub_cloud.size(), '\n');

  auto sub_hull_timer = benchmark::OnceTimer::Started();
  LogMaster("Calculating convex hull for sub cloud\n");
  auto sub_hull = GrahamScan(sub_cloud);
  sub_hull_timer.Stop();
  std::ofstream{std::string{"data/sub_hull_"} + std::to_string(g_comm_rank) +
                ".dat"}
      << sub_hull;

  PointCloud final_hull(cloud.size());
  FintBottom(sub_hull[0], final_hull[0]);
  LogMaster("Found bottom point: ", final_hull[0], '\n');

  auto merge_timer = benchmark::OnceTimer::Started();
  auto tangent_timer = benchmark::OnceTimer{};
  int k = 0;
  do {
    tangent_timer.Start();
    const auto q = FindRightTangent(sub_hull, final_hull[k]);
    tangent_timer.Stop();

    FindNextPointInHull(q, final_hull[k], final_hull[k + 1]);
    ++k;

    LogMaster("Found next point in hull: ", final_hull[k], '\n');
  } while (k <= final_hull.size() && final_hull[k] != final_hull[0]);
  merge_timer.Stop();

  /* The last point is equal to the first one, so we delete it from the final
   * hull */
  final_hull.resize(k);

  total_timer.Stop();

  std::ofstream benchmark_file{benchmark_filename};
  if (!benchmark_file) {
    LogError("Cannot open file ", benchmark_filename, '\n');
    return EX_IOERR;
  }
  benchmark_file << g_comm_size << ',' << cloud.size() << ',';
  benchmark_file.precision(3);
  benchmark_file.setf(std::ios::fixed);
  benchmark_file << total_timer.Get() << ',' << sub_hull_timer.Get() << ','
                 << scatter_timer.Get() << ',' << merge_timer.Get() << ','
                 << tangent_timer.Get() << ',' << g_comm_timer.Get() << ','
                 << g_comm_timer.Get() / total_timer.Get() * 100 << "\n";
  benchmark_file.close();

  std::ofstream hull_file{output_filename};
  if (!hull_file) {
    LogError("Cannot open file ", output_filename, '\n');
    return EX_IOERR;
  }
  hull_file << final_hull;
  hull_file.close();

  return EX_OK;
}

int MpiConvexHullSlave() {
  InitMpiRuntime();
  size_t cloud_size = 0;
  BroadcastCloudSize(cloud_size);

  LogSlave("Advertised point cloud size: ", cloud_size, '\n');

  PointCloud cloud(cloud_size);
  cloud.reserve(cloud_size);

  auto sub_cloud = ScatterCloud(cloud, cloud_size);
  LogSlave("Received point cloud chunk of size ", sub_cloud.size(), '\n');

  LogSlave("Calculating convex hull for sub cloud\n");
  auto sub_hull = GrahamScan(sub_cloud);
  std::ofstream{std::string{"data/sub_hull_"} + std::to_string(g_comm_rank) +
                ".dat"}
      << sub_hull;

  Point first_in_hull;
  FintBottom(sub_hull[0], first_in_hull);

  Point p = first_in_hull;
  do {
    const auto q = FindRightTangent(sub_hull, p);
    FindNextPointInHull(q, p, p);
  } while (p != first_in_hull);

  return EX_OK;
}

}  // namespace

// NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic)
int main(int argc, char** argv) {
  std::ios_base::sync_with_stdio(false);

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &g_comm_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &g_comm_size);

  int result = EXIT_SUCCESS;

  if (argc != 4) {
    if (g_comm_rank == 0) {
      PrintUsage(argv[0]);
    }
    result = EX_USAGE;
  } else if (g_comm_rank == 0) {
    result = MpiConvexHullMaster(argv[1], argv[2], argv[3]);
  } else {
    result = MpiConvexHullSlave();
  }

  MPI_Finalize();

  return result;
}
// NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

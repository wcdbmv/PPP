#include <sysexits.h>
#include <cmath>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>

#include "graham_scan.h"
#include "point_cloud.h"

namespace {

template <typename T>
T RandInt(T a, T b) {
  thread_local std::mt19937 generator(std::random_device{}());
  std::uniform_int_distribution<T> distribution(a, b);
  return distribution(generator);
}

Point RandomPointGenerator(const size_t size) {
  const auto radius = static_cast<int64_t>(size) * 100;
  const auto x = RandInt(-radius, radius);
  const auto max_y = static_cast<int64_t>(std::sqrt(radius * radius - x * x));
  const auto y = RandInt(-max_y, max_y);
  return Point{x, y};
}

size_t ParseCloudSize(const char* num) {
  size_t result = 0;
  std::istringstream{num} >> result;
  return result;
}

void PrintUsage(const char* prog_name) {
  std::cerr << "Usage: " << prog_name << " <size> [cloud] [hull]\n"
            << "Parameters:\n"
               "  size: number of test points to generate\n"
               "  cloud: path of the file where the generated point cloud will "
               "be saved\n"
               "  hull: path of the file where the convex hull will be saved\n";
}

}  // namespace

int main(int argc, const char** argv) {
  std::ios_base::sync_with_stdio(false);

  if (argc < 2) {
    PrintUsage(argv[0]);
    return EX_USAGE;
  }

  const auto cloud_size = ParseCloudSize(argv[1]);
  const auto* dest_filename = argc > 2 ? argv[2] : "data/cloud.dat";
  const auto* hull_filename = argc > 3 ? argv[3] : "data/hull.dat";

  std::cout << "Generating point cloud of size: " << cloud_size << '\n';

  PointCloud pc(cloud_size);
  for (size_t i = 0; i < cloud_size; ++i) {
    pc[i] = RandomPointGenerator(cloud_size);
  }

  std::cout << "Storing point cloud into " << dest_filename << '\n';
  std::ofstream out{dest_filename};
  if (!out) {
    std::cerr << "Error opening file  " << dest_filename << ". Aborting.\n";
    return EX_IOERR;
  }
  out << pc;
  out.close();

  std::cout << "Calculating convex hull for generated cloud.\n";
  const auto hull = GrahamScan(pc);

  std::cout << "Storing convex hull into " << hull_filename << '\n';
  std::ofstream hull_out{hull_filename};
  if (!hull_out) {
    std::cerr << "Error opening file  " << hull_filename << ". Aborting.\n";
    return EX_IOERR;
  }
  hull_out << hull;
  hull_out.close();

  return EX_OK;
}

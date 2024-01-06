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
  thread_local std::uniform_int_distribution<T> distribution(a, b);
  return distribution(generator);
}

int64_t RandCoordinate(const int64_t limit) {
  const int64_t safe_limit = (limit == 0 ? 1L : limit) * 500'000;
  return RandInt(-safe_limit, safe_limit);
}

Point RandPoint(const int64_t limit) {
  return Point{RandCoordinate(limit), RandCoordinate(limit)};
}

/*
 * If we allow each point to be picked in the same range, as the number of
 * points increase, the final shape will strongly resemble a square. Instead,
 * if we partition the number of points so that each subset of points is picked
 * from a range that is larger/smaller than the previous ones, we will obtain a
 * more random shape. In particular, we'll have fewer chances to have
 * collinear points on the convex hull, and its shape will be more fuzzy.
 * To improve the fuzziness around the edges, we'll use a logarithmic function;
 * the choice of the exact expression is rather customary and has been done on
 * an inspection basis (looking at the final picture of the point cloud).
 */
Point RandomPointGenerator(const size_t index, const size_t size) {
  const auto i = static_cast<double>(index);
  const auto n = static_cast<double>(size);
  const auto limit = static_cast<int64_t>(1.0 - log((1.0 + i / 10) / (n / 10)));
  return RandPoint(limit);
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
    pc[i] = RandomPointGenerator(i, cloud_size);
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

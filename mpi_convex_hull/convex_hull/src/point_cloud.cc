#include "point_cloud.h"

#include <istream>
#include <limits>
#include <ostream>

std::istream& operator>>(std::istream& is, PointCloud& cloud) {
  is.ignore(std::numeric_limits<std::streamsize>::max(), '=');

  size_t size = 0;
  is >> size;

  cloud.resize(size);
  for (auto& point : cloud) {
    is >> point.x >> point.y;
  }

  return is;
}

std::ostream& operator<<(std::ostream& os, const PointCloud& cloud) {
  os << "# size=" << cloud.size() << '\n';

  for (const auto point : cloud) {
    os << point.x << '\t' << point.y << '\n';
  }

  // Here we replicate the first point so that we can plot a closed polyline in
  // Gnuplot. This last point is actually ignored when reading it back.
  os << cloud[0].x << '\t' << cloud[0].y << '\n';

  return os;
}

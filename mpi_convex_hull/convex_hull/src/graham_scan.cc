#include "graham_scan.h"

#include "orientation.h"

PointCloud GrahamScan(PointCloud& points) {
  if (points.size() <= 1) {
    return points;
  }

  const auto min = std::min_element(
      points.begin(), points.end(), [](const Point a, const Point b) {
        return a.y < b.y || (a.y == b.y && a.x < b.x);
      });

  std::iter_swap(points.begin(), min);

  const auto p0 = points.front();
  std::sort(
      points.begin() + 1, points.end(), [p0](const Point a, const Point b) {
        const auto direction = Orientation(p0, a, b);
        if (direction == Direction::kCollinear) {
          return Point::SquareDistance(p0, a) < Point::SquareDistance(p0, b);
        }
        return direction == Direction::kCounterclockwise;
      });

  size_t new_size = 1;
  for (size_t i = 1, size = points.size(); i < size; ++i) {
    while (i < size - 1 &&
           Orientation(p0, points[i], points[i + 1]) == Direction::kCollinear) {
      ++i;
    }

    points[new_size] = points[i];
    ++new_size;
  }

  points.resize(new_size);

  if (new_size < 3) {
    return points;
  }

  PointCloud hull{points[0], points[1], points[2]};

  for (size_t i = 3; i < new_size; ++i) {
    while (hull.size() > 1 &&
           Orientation(hull[hull.size() - 2], hull.back(), points[i]) !=
               Direction::kCounterclockwise) {
      hull.pop_back();
    }
    hull.push_back(points[i]);
  }

  return hull;
}

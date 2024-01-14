#include "tangent.h"

#include <algorithm>

#include "orientation.h"

Point FindRightTangent(const PointCloud& hull, const Point reference) {
  const auto hull_size = hull.size();
  if (hull_size == 1) {
    return hull.front();
  }

  // [left, right)
  size_t left = 0;
  size_t right = hull_size;

  auto ref_left_prev = Orientation(reference, hull[left], hull[right - 1]);
  auto ref_left_next =
      Orientation(reference, hull[left], hull[(left + 1) % right]);

  while (left < right) {
    const auto middle = left + (right - left) / 2;
    const auto before_middle = (middle + hull_size - 1) % hull_size;
    const auto after_middle = (middle + 1) % hull_size;
    if (reference == hull[middle]) {
      return hull[after_middle];
    }

    const auto ref_middle_prev =
        Orientation(reference, hull[middle], hull[before_middle]);
    const auto ref_middle_next =
        Orientation(reference, hull[middle], hull[after_middle]);

    const auto middle_side = Orientation(reference, hull[left], hull[middle]);

    if (ref_middle_prev != Direction::kClockwise &&
        ref_middle_next != Direction::kClockwise) {
      left = middle;
      break;
    }

    // if ((middle_side == Direction::kCounterclockwise &&
    //      ref_left_next == Direction::kClockwise) ||
    //     (ref_left_prev == ref_left_next) ||
    //     (middle_side == Direction::kClockwise &&
    //      ref_middle_prev == Direction::kClockwise)) {
    // }

    if ((middle_side == Direction::kCounterclockwise) &&
            (ref_left_next == Direction::kClockwise ||
             ref_left_prev == ref_left_next) ||
        (middle_side == Direction::kClockwise &&
         ref_middle_prev == Direction::kClockwise)) {
      right = middle;
    } else {
      left = middle + 1;
      if (left == hull_size) {
        left = 0;
        break;
      }
      if (reference == hull[left]) {
        return hull[(left + 1) % hull_size];
      }

      ref_left_prev = static_cast<Direction>(-static_cast<int>(ref_middle_next));
      ref_left_next =
          Orientation(reference, hull[left], hull[(left + 1) % hull_size]);
    }
  }

  if (hull[left] == reference) {
    left = (left + 1) % hull_size;
  } else if (hull_size > 1) {
    const auto before_left = (left + hull_size - 1) % hull_size;
    const auto after_left = (left + 1) % hull_size;
    const auto is_ref_left_prev_collinear =
        Point::DotProduct(Point(reference, hull[left]),
                          Point(reference, hull[before_left])) > 0 &&
        Orientation(reference, hull[left], hull[before_left]) ==
            Direction::kCollinear;
    const auto is_ref_left_next_collinear =
        Point::DotProduct(Point(reference, hull[left]),
                          Point(reference, hull[after_left])) > 0 &&
        Orientation(reference, hull[left], hull[after_left]) ==
            Direction::kCollinear;
    if (is_ref_left_prev_collinear || is_ref_left_next_collinear) {
      std::vector<size_t> collinear_idx;
      collinear_idx.reserve(3);
      collinear_idx.push_back(left);
      if (is_ref_left_prev_collinear) {
        collinear_idx.push_back(before_left);
      }
      if (is_ref_left_next_collinear) {
        collinear_idx.push_back(after_left);
      }
      const auto max =
          std::max_element(collinear_idx.begin(), collinear_idx.end(),
                           [reference, &hull](const size_t i, const size_t j) {
                             return Point::SquareDistance(reference, hull[i]) <
                                    Point::SquareDistance(reference, hull[j]);
                           });
      left = *max;
    }
  }

  return hull[left];
}

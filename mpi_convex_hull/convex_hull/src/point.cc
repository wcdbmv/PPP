#include "point.h"

#include <ostream>

std::ostream& operator<<(std::ostream& os, const Point point) {
  return os << '(' << point.x << ", " << point.y << ')';
}

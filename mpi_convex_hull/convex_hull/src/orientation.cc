#include "orientation.h"

Direction Orientation(const Point p, const Point q, const Point r) {
  const auto pq = Point(p, q);
  const auto qr = Point(q, r);
  return Orientation(pq, qr);
}

Direction Orientation(const Point pq, const Point qr) {
  const auto area = Point::SkewProduct(qr, pq);

  if (area == 0) {
    return Direction::kCollinear;
  }

  if (area < 0) {
    return Direction::kCounterclockwise;
  }

  return Direction::kClockwise;
}

bool AngleLess(const Point p, const Point q, const Point r) {
  const auto direction = Orientation(p, q, r);
  if (direction == Direction::kCounterclockwise) {
    return true;
  }
  if (direction == Direction::kCollinear) {
    return Point::SquareDistance(p, q) < Point::SquareDistance(p, r);
  }
  return false;
}

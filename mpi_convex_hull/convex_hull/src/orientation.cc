#include "orientation.h"

Direction Orientation(Point p, Point q, Point r) {
  const auto pq = Point(p, q);
  const auto qr = Point(q, r);
  return Orientation(pq, qr);
}

Direction Orientation(Point pq, Point qr) {
  const auto area = Point::SkewProduct(qr, pq);

  if (area == 0) {
    return Direction::kCollinear;
  }

  if (area < 0) {
    return Direction::kCounterclockwise;
  }

  return Direction::kClockwise;
}
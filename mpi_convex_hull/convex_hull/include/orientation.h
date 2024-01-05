#pragma once

#include "point.h"

enum class Direction {
  kCounterclockwise = -1,
  kCollinear = 0,
  kClockwise = 1,
};

Direction Orientation(Point p, Point q, Point r);
Direction Orientation(Point pq, Point qr);
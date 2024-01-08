#pragma once

#include "point.h"

enum class Direction : int8_t {
  kClockwise = -1,
  kCollinear = 0,
  kCounterclockwise = 1,
};

Direction Orientation(Point p, Point q, Point r);
Direction Orientation(Point pq, Point qr);

bool AngleLess(Point p, Point q, Point r);

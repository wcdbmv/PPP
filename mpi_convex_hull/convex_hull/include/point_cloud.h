#pragma once

#include <iosfwd>
#include <vector>

#include "point.h"

using PointCloud = std::vector<Point>;

std::istream& operator>>(std::istream&, PointCloud&);
std::ostream& operator<<(std::ostream&, const PointCloud&);

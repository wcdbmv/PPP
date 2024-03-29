#pragma once

#include <cstdint>
#include <iosfwd>

struct Point {
  int64_t x;
  int64_t y;

  explicit constexpr Point(int64_t x = 0, int64_t y = 0) noexcept;
  constexpr Point(Point a, Point b) noexcept;

  [[nodiscard]] constexpr int64_t SquareLength() const noexcept;

  [[nodiscard]] static constexpr int64_t SquareDistance(Point a,
                                                        Point b) noexcept;
  [[nodiscard]] static constexpr int64_t DotProduct(Point a, Point b) noexcept;
  [[nodiscard]] static constexpr int64_t SkewProduct(Point a, Point b) noexcept;
};

[[nodiscard]] constexpr bool operator==(Point a, Point b) noexcept;
[[nodiscard]] constexpr bool operator!=(Point a, Point b) noexcept;
[[nodiscard]] constexpr bool operator<(Point a, Point b) noexcept;

std::ostream& operator<<(std::ostream& os, Point point);

constexpr Point::Point(const int64_t x, const int64_t y) noexcept
    : x{x}, y{y} {}

constexpr Point::Point(const Point a, const Point b) noexcept
    : x{b.x - a.x}, y{b.y - a.y} {}

constexpr int64_t Point::SquareLength() const noexcept {
  return x * x + y * y;
}

constexpr int64_t Point::SquareDistance(const Point a, const Point b) noexcept {
  return Point(a, b).SquareLength();
}

constexpr int64_t Point::DotProduct(const Point a, const Point b) noexcept {
  return a.x * b.x + a.y * b.y;
}

constexpr int64_t Point::SkewProduct(const Point a, const Point b) noexcept {
  return a.x * b.y - a.y * b.x;
}

constexpr bool operator==(const Point a, const Point b) noexcept {
  return a.x == b.x && a.y == b.y;
}

constexpr bool operator!=(const Point a, const Point b) noexcept {
  return !(a == b);
}

constexpr bool operator<(const Point a, const Point b) noexcept {
  return a.y < b.y || (a.y == b.y && a.x < b.x);
}

#pragma once

#include <cstdint>

struct Point {
  int64_t x;
  int64_t y;

  explicit constexpr Point(int64_t x = 0, int64_t y = 0) noexcept;
  constexpr Point(Point a, Point b) noexcept;

  [[nodiscard]] constexpr int64_t SquareLength() const noexcept;

  [[nodiscard]] static constexpr int64_t SquareDistance(Point a,
                                                        Point b) noexcept;
  [[nodiscard]] static constexpr int64_t SkewProduct(Point a, Point b) noexcept;
};

[[nodiscard]] constexpr bool operator==(Point a, Point b) noexcept;
[[nodiscard]] constexpr bool operator!=(Point a, Point b) noexcept;

inline constexpr Point::Point(int64_t x, int64_t y) noexcept : x{x}, y{y} {}

inline constexpr Point::Point(Point a, Point b) noexcept
    : x{b.x - a.x}, y{b.y - a.y} {}

inline constexpr int64_t Point::SquareLength() const noexcept {
  return x * x + y * y;
}

inline constexpr int64_t Point::SquareDistance(Point a, Point b) noexcept {
  return Point(a, b).SquareLength();
}

inline constexpr int64_t Point::SkewProduct(Point a, Point b) noexcept {
  return a.x * b.y - a.y * b.x;
}

inline constexpr bool operator==(Point a, Point b) noexcept {
  return a.x == b.x && a.y == b.y;
}

inline constexpr bool operator!=(Point a, Point b) noexcept {
  return !(a == b);
}

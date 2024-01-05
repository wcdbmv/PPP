#include <gtest/gtest.h>

#include "orientation.h"

TEST(Orientation, Collinear1) {
  constexpr Point p{0, 0};
  constexpr Point q{0, 10};
  constexpr Point r{0, 20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCollinear);
}

TEST(Orientation, Collinear2) {
  constexpr Point p{0, 0};
  constexpr Point q{0, -10};
  constexpr Point r{0, -20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCollinear);
}

TEST(Orientation, Collinear3) {
  constexpr Point p{0, 0};
  constexpr Point q{10, 0};
  constexpr Point r{20, 0};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCollinear);
}

TEST(Orientation, Collinear4) {
  constexpr Point p{0, 0};
  constexpr Point q{-10, 0};
  constexpr Point r{-20, 0};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCollinear);
}

TEST(Orientation, Collinear5) {
  constexpr Point p{-10, -10};
  constexpr Point q{0, 10};
  constexpr Point r{10, 30};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCollinear);
}

TEST(Orientation, Counterclockwise1) {
  constexpr Point p{0, 0};
  constexpr Point q{0, 10};
  constexpr Point r{-5, 20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Counterclockwise2) {
  constexpr Point p{0, 0};
  constexpr Point q{0, -10};
  constexpr Point r{5, -20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Counterclockwise3) {
  constexpr Point p{0, 0};
  constexpr Point q{10, 0};
  constexpr Point r{20, 5};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Counterclockwise4) {
  constexpr Point p{0, 0};
  constexpr Point q{-10, 0};
  constexpr Point r{-20, -5};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Counterclockwise5) {
  constexpr Point p{-10, -10};
  constexpr Point q{0, 10};
  constexpr Point r{10, 35};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Counterclockwise6) {
  // y = 2x + 1.
  constexpr Point p{20, 50};
  constexpr Point q{30, 70};
  constexpr Point r{35, 90};

  ASSERT_EQ(Orientation(p, q, r), Direction::kCounterclockwise);
}

TEST(Orientation, Clockwise1) {
  constexpr Point p{0, 0};
  constexpr Point q{0, 10};
  constexpr Point r{5, 20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

TEST(Orientation, Clockwise2) {
  constexpr Point p{0, 0};
  constexpr Point q{0, -10};
  constexpr Point r{-5, -20};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

TEST(Orientation, Clockwise3) {
  constexpr Point p{0, 0};
  constexpr Point q{10, 0};
  constexpr Point r{20, -5};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

TEST(Orientation, Clockwise4) {
  constexpr Point p{0, 0};
  constexpr Point q{-10, 0};
  constexpr Point r{-20, 5};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

TEST(Orientation, Clockwise5) {
  constexpr Point p{-10, -10};
  constexpr Point q{0, 10};
  constexpr Point r{10, 25};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

TEST(Orientation, Clockwise6) {
  // y = 2x + 1.
  constexpr Point p{40, 90};
  constexpr Point q{50, 110};
  constexpr Point r{65, 130};

  ASSERT_EQ(Orientation(p, q, r), Direction::kClockwise);
}

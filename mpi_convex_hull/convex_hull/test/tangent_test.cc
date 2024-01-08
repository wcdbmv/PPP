#include <gtest/gtest.h>

#include "tangent.h"

TEST(Tangent, Cloud1) {
  constexpr Point a{100, -100};
  constexpr Point b{200, -50};
  constexpr Point c{100, 50};
  constexpr Point d{50, 40};
  constexpr Point e{0, 0};
  const PointCloud hull{a, b, c, d, e};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);

  tangent = FindRightTangent(hull, Point{0, -150});
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, Point{180, -60});
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, Point{300, 0});
  ASSERT_EQ(tangent, c);
}

TEST(Tangent, Cloud2) {
  constexpr Point a{129752, -488227};
  constexpr Point b{454065, -84320};
  constexpr Point c{496921, 490968};
  constexpr Point d{46027, 460385};
  constexpr Point e{-190453, 98675};
  constexpr Point f{-199647, 27785};
  const PointCloud hull{a, b, c, d, e, f};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, f);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);
}

TEST(Tangent, Cloud3) {
  constexpr Point a{0, -10};
  constexpr Point b{8, -8};
  constexpr Point c{10, 0};
  constexpr Point d{8, 8};
  constexpr Point e{0, 10};
  constexpr Point f{-8, 8};
  constexpr Point g{-10, 0};
  constexpr Point h{-8, -8};
  const PointCloud hull{a, b, c, d, e, f, g, h};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, f);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, g);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, h);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);
}

TEST(Tangent, Cloud4) {
  constexpr Point a{35, -42};
  constexpr Point b{45, -18};
  constexpr Point c{46, 18};
  constexpr Point d{4, 46};
  constexpr Point e{-17, 40};
  constexpr Point f{-48, -18};
  const PointCloud hull{a, b, c, d, e, f};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, f);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);

  tangent = FindRightTangent(hull, Point{-43, -48});
  ASSERT_EQ(tangent, a);
}

TEST(Tangent, Cloud5) {
constexpr Point a{-166412, -474153};
constexpr Point b{155678, -469377};
constexpr Point c{314713, -417072};
constexpr Point d{491417, -243050};
constexpr Point e{305426, 466310};
constexpr Point f{267706, 493725};
constexpr Point g{-482112, 488033};
constexpr Point h{-496111, 319337};
constexpr Point i{-351949, -271524};
  const PointCloud hull{a, b, c, d, e, f, g, h, i};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, f);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, g);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, h);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, i);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);
}

TEST(Tangent, Cloud5_0) {
  constexpr Point a{-166412, -474153};
  constexpr Point b{87273, -427908};
  constexpr Point c{491417, -243050};
  constexpr Point d{305426, 466310};
  constexpr Point e{267706, 493725};
  constexpr Point f{-105673, 106476};
  constexpr Point g{-351949, -271524};
  const PointCloud hull{a, b, c, d, e, f, g};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, c);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, d);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, e);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, f);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, g);
  tangent = FindRightTangent(hull, tangent);
  ASSERT_EQ(tangent, a);

  tangent = FindRightTangent(hull, Point{314713, -417072});
  ASSERT_EQ(tangent, c);
}

TEST(Tangent, SinglePointCloud) {
  constexpr Point a{10, 10};
  const PointCloud hull{a};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, a);
  tangent = FindRightTangent(hull, Point{});
  ASSERT_EQ(tangent, a);
  tangent = FindRightTangent(hull, Point{20, 20});
  ASSERT_EQ(tangent, a);
}

TEST(Tangent, TwoPointCloud) {
  constexpr Point a{10, 10};
  constexpr Point b{20, 20};
  const PointCloud hull{a, b};

  auto tangent = FindRightTangent(hull, a);
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, b);
  ASSERT_EQ(tangent, a);
  tangent = FindRightTangent(hull, Point{0, 0});
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, Point{30, 30});
  ASSERT_EQ(tangent, a);

  tangent = FindRightTangent(hull, Point{16, 16});
  ASSERT_EQ(tangent, b);
  tangent = FindRightTangent(hull, Point{12, 12});
  ASSERT_EQ(tangent, b);
}

#include <gtest/gtest.h>

#include "graham_scan.h"
#include "point_cloud.h"

TEST(GrahamScan, Cloud1) {
  // Convex hull.
  constexpr Point a{100, -100};
  constexpr Point b{200, -50};
  constexpr Point c{100, 50};
  constexpr Point d{50, 40};
  constexpr Point e{0, 0};

  // Inner points.
  constexpr Point f{150, -50};
  constexpr Point g{120, 10};
  constexpr Point h{70, 30};
  constexpr Point i{25, 19};
  constexpr Point j{50, -30};

  PointCloud cloud{j, i, h, g, f, e, d, c, b, a};

  const auto hull = GrahamScan(cloud);

  EXPECT_EQ(hull.size(), 5);

  EXPECT_EQ(hull[0], a);
  EXPECT_EQ(hull[1], b);
  EXPECT_EQ(hull[2], c);
  EXPECT_EQ(hull[3], d);
  EXPECT_EQ(hull[4], e);
}

TEST(GrahamScan, Cloud2) {
  // Convex hull.
  constexpr Point a{129752, -488227};
  constexpr Point b{454065, -84320};
  constexpr Point c{496921, 490968};
  constexpr Point d{46027, 460385};
  constexpr Point e{-190453, 98675};
  constexpr Point f{-199647, 27785};

  // Inner points.
  constexpr Point g{178349, 332909};
  constexpr Point h{213640, 333725};
  constexpr Point i{134712, 373613};
  constexpr Point j{376639, 139387};
  constexpr Point k{178349, 332909};

  PointCloud cloud{g, d, c, e, a, h, b, i, f, j, k};

  const auto hull = GrahamScan(cloud);

  ASSERT_EQ(hull.size(), 6);
  EXPECT_EQ(hull[0], a);
  EXPECT_EQ(hull[1], b);
  EXPECT_EQ(hull[2], c);
  EXPECT_EQ(hull[3], d);
  EXPECT_EQ(hull[4], e);
  EXPECT_EQ(hull[5], f);
}

TEST(GrahamScan, EmptyCloud) {
  PointCloud cloud;

  const auto hull = GrahamScan(cloud);

  ASSERT_TRUE(hull.empty());
}

TEST(GrahamScan, SinglePointCloud) {
  constexpr Point a{10, 10};

  PointCloud cloud{a};

  const auto hull = GrahamScan(cloud);

  ASSERT_EQ(hull.size(), 1);
  EXPECT_EQ(hull[0], a);
}

TEST(GrahamScan, TwoPointCloud) {
  constexpr Point a{10, 10};
  constexpr Point b{20, 20};

  PointCloud cloud{a, b};

  const auto hull = GrahamScan(cloud);

  ASSERT_EQ(hull.size(), 2);
  EXPECT_EQ(hull[0], a);
  EXPECT_EQ(hull[1], b);
}

TEST(GrahamScan, CollinearCloud) {
  constexpr Point a{10, 10};
  constexpr Point b{20, 20};
  constexpr Point c{30, 30};
  constexpr Point d{40, 40};
  constexpr Point e{50, 50};
  constexpr Point f{60, 60};
  constexpr Point g{70, 70};
  constexpr Point h{80, 80};
  constexpr Point i{90, 90};

  PointCloud cloud{a, b, c, d, e, f, g, h, i};

  const auto hull = GrahamScan(cloud);

  ASSERT_EQ(hull.size(), 2);
  EXPECT_EQ(hull[0], a);
  EXPECT_EQ(hull[1], i);
}

TEST(GrahamScan, BoxCloud) {
  constexpr Point a{0, 0};
  constexpr Point b{10, 0};
  constexpr Point c{20, 0};
  constexpr Point d{20, 10};
  constexpr Point e{20, 20};
  constexpr Point f{10, 20};
  constexpr Point g{0, 20};
  constexpr Point h{0, 10};

  PointCloud cloud{a, b, c, d, e, f, g, h};

  const auto hull = GrahamScan(cloud);

  ASSERT_EQ(hull.size(), 4);
  EXPECT_EQ(hull[0], a);
  EXPECT_EQ(hull[1], c);
  EXPECT_EQ(hull[2], e);
  EXPECT_EQ(hull[3], g);
}

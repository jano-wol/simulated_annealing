#include <gmock/gmock.h>

#include <random>

#include "./../../../../applications/graphs/bridges/bridges.h"

namespace
{
void testBridges(Graph g, std::vector<size_t> expected)
{
  auto resultNaive = bridgesNaive(g);
  auto resultDisjointSetUnion = bridgesDisjointSetUnion(g);
  std::sort(expected.begin(), expected.end());
  std::sort(resultNaive.begin(), resultNaive.end());
  std::sort(resultDisjointSetUnion.begin(), resultDisjointSetUnion.end());
  EXPECT_EQ(resultNaive, expected);
  EXPECT_EQ(resultDisjointSetUnion, expected);
}

void testRandomGraph(size_t n, double p)
{
  std::vector<std::pair<size_t, size_t>> edges;
  std::mt19937 e;
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < n; ++j) {
      if (static_cast<double>(e() % 100) < p * 100.0) {
        edges.push_back({i, j});
      }
    }
  }
  Graph g(n, std::move(edges));
  auto resultNaive = bridgesNaive(g);
  auto resultDisjointSetUnion = bridgesDisjointSetUnion(g);
  std::sort(resultNaive.begin(), resultNaive.end());
  std::sort(resultDisjointSetUnion.begin(), resultDisjointSetUnion.end());
  EXPECT_EQ(resultNaive, resultDisjointSetUnion);
}

}  // namespace

TEST(Bridges, TestBridges)
{
  testBridges({0, {}}, {});
  testBridges({1, {{}}}, {});
  testBridges({1, {{0, 0}}}, {});
  testBridges({2, {{0, 1}}}, {0});
  testBridges({2, {{0, 0}, {1, 1}}}, {});
  testBridges({3, {{0, 1}, {1, 2}}}, {1, 0});
  testBridges({3, {{0, 1}, {1, 2}, {2, 0}}}, {});
  testBridges({4, {{0, 1}, {0, 2}, {0, 3}}}, {0, 1, 2});
  testBridges({4, {{0, 1}, {0, 2}, {0, 3}, {1, 2}}}, {2});
  testBridges({4, {{0, 1}, {0, 2}, {0, 3}, {1, 3}}}, {1});
  testBridges({4, {{0, 1}, {0, 2}, {0, 3}, {2, 3}}}, {0});
  testBridges({4, {{0, 1}, {0, 2}, {0, 3}, {2, 3}, {1, 2}}}, {});
  testBridges({7, {{0, 1}, {1, 2}, {0, 3}, {3, 4}, {0, 5}, {5, 6}}}, {0, 1, 2, 3, 4, 5});
  testBridges({7, {{0, 1}, {1, 2}, {0, 3}, {3, 4}, {0, 5}, {5, 6}, {2, 6}}}, {2, 3});
  testBridges({7, {{0, 1}, {1, 2}, {0, 3}, {3, 4}, {0, 5}, {5, 6}, {2, 5}}}, {2, 3, 5});
  testBridges({7, {{0, 1}, {1, 2}, {0, 3}, {3, 4}, {0, 5}, {5, 6}, {2, 0}}}, {2, 3, 4, 5});
  testRandomGraph(15, 0.0);
  testRandomGraph(15, 0.01);
  testRandomGraph(15, 0.04);
  testRandomGraph(15, 0.05);
  testRandomGraph(15, 0.06);
  testRandomGraph(15, 0.1);
  testRandomGraph(15, 0.15);
  testRandomGraph(16, 0.03);
  testRandomGraph(24, 0.01);
  testRandomGraph(24, 0.02);
  testRandomGraph(24, 0.03);
  testRandomGraph(24, 0.04);
  testRandomGraph(24, 0.05);
  testRandomGraph(25, 0.04);
  for (int i = 5; i < 15; ++i) {
    for (int j = 0; j < 20; ++j) {
      testRandomGraph(i, double(j) * 0.01);
    }
  }
}

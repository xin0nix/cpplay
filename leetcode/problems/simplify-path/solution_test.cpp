
#include <gtest/gtest.h>

#include <deque>
#include <ranges>
#include <string>
#include <string_view>
namespace rng = std::ranges;
namespace view = std::views;
struct Solution {

  std::string simplifyPath(std::string path) {
    using std::operator""sv;
    std::deque<std::string_view> nodes;
    for (const auto node : view::split(path, "/"sv)) {
      auto nodeView = std::string_view(node);
      const auto size = nodeView.size();
      // Пропускаем пустое пространство между косыми чертами
      if (size == 0) {
        continue;
      }
      if (nodeView == ".") {
        continue;
      }
      if (nodeView == "..") {
        if (!nodes.empty()) {
          nodes.pop_back();
        }
        continue;
      }
      nodes.push_back(nodeView);
    }
    if (nodes.empty()) {
      return "/";
    }
    std::stringstream canonical;
    for (auto node : nodes | view::all) {
      canonical << '/' << node;
    }
    return canonical.str();
  }
};

class SimplifyPathTest : public ::testing::Test {
protected:
  Solution solution;
};

TEST_F(SimplifyPathTest, RemoveTrailingSlash) {
  EXPECT_EQ(solution.simplifyPath("/home/"), "/home");
}

TEST_F(SimplifyPathTest, RemoveConsecutiveSlashes) {
  EXPECT_EQ(solution.simplifyPath("/home//foo/"), "/home/foo");
}

TEST_F(SimplifyPathTest, HandleParentDirectory) {
  EXPECT_EQ(solution.simplifyPath("/home/user/Documents/../Pictures"),
            "/home/user/Pictures");
}

TEST_F(SimplifyPathTest, HandleRootDirectory) {
  EXPECT_EQ(solution.simplifyPath("/../"), "/");
}

TEST_F(SimplifyPathTest, HandleComplexPath) {
  EXPECT_EQ(solution.simplifyPath("/.../a/../b/c/../d/./"), "/.../b/d");
}
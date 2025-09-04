#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

struct DirReduction {
  static bool opposite(const std::string &dirA, const std::string &dirB) {
    return (dirA == "NORTH" && dirB == "SOUTH") ||
           (dirA == "SOUTH" && dirB == "NORTH") ||
           (dirA == "EAST" && dirB == "WEST") ||
           (dirA == "WEST" && dirB == "EAST");
  }

  static std::vector<std::string>
  dirReduc(const std::vector<std::string> &arr) {
    std::vector<std::string> res;
    for (const auto &dir : arr) {
      if (!res.empty() && opposite(res.back(), dir)) {
        res.pop_back();
      } else {
        res.push_back(dir);
      }
    }
    return res;
  }
};

TEST(dirReduc_Tests, Examples) {
  {
    SCOPED_TRACE("Case1");
    std::vector<std::string> data = {"NORTH", "SOUTH", "SOUTH", "EAST",
                                     "WEST",  "NORTH", "WEST"};
    std::vector<std::string> ans = DirReduction::dirReduc(data);
    std::vector<std::string> sol = {"WEST"};
    EXPECT_THAT(ans, ::testing::ElementsAreArray(sol));
  }
  {
    SCOPED_TRACE("Case2");
    std::vector<std::string> data = {"NORTH", "SOUTH", "SOUTH", "EAST",
                                     "WEST",  "NORTH", "NORTH"};
    std::vector<std::string> ans = DirReduction::dirReduc(data);
    std::vector<std::string> sol = {"NORTH"};
    EXPECT_THAT(ans, ::testing::ElementsAreArray(sol));
  }
}

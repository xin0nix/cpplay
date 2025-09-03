#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

struct DirReduction {
  static bool opposite(char dirA, char dirB) {
    // N <-> S
    // W <-> E
    return (dirA == 'N' and dirB == 'S') or (dirA == 'S' and dirB == 'N') or
           (dirA == 'W' and dirB == 'E') or (dirA == 'E' and dirB == 'W');
  }

  static std::vector<std::string>
  dirReduc(const std::vector<std::string> &arr) {
    auto res = arr;
    bool changed = true;
    auto prev = arr.end();
    while (changed) {
      changed = false;
      auto cur = res.begin();
      for (auto end = res.end(); cur != end; ++cur) {
        if (prev == arr.end()) {
          prev = cur;
          continue;
        }
        if (opposite(prev->front(), cur->front())) {
          changed = true;
          break;
        }
      }
      if (changed) {
        res.erase(prev, cur + 1);
      }
    }
    return res;
  }
};

TEST(dirReduc_Tests, Examples) {
  {
    SCOPED_TRACE("Helper");
    EXPECT_TRUE(DirReduction::opposite('N', 'S'));
    EXPECT_TRUE(DirReduction::opposite('S', 'N'));
    EXPECT_TRUE(DirReduction::opposite('E', 'W'));
    EXPECT_TRUE(DirReduction::opposite('W', 'E'));
    EXPECT_FALSE(DirReduction::opposite('N', 'N'));
    EXPECT_FALSE(DirReduction::opposite('N', 'E'));
    EXPECT_FALSE(DirReduction::opposite('S', 'E'));
    EXPECT_FALSE(DirReduction::opposite('S', 'W'));
  }
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

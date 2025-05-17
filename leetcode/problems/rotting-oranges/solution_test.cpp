#include <gtest/gtest.h>
#include <queue>
#include <vector>

struct Solution {
  static constexpr int kEmpty = 0;
  static constexpr int kFresh = 1;
  static constexpr int kRotten = 2;
  const std::vector<std::pair<int, int>> kShifts = {
      {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  int orangesRotting(std::vector<std::vector<int>> &grid) {
    struct Cell {
      int row;
      int col;
      int distance{0};
    };
    std::queue<Cell> queue;
    const int numRows = grid.size();
    const int numCols = grid.front().size();
    auto visitAll = [&grid, numRows, numCols](auto cb) {
      for (int row = 0; row < (int)numRows; ++row) {
        for (int col = 0; col < (int)numCols; ++col) {
          cb(row, col, grid[row][col]);
        }
      }
    };
    visitAll([&queue](int row, int col, int val) {
      if (val == kRotten) {
        queue.push(Cell{row, col});
      }
    });
    int res = 0;
    auto exists = [numRows, numCols](int row, int col) {
      return 0 <= row and row < numRows and 0 <= col and col < numCols;
    };
    while (not queue.empty()) {
      auto cell = queue.front();
      queue.pop();
      for (auto &&[shiftRow, shiftCol] : kShifts) {
        int row = cell.row + shiftRow;
        int col = cell.col + shiftCol;
        if (not exists(row, col)) {
          continue;
        }
        auto &&val = grid[row][col];
        if (val != kFresh) {
          continue;
        }
        val = kRotten;
        int distance = cell.distance + 1;
        res = std::max<int>(res, distance);
        queue.push(Cell{row, col, distance});
      }
    }
    visitAll([&res](int, int, int val) {
      if (val == kFresh) {
        res = -1;
      }
    });
    return res;
  }
};

// Структура для параметров теста: вход и ожидаемый результат
struct TestParam {
  std::vector<std::vector<int>> grid;
  int expected;
};

// Класс теста с параметрами
class RottingOrangesTest : public ::testing::TestWithParam<TestParam> {};

TEST_P(RottingOrangesTest, HandlesVariousCases) {
  Solution sol;
  auto param = GetParam();
  // Копируем вход, так как метод принимает по ссылке и может менять
  std::vector<std::vector<int>> grid = param.grid;
  int result = sol.orangesRotting(grid);
  ASSERT_EQ(result, param.expected);
}

// Набор тестовых данных
INSTANTIATE_TEST_SUITE_P(
    RottingOrangesTests, RottingOrangesTest,
    ::testing::Values(
        // Пример из условия: все апельсины гниют за 4 минуты
        TestParam{{{2, 1, 1}, {1, 1, 0}, {0, 1, 1}}, 4},
        // Пример, когда некоторые апельсины не могут загнить
        TestParam{{{2, 1, 1}, {0, 1, 1}, {1, 0, 1}}, -1},
        // Нет свежих апельсинов, время 0
        TestParam{{{0, 2}}, 0},
        // Все пустые клетки
        TestParam{{{0, 0}, {0, 0}}, 0},
        // Все апельсины уже гнилые
        TestParam{{{2, 2}, {2, 2}}, 0},
        // Один свежий апельсин без гнилых рядом
        TestParam{{{1}}, -1}));

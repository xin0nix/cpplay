#include <gtest/gtest.h>

#include <queue>
#include <string>
#include <vector>

using namespace std;

#include <limits>
#include <optional>
#include <queue>
#include <vector>

using namespace std;

namespace {
constexpr int kWatr = -1;
constexpr int kChst = 0;
constexpr int kInfi = numeric_limits<int>::max();
} // namespace

class Solution {
public:
  void islandsAndTreasure(vector<vector<int>> &grid) {
    if (grid.empty())
      return;

    int numRows = static_cast<int>(grid.size());
    int numCols = static_cast<int>(grid[0].size());

    using Point = pair<int, int>;

    // Проверка валидности координат
    auto isValid = [&](int row, int col) -> bool {
      return row >= 0 && row < numRows && col >= 0 && col < numCols;
    };

    struct Entry {
      Point cell;
      int distance;
      Entry(Point c, int d) : cell(c), distance(d) {}
    };

    queue<Entry> waitList;

    // Добавляем все сундуки в очередь с расстоянием 0
    for (int row = 0; row < numRows; ++row) {
      for (int col = 0; col < numCols; ++col) {
        if (grid[row][col] == kChst) {
          waitList.push(Entry(Point(row, col), 0));
        }
      }
    }

    // Смещения соседних клеток: вверх, вниз, влево, вправо
    const vector<Point> kShiftMatrix = {Point(0, 1), Point(0, -1), Point(1, 0),
                                        Point(-1, 0)};

    while (!waitList.empty()) {
      Entry entry = waitList.front();
      waitList.pop();

      int row = entry.cell.first;
      int col = entry.cell.second;

      // Обновляем расстояние в текущей ячейке
      grid[row][col] = entry.distance;

      // Обход соседей
      for (size_t i = 0; i < kShiftMatrix.size(); ++i) {
        int nRow = row + kShiftMatrix[i].first;
        int nCol = col + kShiftMatrix[i].second;

        if (!isValid(nRow, nCol))
          continue;

        int val = grid[nRow][nCol];

        // Пропускаем воду и уже посещённые клетки (значение меньше INF)
        if (val < kInfi)
          continue;

        // Обновляем расстояние и добавляем в очередь
        grid[nRow][nCol] = entry.distance + 1;
        waitList.push(Entry(Point(nRow, nCol), entry.distance + 1));
      }
    }
  }
};

using Grid = vector<vector<int>>;

struct TestCase {
  Grid input;
  Grid expected;
};

// Параметризованный тестовый класс
class IslandsAndTreasureTest : public ::testing::TestWithParam<TestCase> {
protected:
  Solution sol;
};

TEST_P(IslandsAndTreasureTest, HandlesVariousCases) {
  TestCase tc = GetParam();
  sol.islandsAndTreasure(tc.input);
  EXPECT_EQ(tc.input, tc.expected);
}

// Массив тестовых кейсов с ASCII-графиками в комментариях
static const TestCase test_cases[] =
    {
        // EmptyGrid
        // (пусто)
        {{}, {}},

        // NoTreasures
        /*
          .   W   .
          .   W   .
          .   .   .
        */
        {{
             {kInfi, kWatr, kInfi},
             {kInfi, kWatr, kInfi},
             {kInfi, kInfi, kInfi},
         },
         {
             {kInfi, kWatr, kInfi},
             {kInfi, kWatr, kInfi},
             {kInfi, kInfi, kInfi},
         }},

        // SingleTreasure
        /*
          .   W   C
          .   .   .
          .   W   .
        */
        {{
             {kInfi, kWatr, kChst},
             {kInfi, kInfi, kInfi},
             {kInfi, kWatr, kInfi},
         },
         {
             {4, kWatr, kChst},
             {3, 2, 1},
             {4, kWatr, 2},
         }},

        // MultipleTreasures
        /*
          C   .   .
          .   W   .
          .   .   C
        */
        {{
             {kChst, kInfi, kInfi},
             {kInfi, kWatr, kInfi},
             {kInfi, kInfi, kChst},
         },
         {
             {kChst, 1, 2},
             {1, kWatr, 1},
             {2, 1, kChst},
         }},

        // UnreachableCells
        /*
          C   W   .
          W   W   .
          .   .   .
        */
        {
            {
                {kChst, kWatr, kInfi},
                {kWatr, kWatr, kInfi},
                {kInfi, kInfi, kInfi},
            },
            {
                {kChst, kWatr, kInfi},
                {kWatr, kWatr, kInfi},
                {kInfi, kInfi, kInfi},
            },
        },
}

;

// Функция для генерации имён тестов по индексу
std::string TestNameGenerator(const ::testing::TestParamInfo<TestCase> &info) {
  switch (info.index) {
  case kChst:
    return "EmptyGrid";
  case 1:
    return "NoTreasures";
  case 2:
    return "SingleTreasure";
  case 3:
    return "MultipleTreasures";
  case 4:
    return "UnreachableCells";
  default:
    return "UnknownTest";
  }
}

INSTANTIATE_TEST_SUITE_P(NamedTests, IslandsAndTreasureTest,
                         ::testing::ValuesIn(test_cases), TestNameGenerator);

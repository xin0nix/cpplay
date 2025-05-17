#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <queue>
#include <string>
#include <vector>

/*
Будем использовать BFS. Начинать с 'X' не имеет смысла, т.к. когда мы достигнем
клетку 'O' на границе сетки то для ответа придётся откатываться назад (как мы
иначе поймём что именно нужно исключить)? К тому же непонятно до какого момента
откатываться если мы шли по принципу "обошли и забыли". Поэтому будем
действовать в два прохода. На первом проходе будем выполнять BFS начиная с
клеток 'O' на краях сетки. Сначала добавим все такие ячейки с 'O' в очередь Q, и
заменим их выделенным символом 'V' (visited). Далее будем брать по одной ячейке
из начала очереди, обходить соседей, и если кто-то из них 'O', добавлять их в
конец. Таким образом мы в конце концов "закрасим" все неокруженные регионы в
'V'. На втором проходе просто обойдём все ячейки и будем менять их значения по
следующему правилу:
*   'X' -> 'X', то есть мы не меняем значение 'X'
*   'O' -> 'X', мы знаем что 'O' не попала в список "неокруженных" ячеек,
следовательно она в списке окруженных (двойное отрицание)
*   'V' -> 'O', мы знаем что эти ячейки часть открытых регионов, возвращаем как
было
*/
struct Solution {
  static constexpr char kCross = 'X';
  static constexpr char kNought = 'O';
  static constexpr char kVisited = 'V';
  const std::vector<std::pair<int, int>> kShifts = {
      {0, 1}, {0, -1}, {1, 0}, {-1, 0}};
  void solve(std::vector<std::vector<char>> &board) {
    std::queue<std::pair<int, int>> queue;
    if (board.empty()) {
      return;
    }
    const int numRows = board.size();
    const int numCols = board.front().size();
    // Первый проход - поиск открытых регионов
    // o o o o o
    // o . . . o
    // o . . . o
    // o . . . o
    // o o o o o
    auto checkAndEnqueue = [&queue, &board](int row, int col) {
      if (board[row][col] == kNought) {
        queue.push({row, col});
      }
    };
    for (int i = 0; i < numRows; ++i) {
      checkAndEnqueue(i, 0);
      checkAndEnqueue(i, numCols - 1);
    }
    for (int j = 0; j < numCols; ++j) {
      checkAndEnqueue(0, j);
      checkAndEnqueue(numRows - 1, j);
    }
    auto exists = [numRows, numCols](int row, int col) {
      return 0 <= row and row < numRows and 0 <= col and col < numCols;
    };
    while (not queue.empty()) {
      auto [row, col] = queue.front();
      queue.pop();
      auto &cell = board[row][col];
      cell = kVisited;
      for (auto [rowShift, colShift] : kShifts) {
        auto nextRow = row + rowShift;
        auto nextCol = col + colShift;
        if (not exists(nextRow, nextCol)) {
          continue;
        }
        auto &nextCell = board[nextRow][nextCol];
        if (nextCell == kNought) {
          queue.push({nextRow, nextCol});
        }
      }
    }
    // Второй проход - преобразование и восстановление
    for (int i = 0; i < numRows; ++i) {
      for (int j = 0; j < numCols; ++j) {
        char &cell = board[i][j];
        if (cell == kCross) {
          ;
        } else if (cell == kNought) {
          cell = kCross;
        } else {
          cell = kNought;
        }
      }
    }
  }
};

using Board = std::vector<std::vector<char>>;

// Утилита для удобного создания доски из вектора строк
Board makeBoard(const std::vector<std::string> &rows) {
  Board board;
  for (const auto &row : rows) {
    board.emplace_back(row.begin(), row.end());
  }
  return board;
}

struct SurroundedRegionsTest
    : ::testing::TestWithParam<std::tuple<Board, Board>> {
  Solution solution;
};

TEST_P(SurroundedRegionsTest, HandlesVariousBoards) {
  auto [input, expected] = GetParam();
  solution.solve(input);
  assert(input.size() == expected.size());
  for (int i = 0; i < expected.size(); ++i) {
    auto &inputRow = input[i];
    auto &expRow = expected[i];
    EXPECT_THAT(inputRow, ::testing::ElementsAreArray(expRow))
        << i << "th row mismatched";
  }
}

INSTANTIATE_TEST_SUITE_P(
    SurroundedRegionsTests, SurroundedRegionsTest,
    ::testing::Values(
        // Пример 1: пустая доска
        std::make_tuple(Board{}, Board{}),

        // Пример 2: доска без 'O'
        std::make_tuple(makeBoard({
                            "XXX",
                            "XXX",
                            "XXX",
                        }),
                        makeBoard({
                            "XXX",
                            "XXX",
                            "XXX",
                        })),

        // Пример 3: 'O' на границе, не окружены
        std::make_tuple(makeBoard({
                            "XOX",
                            "OXO",
                            "XOX",
                        }),
                        makeBoard({
                            "XOX",
                            "OXO",
                            "XOX",
                        })),

        // Пример 4: 'O' окружены и должны стать 'X'
        std::make_tuple(makeBoard({
                            "XXXX",
                            "XOOX",
                            "XXOX",
                            "XOXX",
                        }),
                        makeBoard({
                            "XXXX",
                            "XXXX",
                            "XXXX",
                            "XOXX",
                        })),

        // Пример 5: вся доска 'O' (все не окружены, т.к. на границе)
        std::make_tuple(makeBoard({
                            "OOO",
                            "OOO",
                            "OOO",
                        }),
                        makeBoard({
                            "OOO",
                            "OOO",
                            "OOO",
                        })),

        // Пример 6: сложный случай с несколькими регионами
        std::make_tuple(makeBoard({
                            "XOOOX",
                            "OXOXO",
                            "XOXOX",
                            "OXOOX",
                            "XXXXX",
                        }),
                        makeBoard({
                            "XOOOX",
                            "OXOXO",
                            "XXXXX",
                            "OXXXX",
                            "XXXXX",
                        }))));

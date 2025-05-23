#include <gtest/gtest.h>

#include <deque>
#include <queue>
#include <vector>

// Предполагается, что класс KthLargest уже реализован где-то
class KthLargest {
  std::priority_queue<int, std::deque<int>, std::greater<int>> streamQueue;
  int limit{0};

public:
  KthLargest(int limit_, std::vector<int> &numbers_) : limit(limit_) {
    for (int val : numbers_) {
      streamQueue.push(val);
      if (streamQueue.size() > limit_) {
        streamQueue.pop();
      }
    }
  }

  int add(int val) {
    streamQueue.push(val);
    if (streamQueue.size() > limit) {
      streamQueue.pop();
    }
    return streamQueue.top();
  }
};

// Структура для параметризации тестов
struct KthLargestTestParam {
  int k;
  std::vector<int> initial_nums;
  std::vector<std::pair<int, int>>
      adds_and_expected; // {добавляемое значение, ожидаемый k-й элемент}
};

class KthLargestTest : public ::testing::TestWithParam<KthLargestTestParam> {};

TEST_P(KthLargestTest, HandlesVariousInputs) {
  const auto &param = GetParam();
  KthLargest kthLargest(param.k,
                        const_cast<std::vector<int> &>(param.initial_nums));

  for (const auto &[val, expected] : param.adds_and_expected) {
    int result = kthLargest.add(val);
    EXPECT_EQ(result, expected) << "Failed on add(" << val << ")";
  }
}

INSTANTIATE_TEST_SUITE_P(
    KthLargestTests, KthLargestTest,
    ::testing::Values(
        // Пример из задачи с повторяющимися элементами
        KthLargestTestParam{
            3, {4, 5, 8, 2}, {{3, 4}, {5, 5}, {10, 5}, {9, 8}, {4, 8}}},
        // k=1, всегда возвращаем максимум
        KthLargestTestParam{1, {2, 3, 4}, {{1, 4}, {5, 5}, {0, 5}}},
        // Пустой начальный массив
        KthLargestTestParam{2, {}, {{3, 3}, {5, 3}, {10, 5}, {9, 9}}},
        // Все элементы одинаковые
        KthLargestTestParam{2, {5, 5, 5}, {{5, 5}, {4, 5}, {6, 5}, {7, 6}}},
        // Добавляем элементы меньше текущего k-го
        KthLargestTestParam{
            3, {10, 20, 30, 40}, {{5, 20}, {25, 25}, {35, 30}, {15, 30}}},
        // Большие значения и отрицательные
        KthLargestTestParam{
            4, {1000, 2000, -1000}, {{500, -1000}, {3000, 500}, {1500, 1000}}}));

INSTANTIATE_TEST_SUITE_P(
    KthLargestTestsWithNegatives, KthLargestTest,
    ::testing::Values(
        // Отрицательные и положительные числа
        KthLargestTestParam{
            3, {-10, -20, 0, 5, 15}, {{-5, 0}, {10, 5}, {-15, 5}, {20, 10}}},
        // Все отрицательные
        KthLargestTestParam{2, {-5, -1, -3}, {{-2, -2}, {-6, -2}, {0, -1}}},
        // Отрицательные и ноль
        KthLargestTestParam{
            1, {-100, 0, -50}, {{-10, 0}, {-200, 0}, {10, 10}}}));

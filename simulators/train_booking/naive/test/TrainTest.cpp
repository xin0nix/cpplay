#include "Train.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <range/v3/all.hpp>

using ::testing::ElementsAre;
using ::testing::ElementsAreArray;

class TrainTest : public testing::Test {
protected:
  app::Train train;

  void fillIn(const bool val) {
    for (auto &car : train.mCarriages) {
      for (auto &seat : car) {
        seat = val;
      }
    }
  }

  void SetUp() override { fillIn(false); }

  void TearDown() override {}
};

TEST_F(TrainTest, AllEmpty) {
  EXPECT_THAT(train.getVacantCarriages(),
              ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST_F(TrainTest, AllBookedOut) {
  fillIn(true);
  EXPECT_THAT(train.getVacantCarriages(), ElementsAre());
}

TEST_F(TrainTest, Only2nd3rdAnd6thAvailable) {
  fillIn(true);
  train.mCarriages[2][42] = false;
  train.mCarriages[3][17] = false;
  train.mCarriages[6][13] = false;
  EXPECT_THAT(train.getVacantCarriages(), ElementsAre(2, 3, 6));
}

TEST_F(TrainTest, CarriageOutOfBounts) {
  EXPECT_ANY_THROW(train.getVacantSeats(999));
}

TEST_F(TrainTest, AllCarriagesBookedOut) {
  fillIn(true);
  for (size_t i = 0; i < app::kNumCarriages; ++i) {
    EXPECT_THAT(train.getVacantSeats(i), ElementsAre());
  }
}

TEST_F(TrainTest, AllCarriagesVacant) {
  auto expected = ranges::views::ints(0, 60) | ranges::to<std::vector>;
  for (size_t i = 0; i < app::kNumCarriages; ++i) {
    EXPECT_THAT(train.getVacantSeats(i), ElementsAreArray(expected));
  }
}

TEST_F(TrainTest, SomeSeatsBookedInCar6) {
  train.mCarriages[6][0] = true;
  train.mCarriages[6][3] = true;
  train.mCarriages[6][7] = true;
  train.mCarriages[6][8] = true;
  train.mCarriages[6][11] = true;
  train.mCarriages[6][17] = true;
  train.mCarriages[6][23] = true;
  train.mCarriages[6][32] = true;
  train.mCarriages[6][42] = true;
  train.mCarriages[6][43] = true;
  std::vector<int> expected =
      ranges::views::ints(0, 60) | ranges::to<std::vector>;
  expected[0] = -1;
  expected[3] = -1;
  expected[7] = -1;
  expected[8] = -1;
  expected[11] = -1;
  expected[17] = -1;
  expected[23] = -1;
  expected[32] = -1;
  expected[42] = -1;
  expected[43] = -1;
  auto filteredExpected = expected |
                          ranges::views::filter([](int v) { return v >= 0; }) |
                          ranges::to<std::vector>;
  EXPECT_THAT(train.getVacantSeats(6), ElementsAreArray(filteredExpected));
}

TEST_F(TrainTest, BookOutOfRange) {
  EXPECT_ANY_THROW(train.tryToBook(13, 0));
  EXPECT_ANY_THROW(train.tryToBook(0, 99));
  EXPECT_ANY_THROW(train.tryToBook(13, 99));
}

TEST_F(TrainTest, BookSuccess) {
  EXPECT_TRUE(train.tryToBook(0, 0));
  EXPECT_TRUE(train.tryToBook(3, 42));
  EXPECT_TRUE(train.tryToBook(6, 13));
}

TEST_F(TrainTest, BookFailure) {
  fillIn(true);
  EXPECT_FALSE(train.tryToBook(0, 0));
  EXPECT_FALSE(train.tryToBook(3, 42));
  EXPECT_FALSE(train.tryToBook(6, 13));
}

#include "Train.hpp"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::ElementsAre;

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
  for (size_t i = 0; i < app::kNumCarriages; ++i) {
    EXPECT_THAT(train.getVacantSeats(i),
                ElementsAre(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
  }
}

TEST_F(TrainTest, SomeSeatsBookedInCar6) {
  train.mCarriages[6][0] = true;
  train.mCarriages[6][3] = true;
  train.mCarriages[6][7] = true;
  train.mCarriages[6][8] = true;
  train.mCarriages[6][11] = true;
  EXPECT_THAT(train.getVacantSeats(6),
              ElementsAre(1, 2, 4, 5, 6, 9, 10, 12, 13, 14));
}

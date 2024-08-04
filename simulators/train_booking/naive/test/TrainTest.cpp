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

  void SetUp() override {}

  void TearDown() override {}
};

TEST_F(TrainTest, AllEmpty) {
  fillIn(false);
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

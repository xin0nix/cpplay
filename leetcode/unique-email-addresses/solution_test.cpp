#include <gtest/gtest.h>
#include <string>
#include <vector>
using std::string;
using std::vector;

class Solution {
public:
  int numUniqueEmails(vector<string> &emails) {}
};

TEST(UniqueEmailAddressesTest, EmptyTest) {
  Solution solution;
  vector<string> emails = {};
  EXPECT_EQ(solution.numUniqueEmails(emails), 0);
}

TEST(UniqueEmailAddressesTest, BasicLeet1) {
  Solution solution;
  vector<string> emails = {
      "a@leetcode.com",
      "b@leetcode.com",
      "c@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 3);
}

TEST(UniqueEmailAddressesTest, BasicLeet2) {
  Solution solution;
  vector<string> emails = {
      "test.email+alex@leetcode.com",
      "test.e.mail+bob.cathy@leetcode.com",
      "testemail+david@lee.tcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

TEST(UniqueEmailAddressesTest, DotsAndPlusCombined) {
  Solution solution;
  vector<string> emails = {
      "testemail.+@leetcode.com",           "testemail+@leetcode.com",
      "testemail++@leetcode.com",           "testemail++@leetcode.com",
      "testemail+++@leetcode.com",          ".testemail+@leetcode.com",
      "testemail+.@leetcode.com",           "test.email+alex@leetcode.com",
      "test.e.mail+bob.cathy@leetcode.com", "testemail+david@lee.tcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

TEST(UniqueEmailAddressesTest, IgnoreAfterPlusTest) {
  Solution solution;
  vector<string> emails = {
      "alice@leetcode.com",      "alice+@leetcode.com",
      "alice+.@leetcode.com",    "alice.+@leetcode.com",
      "alice++@leetcode.com",    "alice+bob+@leetcode.com",
      "alice++bob@leetcode.com", "alice+.+bob++@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

TEST(UniqueEmailAddressesTest, SkipDotTest) {
  Solution solution;
  vector<string> emails = {
      "test.email@leetcode.com", "testemail@leetcode.com",
      ".testemail@leetcode.com", "...testemail@leetcode.com",
      "testemail.@leetcode.com", "testemail...@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

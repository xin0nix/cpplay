#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::string;
using std::vector;
using std::unordered_set;
using std::stringstream;

struct BruteForce {
  int numUniqueEmails(vector<string> &emails) {
    unordered_set<string> recepients;
    for (const auto &mail : emails) {
      string ss;
      ss.reserve(mail.size());
      auto it = mail.cbegin();
      // Local name
      while (*it != '@' && *it != '+') {
        if (*it != '.') {
          ss.push_back(*it);
        }
        ++it;
      }
      // Skip everything up to @
      while (*it != '@') {
        ++it;
      }
      // Get the domain par
      while (it != mail.cend()) {
        ss.push_back(*it);
        ++it;
      }
      recepients.insert(std::move(ss));
    }
    return recepients.size();
  }
};

TEST(UniqueEmailAddressesTest, EmptyTest) {
  BruteForce solution;
  vector<string> emails = {};
  EXPECT_EQ(solution.numUniqueEmails(emails), 0);
}

TEST(UniqueEmailAddressesTest, BasicLeet1) {
  BruteForce solution;
  vector<string> emails = {
      "a@leetcode.com",
      "b@leetcode.com",
      "c@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 3);
}

TEST(UniqueEmailAddressesTest, BasicLeet2) {
  BruteForce solution;
  vector<string> emails = {
      "test.email+alex@leetcode.com",
      "test.e.mail+bob.cathy@leetcode.com",
      "testemail+david@lee.tcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 2);
}

TEST(UniqueEmailAddressesTest, DotsAndPlusCombined) {
  BruteForce solution;
  vector<string> emails = {
      "testemail.+@leetcode.com",           "testemail+@leetcode.com",
      "testemail++@leetcode.com",           "testemail++@leetcode.com",
      "testemail+++@leetcode.com",          ".testemail+@leetcode.com",
      "testemail+.@leetcode.com",           "test.email+alex@leetcode.com",
      "test.e.mail+bob.cathy@leetcode.com", "testemail+david@lee.tcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 2);
}

TEST(UniqueEmailAddressesTest, IgnoreAfterPlusTest) {
  BruteForce solution;
  vector<string> emails = {
      "alice@leetcode.com",      "alice+@leetcode.com",
      "alice+.@leetcode.com",    "alice.+@leetcode.com",
      "alice++@leetcode.com",    "alice+bob+@leetcode.com",
      "alice++bob@leetcode.com", "alice+.+bob++@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

TEST(UniqueEmailAddressesTest, SkipDotTest) {
  BruteForce solution;
  vector<string> emails = {
      "test.email@leetcode.com", "testemail@leetcode.com",
      ".testemail@leetcode.com", "...testemail@leetcode.com",
      "testemail.@leetcode.com", "testemail...@leetcode.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 1);
}

TEST(UniqueEmailAddressesTest, SameLocalNameDifferentDomains) {
  BruteForce solution;
  vector<string> emails = {
      "alice@leetcode.com",  "a.lice@leetcode.com", "alice@.leetcode.com",
      "alice@leet.code.com", "alice@leetcode+.com",
  };
  EXPECT_EQ(solution.numUniqueEmails(emails), 4);
}

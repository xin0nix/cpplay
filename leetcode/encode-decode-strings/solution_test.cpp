#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Solution {
public:
  string encode(vector<string> &strs) {
    // Each string is replaced with its size + delimiter + content
    stringstream ss;
    for (const string &s : strs)
      ss << s.size() << ';' << s;
    return ss.str();
  }

  vector<string> decode(string &str) {
    // Iterate over all the symbols
    // Read prefix until ';' is reached
    // Parse the collected symbols (if any)
    // Read n characters from the string
    vector<string> res;
    for (int i = 0, e = str.size(); i < e;) {
      int d = i;
      while (str[d] != ';')
        ++d;
      int numDigits = d - i;
      int wordLen = 0;
      if (numDigits > 0)
        wordLen = atoi(str.substr(i, numDigits).c_str());
      string word = str.substr(d + 1, wordLen);
      res.push_back(word);
      i = d + 1 + wordLen;
    }
    return res;
  }
};

class EncDecStrsTest : public ::testing::Test {
protected:
  void check(vector<string> &strs) {
    Solution s;
    auto e = s.encode(strs);
    auto r = s.decode(e);
    ASSERT_THAT(r, ::testing::ElementsAreArray(strs));
  }
};

TEST_F(EncDecStrsTest, Empty) {
  vector<string> strs;
  ASSERT_NO_FATAL_FAILURE(check(strs));
}

TEST_F(EncDecStrsTest, EmptyMultiple) {
  vector<string> strs{"", ";", ""};
  ASSERT_NO_FATAL_FAILURE(check(strs));
}

TEST_F(EncDecStrsTest, Basic0) {
  vector<string> strs{"lint", "code", "love", "you"};
  ASSERT_NO_FATAL_FAILURE(check(strs));
}

TEST_F(EncDecStrsTest, Basic1) {
  vector<string> strs{"we", "say", ":", "yes"};
  ASSERT_NO_FATAL_FAILURE(check(strs));
}

TEST_F(EncDecStrsTest, Tricky) {
  vector<string> strs{";;;", "2;;", "0;", "43;1342;143234;44;4;4;4;4;"};
  ASSERT_NO_FATAL_FAILURE(check(strs));
}

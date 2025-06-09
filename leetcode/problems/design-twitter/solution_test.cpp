#include <gtest/gtest.h>

#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using std::vector;

class Twitter {
  using UserId = int;
  using TweetId = int;
  struct Post {
    uint32_t count;
    TweetId tweetId;
    bool operator<(const Post &rhs) const { return count < rhs.count; }
  };

  uint32_t mCount{0};
  std::unordered_map<UserId, std::unordered_set<UserId>> mSocialGraph;
  std::unordered_map<UserId, std::vector<Post>> mUsersFeed;

public:
  Twitter() {}

  void postTweet(int userId, int tweetId) {
    mUsersFeed[userId].push_back(Post{mCount++, tweetId});
  }

  vector<int> getNewsFeed(int userId) {
    std::priority_queue<Post> feed;
    std::vector<TweetId> result;

    auto feedIt = mUsersFeed.find(userId);

    if (feedIt != mUsersFeed.cend()) {
      for (auto &&userPost : feedIt->second) {
        feed.push(userPost);
      }
    }

    auto userIt = mSocialGraph.find(userId);
    if (userIt != mSocialGraph.cend()) {
      for (auto friendId : userIt->second) {
        auto userPostsIt = mUsersFeed.find(friendId);
        if (userPostsIt == mUsersFeed.cend()) {
          continue;
        }
        for (auto &&userPost : userPostsIt->second) {
          feed.push(userPost);
        }
      }
    }

    auto feedLen = std::min(10UL, feed.size());
    for (auto i = 0UL; i < feedLen; ++i) {
      result.push_back(feed.top().tweetId);
      feed.pop();
    }
    return result;
  }

  void follow(int userId, int friendId) {
    mSocialGraph[userId].insert(friendId);
  }

  void unfollow(int userId, int friendId) {
    auto iter = mSocialGraph.find(userId);
    if (iter != mSocialGraph.end()) {
      iter->second.erase(friendId);
    }
  }
};

using namespace std;

class TwitterTest : public ::testing::TestWithParam<vector<string>> {
protected:
  Twitter twitter;
};

vector<string> split(const string &s, char delimiter) {
  vector<string> tokens;
  string token;
  istringstream tokenStream(s);
  while (getline(tokenStream, token, delimiter)) {
    if (!token.empty()) {
      tokens.push_back(token);
    }
  }
  return tokens;
}

TEST_P(TwitterTest, VerifyTwitterBehavior) {
  for (const auto &operation : GetParam()) {
    vector<string> parts = split(operation, ' ');
    if (parts.empty()) {
      continue;
    }

    if (parts[0] == "postTweet") {
      int userId = stoi(parts[1]);
      int tweetId = stoi(parts[2]);
      twitter.postTweet(userId, tweetId);
    } else if (parts[0] == "follow") {
      int follower = stoi(parts[1]);
      int followee = stoi(parts[2]);
      twitter.follow(follower, followee);
    } else if (parts[0] == "unfollow") {
      int follower = stoi(parts[1]);
      int followee = stoi(parts[2]);
      twitter.unfollow(follower, followee);
    } else if (parts[0] == "checkFeed") {
      int userId = stoi(parts[1]);
      vector<int> expected;
      for (size_t i = 2; i < parts.size(); ++i) {
        expected.push_back(stoi(parts[i]));
      }
      ASSERT_EQ(twitter.getNewsFeed(userId), expected);
    }
  }
}

INSTANTIATE_TEST_SUITE_P(
    TwitterFunctionality, TwitterTest,
    ::testing::Values(
        // Bug2
        vector<string>{"follow 1 5", "getNewsFeed 1"},
        // Bug1
        vector<string>{"postTweet 1 1", "getNewsFeed 1", "follow 2 1",
                       "getNewsFeed 2", "unfollow 2 1", "getNewsFeed 2"},
        // Basic posting and retrieval
        vector<string>{"postTweet 1 101", "checkFeed 1 101"},
        // Following behavior
        vector<string>{"postTweet 1 201", "postTweet 2 202", "follow 1 2",
                       "checkFeed 1 202 201"},
        // Unfollowing scenario
        vector<string>{"postTweet 1 301", "follow 1 2", "postTweet 2 302",
                       "unfollow 1 2", "checkFeed 1 301"},
        // Time ordering check
        vector<string>{"postTweet 1 401", "postTweet 1 402",
                       "checkFeed 1 402 401"},
        // 10-item feed limit
        vector<string>{"postTweet 1 501", "postTweet 1 502", "postTweet 1 503",
                       "postTweet 1 504", "postTweet 1 505", "postTweet 1 506",
                       "postTweet 1 507", "postTweet 1 508", "postTweet 1 509",
                       "postTweet 1 510", "postTweet 1 511",
                       "checkFeed 1 511 510 509 508 507 506 505 504 503 502"}));

#include "gmock/gmock.h"
#include <deque>
#include <functional>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
using ::testing::ElementsAre;

class LRUCache final {
  struct Node {
    Node() = default;
    explicit Node(int key_, int val_) : key(key_), val(val_) {}
    int key;
    int val;
    Node *prev{nullptr};
    Node *next{nullptr};
  };

  const int mCapacity;
  // L <-----------------------> R
  // least ... node, node ... most
  Node mLeastRecentlyUsed;
  Node mMostRecentlyUsed;
  unordered_map<int, Node *> mKeyNodeMap;
  deque<Node> mNodes;

public:
  LRUCache(int capacity) : mCapacity(capacity) {
    // Restore the pointers
    mLeastRecentlyUsed.next = &mMostRecentlyUsed;
    mMostRecentlyUsed.prev = &mLeastRecentlyUsed;
    // Clear the memory
    mKeyNodeMap.clear();
    mNodes.clear();
  }

  int get(int key) {
    auto it = mKeyNodeMap.find(key);
    if (it == mKeyNodeMap.end())
      return -1;
    Node *n = it->second;
    // Move to the most recent position
    evict(n);
    inject(n);
    return n->val;
  }

  void put(int key, int value) {
    auto it = mKeyNodeMap.find(key);
    // The key is already present, update the node and move up
    if (it != mKeyNodeMap.end()) {
      Node *refreshed = it->second;
      refreshed->val = value;
      evict(refreshed);
      inject(refreshed);
      assert(mNodes.size() == mKeyNodeMap.size());
      return;
    }
    // Allocate new node and make it most recently used
    if (mNodes.size() < mCapacity) {
      Node &newNode = mNodes.emplace_back(key, value);
      mKeyNodeMap[key] = &newNode;
      inject(&newNode);
      assert(mNodes.size() == mKeyNodeMap.size());
      return;
    }
    // Repurpose the least recenlty used node
    Node *replacement = mLeastRecentlyUsed.next;
    mKeyNodeMap.erase(replacement->key);
    replacement->key = key;
    replacement->val = value;
    mKeyNodeMap[key] = replacement;
    evict(replacement);
    inject(replacement);
    assert(mNodes.size() == mKeyNodeMap.size());
  }

private:
  /// Remove node from the list without deallocation
  void evict(Node *node) {
    Node *prev = node->prev;
    Node *next = node->next;
    prev->next = next;
    next->prev = prev;
  }

  /// Put node to the most recently used place, without allocation
  void inject(Node *node) {
    Node *prev = mMostRecentlyUsed.prev;
    prev->next = node;
    node->prev = prev;
    mMostRecentlyUsed.prev = node;
    node->next = &mMostRecentlyUsed;
  }
};

struct LRUCacheHandle {
  unique_ptr<LRUCache> cache;
  vector<optional<int>> results;
  void check(vector<string> commands, vector<vector<int>> inputs) {
    cache.reset();
    results.clear();
    using Handle = function<optional<int>(vector<int> &)>;
    Handle init = [&](vector<int> &data) -> optional<int> {
      cache = make_unique<LRUCache>(data[0]);
      return nullopt;
    };
    Handle put = [&](vector<int> &data) -> optional<int> {
      cache->put(data[0], data[1]);
      return nullopt;
    };
    Handle get = [&](vector<int> &data) -> optional<int> {
      return cache->get(data[0]);
    };
    unordered_map<string, Handle> executor;
    executor["LRUCache"] = init;
    executor["put"] = put;
    executor["get"] = get;
    for (int i = 0; i < commands.size(); ++i) {
      Handle handle = executor[commands[i]];
      results.push_back(handle(inputs[i]));
    }
  }
};

TEST(LRUCacheTest, LeetCodeExample1) {
  LRUCacheHandle lch;
  {

    SCOPED_TRACE("LeetCodeExample1");
    lch.check({"LRUCache", "put", "put", "get", "put", "get", "put", "get",
               "get", "get"},
              {{2}, {1, 1}, {2, 2}, {1}, {3, 3}, {2}, {4, 4}, {1}, {3}, {4}});
    EXPECT_THAT(lch.results, ElementsAre(nullopt, nullopt, nullopt, 1, nullopt,
                                         -1, nullopt, -1, 3, 4));
  }
}

TEST(LRUCacheTest, LeetCodeBug1) {
  LRUCacheHandle lch;
  lch.check({"LRUCache", "put", "put", "get", "put", "get", "put", "get", "get",
             "get"},
            {{2}, {1, 0}, {2, 2}, {1}, {3, 3}, {2}, {4, 4}, {1}, {3}, {4}});
  EXPECT_THAT(lch.results, ElementsAre(nullopt, nullopt, nullopt, 0, nullopt,
                                       -1, nullopt, -1, 3, 4));
}

TEST(LRUCacheTest, LeetCodeBug2) {
  LRUCacheHandle lch;
  lch.check({"LRUCache", "put", "put", "get", "put", "put", "get"},
            {{2}, {2, 1}, {2, 2}, {2}, {1, 1}, {4, 1}, {2}});
  EXPECT_THAT(lch.results,
              ElementsAre(nullopt, nullopt, nullopt, 2, nullopt, nullopt, -1));
}

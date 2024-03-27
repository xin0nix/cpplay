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

struct LRUCache final {
  struct Node {
    Node *prev{nullptr};
    Node *next{nullptr};
    int val{0};
  };

  int mCapacity;
  // L <--------------------> R
  // leastUsed ... ... mostUsed
  Node *leastUsed{nullptr};
  Node *mostUsed{nullptr};
  unordered_map<int, Node *> kvMap;

  LRUCache(int capacity) : mCapacity(capacity) {}

  int get(int key) {
    auto it = kvMap.find(key);
    if (it == kvMap.end())
      return -1;
    Node *n = it->second;
    // TODO: move to the right-most position
    return n->val;
  }

  void put(int key, int value) {
    // TODO: evict the least recently used item if necessary
    // TODO: put the new node to the right-most position
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
  lch.check({"LRUCache", "put", "put", "get", "put", "get", "put", "get", "get",
             "get"},
            {{2}, {1, 1}, {2, 2}, {1}, {3, 3}, {2}, {4, 4}, {1}, {3}, {4}});
}

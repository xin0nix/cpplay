#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct LRUCache {
  LRUCache(int capacity) {}

  int get(int key) {}

  void put(int key, int value) {}
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

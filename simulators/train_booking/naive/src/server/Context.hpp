#pragma once

#include <memory>
#include <string>
#include <unordered_map>

namespace app {
struct UserProfile {
  std::string mFirstName;
  std::string mLastName;
};
struct Context {
  inline static std::shared_ptr<Context> create() {
    return std::make_shared<Context>();
  }
  std::unordered_map<std::string /*uuid*/, UserProfile> mUsers;
};
struct RequestHandle {
  std::shared_ptr<Context> mContext;
  std::string mUserId;
  std::string mCorrelationId;
};
} // namespace app

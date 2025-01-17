#pragma once

#include "Server.hpp"

struct ThreadPoolServer final : Server {
  using Server::Server;
  void serve() override;
};
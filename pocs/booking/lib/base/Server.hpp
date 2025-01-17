#pragma once

#include "Storage.hpp"
#include <boost/asio.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <chrono>
#include <filesystem>

struct Server {
  Server(boost::asio::ip::tcp::endpoint endpoint, uint16_t maxBacklogSize,
         std::filesystem::path dbPath, std::size_t freeSeatsAvailable,
         std::chrono::milliseconds fakeDelay, std::size_t numThreads);

  virtual void serve() = 0;
  virtual ~Server() {}

protected:
  boost::asio::ip::tcp::endpoint endPoint_;
  uint16_t maxBacklogSize_;
  std::size_t freeSeatsAvailable_;
  std::chrono::milliseconds fakeDelay_;
  std::size_t numThreads_;
  storage::TicketStorage storage_;
};
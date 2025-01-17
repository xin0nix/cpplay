#pragma once

#include "Server.hpp"

#include "Exchange.hpp"
#include "Exchange.pb.h"
#include "Storage.hpp"
#include "boost/asio/steady_timer.hpp"
#include "boost/asio/this_coro.hpp"

#include <boost/asio.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/post.hpp>
#include <boost/asio/thread_pool.hpp>
#include <boost/asio/use_awaitable.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>

#include <filesystem>

using namespace std::chrono_literals;

using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
using boost::asio::ip::tcp;

struct CoroutineServer final : Server {
  CoroutineServer(tcp::endpoint endpoint, uint16_t maxBacklogSize,
                  std::filesystem::path dbPath, std::size_t freeSeatsAvailable,
                  std::chrono::milliseconds fakeDelay, std::size_t numThreads)
      : Server(std::move(endpoint), maxBacklogSize, std::move(dbPath),
               freeSeatsAvailable, fakeDelay, numThreads),
        ioContext_(numThreads), signalSet_(ioContext_, SIGINT, SIGTERM),
        work_(ioContext_) 
         {
    signalSet_.async_wait([this](auto, auto) { ioContext_.stop(); });
  }

  void serve() override;

private:
  // Корутина для обработки клиентского подключения
  awaitable<void> handle_client(tcp::socket socket);

  // Корутина основного цикла сервера
  awaitable<void> listen();

  boost::thread_group threads_;
  boost::asio::io_context ioContext_;
  boost::asio::signal_set signalSet_;
  boost::asio::io_context::work work_;
};

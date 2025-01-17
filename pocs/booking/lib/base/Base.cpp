#include "Server.hpp"

Server::Server(boost::asio::ip::tcp::endpoint endpoint, uint16_t maxBacklogSize,
               std::filesystem::path dbPath, std::size_t freeSeatsAvailable,
               std::chrono::milliseconds fakeDelay, std::size_t numThreads)
    : endPoint_(std::move(endpoint)), maxBacklogSize_(maxBacklogSize),
      freeSeatsAvailable_(freeSeatsAvailable), fakeDelay_(fakeDelay),
      numThreads_(numThreads), storage_(std::move(dbPath), freeSeatsAvailable) {
}
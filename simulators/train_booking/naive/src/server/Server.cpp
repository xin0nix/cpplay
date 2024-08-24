#include "BookingService.hpp"
#include "ExchangeFormat.hpp"
#include "TcpSocket.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int ac, char *av[]) {
  po::options_description desc("Allowed options");
  // clang-format off
  desc.add_options()
    ("help", "produce help message")
    ("port", po::value<int>()->default_value(8080), "set port number")
    ("address", po::value<std::string>()->default_value("127.0.0.1"), "set address")
    ("backlog", po::value<int>()->default_value(10), "set backlog size");
  // clang-format on
  po::variables_map varMap;
  po::store(po::parse_command_line(ac, av, desc), varMap);
  po::notify(varMap);

  if (varMap.count("help")) {
    // TODO: replace cout with logger
    std::cout << desc << std::endl;
    return EXIT_SUCCESS;
  }

  // TODO: server logic
  int port = varMap["port"].as<int>();
  std::string address = varMap["address"].as<std::string>();
  int backlog = varMap["backlog"].as<int>();

  std::cout << "Port: " << port << std::endl;
  std::cout << "Address: " << address << std::endl;
  std::cout << "Backlog: " << backlog << std::endl;

  using tcp_socket::TcpSocket;
  TcpSocket socket;
  socket.bind(address, port);
  socket.listen(backlog);
  std::cout << "Waiting for incoming connections..." << std::endl;
  app::BookingService bookingService;
  for (;;) {
    try {
      auto conn = socket.accept();
      std::cout << "Client IP: " << conn->mAddress << std::endl;
      std::cout << "Client Port: " << conn->mPort << std::endl;
      char buffer[1024];
      int bytesRead = ::read(conn->mFileDescriptor, buffer, sizeof buffer);
      std::string message(buffer, bytesRead);
      exchange_format::Request req;
      req.ParseFromString(message);
      const auto [userId, correlationId] = app::getClientMetaData(req);
      auto reqVar = app::fromRequest(req);
      auto responseBody = std::visit(
          app::overloaded{
              [&userId, &bookingService](app::request::Profile &msg) {
                return bookingService.handle(userId, msg);
              },
              [&userId, &bookingService](app::request::VacantCars &msg) {
                return bookingService.handle(userId, msg);
              },
              [&userId, &bookingService](app::request::VacantSeats &msg) {
                return bookingService.handle(userId, msg);
              },
              [&userId, &bookingService](app::request::TryToBook &msg) {
                return bookingService.handle(userId, msg);
              },
          },
          reqVar);
      auto response = app::toResponse(std::move(responseBody));
      app::setClientMetaData(response, {userId, correlationId});
      // TODO: send the actual response!
    } catch (...) {
      // TODO: try to send response to the active user
      std::cerr << "Unknown error, disconnect" << std::endl;
    }
  }
  return EXIT_SUCCESS;
}

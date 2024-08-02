#include "server.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

namespace po = boost::program_options;

int main(int ac, char *av[]) {
  po::options_description desc("Allowed options");
  desc.add_options()("help", "produce help message")(
      "port", po::value<int>()->default_value(8080), "set port number")(
      "address", po::value<std::string>()->default_value("localhost"),
      "set address");

  po::variables_map varMap;
  po::store(po::parse_command_line(ac, av, desc), varMap);
  po::notify(varMap);

  if (varMap.count("help")) {
    // TODO: replace cout with logger
    std::cout << desc << "\n";
    return 1;
  }

  int port = varMap["port"].as<int>();
  std::string address = varMap["address"].as<std::string>();

  std::cout << "Port: " << port << "\n";
  std::cout << "Address: " << address << "\n";

  // TODO: server logic
  return 0;
}

# Train Booking Simulation Project

## Overview

This project simulates a train booking system, focusing on low-level POSIX socket APIs, C++ concurrency, and various networking approaches. The project is divided into sub-projects, each implementing the same business logic but using different concurrency and networking techniques.

## Sub-Projects

1. **Naive Single-Threaded Blocking Server**
   - **Description**: This sub-project uses a single-threaded blocking server where each client must wait for the server to respond before proceeding.
   - **Implementation**: The server handles one client at a time, blocking until it finishes processing the request.

2. **Multi-Threaded Server with Threads and Condition Variables**
   - **Description**: This sub-project employs a multi-threaded server using threads and condition variables to manage concurrent client requests.
   - **Implementation**: Each client request is handled in a separate thread, and condition variables are used to synchronize access to shared resources.

3. **Boost.Asio Based Server**
   - **Description**: This sub-project utilizes the Boost.Asio library for asynchronous I/O operations, allowing the server to handle multiple clients concurrently.
   - **Implementation**: Boost.Asio's asynchronous model is used to manage client connections and requests without blocking.

4. **Thread Pool Based Server**
   - **Description**: This sub-project uses a thread pool to manage a fixed number of threads that handle client requests concurrently.
   - **Implementation**: A pool of worker threads is maintained, and each client request is assigned to an available thread from the pool.

5. **Coroutines Based Server**
   - **Description**: This sub-project leverages C++ coroutines to handle client requests asynchronously, allowing for efficient and lightweight concurrency.
   - **Implementation**: Coroutines are used to manage the asynchronous flow of client requests, ensuring efficient use of system resources.

## Business Logic

- **Train Configuration**: The train has 10 carriages, each with 60 seats.
- **Client Workflow**:
  1. The client requests a list of carriages with vacant seats.
  2. If the list is not empty, the client picks a random carriage and requests a list of vacant seats in that carriage.
  3. The client then randomly selects a seat and attempts to book it.
  4. If the booking fails, the client returns to the initial step.
  5. If all seats are booked, the client shuts down.

## Client and Server Implementation

- **Client**: Written in Python, the client simulates multiple users trying to book seats concurrently. Each client has a unique UUID.
- **Server**: Written in C++, the server manages the train's seat bookings and logs information about booked seats.

## Logging and Verification

- **Logging**: Both the server and clients log information about booked seats.
- **Verification**: A standalone Python tool verifies that the logs from the server and clients match 1:1 after the simulation finishes.

## Running the Project

1. Clone the repository.
2. Build the server and client executables.
3. Run the server.
4. Start multiple client instances.
5. Verify the logs after the simulation completes.

## Contributing

Contributions are welcome Feel free to explore the different sub-projects, suggest improvements, or implement new concurrency techniques.

## License

This project is licensed under the MIT License. See LICENSE for details.

## Acknowledgments

Special thanks to the contributors and maintainers of the Boost.Asio library and other open-source projects used in this simulation.

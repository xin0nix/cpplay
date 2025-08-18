#!/usr/bin/env python3
"""
Rate Limiter Test Script

This script orchestrates testing of a C++ rate limiter by running:
1. An API mock server (listens on 127.0.0.1:8081)
2. Multiple client processes that send requests to the rate limiter (127.0.0.1:4321)

Usage:
    python test_rate_limiter.py --clients 5 --requests 20 --delay 0.5
"""

import multiprocessing
import socket
import time
import sys
import argparse
import signal
import threading
from collections import defaultdict


def api_server():
    """
    Mock API server that sends a response when connected to.
    The C++ rate limiter connects here and forwards responses to clients.
    """
    print("🚀 Starting API server on 127.0.0.1:8081")
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    try:
        server.bind(("127.0.0.1", 8081))
        server.listen(10)

        connection_count = 0
        while True:
            try:
                client_sock, addr = server.accept()
                connection_count += 1

                # Send a response (this gets forwarded to the client by the rate limiter)
                response = f"API Response #{connection_count} from {addr[0]}:{addr[1]}".encode()
                print(
                    f"📡 API: Connection #{connection_count} from {addr} - sending {len(response)} bytes"
                )

                client_sock.send(response)
                client_sock.close()

            except socket.error as e:
                if server.fileno() == -1:  # Socket was closed
                    break
                print(f"❌ API server socket error: {e}")
            except Exception as e:
                print(f"❌ API server error: {e}")

    except KeyboardInterrupt:
        print("🛑 API server interrupted")
    except Exception as e:
        print(f"❌ API server fatal error: {e}")
    finally:
        server.close()
        print("🔴 API server shut down")


def client_worker(client_id, num_requests, delay_between_requests, start_delay):
    """
    Client that sends requests to the rate limiter.
    Rate limiter should either forward to API or return rate limit response.
    """
    results = {
        "success": 0,
        "rate_limited": 0,
        "connection_error": 0,
        "timeout_error": 0,
    }

    # Stagger client starts to avoid thundering herd
    time.sleep(start_delay)

    print(f"🤖 Client {client_id} starting {num_requests} requests")

    for req_num in range(num_requests):
        try:
            sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            sock.settimeout(10.0)  # 10 second timeout

            start_time = time.time()
            sock.connect(("127.0.0.1", 4321))

            # Send a request to the rate limiter
            request_data = f"Client-{client_id}-Request-{req_num}".encode()
            sock.send(request_data)

            # Read response from rate limiter
            response = sock.recv(4096)
            elapsed = time.time() - start_time

            if len(response) == 1 and response[0] == 0:
                results["rate_limited"] += 1
                print(f"🔴 Client {client_id}.{req_num}: RATE LIMITED ({elapsed:.2f}s)")
            elif len(response) > 1:
                results["success"] += 1
                response_preview = response[:50].decode("utf-8", errors="ignore")
                print(
                    f"✅ Client {client_id}.{req_num}: SUCCESS ({elapsed:.2f}s) - '{response_preview}...'"
                )
            else:
                results["connection_error"] += 1
                print(
                    f"❓ Client {client_id}.{req_num}: EMPTY RESPONSE ({elapsed:.2f}s)"
                )

            sock.close()

        except socket.timeout:
            results["timeout_error"] += 1
            print(f"⏰ Client {client_id}.{req_num}: TIMEOUT")
        except Exception as e:
            results["connection_error"] += 1
            print(f"❌ Client {client_id}.{req_num}: ERROR - {e}")

        if delay_between_requests > 0 and req_num < num_requests - 1:
            time.sleep(delay_between_requests)

    print(f"🏁 Client {client_id} finished: {results}")
    return client_id, results


def print_banner():
    print("=" * 60)
    print("🔬 C++ RATE LIMITER TEST SUITE")
    print("=" * 60)
    print("Architecture:")
    print("  [Python Clients] → [C++ Rate Limiter] → [Python API Mock]")
    print("       (port 4321)                          (port 8081)")
    print("=" * 60)


def print_results(all_results, start_time, total_requests):
    duration = time.time() - start_time

    print("\n" + "=" * 60)
    print("📊 FINAL RESULTS")
    print("=" * 60)
    print(f"✅ Successful requests:    {all_results['success']:4d}")
    print(f"🔴 Rate limited requests:  {all_results['rate_limited']:4d}")
    print(f"⏰ Timeout errors:         {all_results['timeout_error']:4d}")
    print(f"❌ Connection errors:      {all_results['connection_error']:4d}")
    print(f"📈 Total requests:         {total_requests:4d}")
    print(f"⏱️  Total duration:         {duration:.2f} seconds")
    print(f"🚀 Request rate:           {total_requests/duration:.1f} req/s")

    successful_and_limited = all_results["success"] + all_results["rate_limited"]
    if successful_and_limited > 0:
        rate_limit_percentage = (
            all_results["rate_limited"] / successful_and_limited
        ) * 100
        print(f"🚫 Rate limit percentage:  {rate_limit_percentage:.1f}%")


def main():
    parser = argparse.ArgumentParser(description="Test C++ Rate Limiter")
    parser.add_argument(
        "--clients",
        type=int,
        default=20,
        help="Number of concurrent client processes (default: 3)",
    )
    parser.add_argument(
        "--requests",
        type=int,
        default=10,
        help="Number of requests per client (default: 10)",
    )
    parser.add_argument(
        "--delay",
        type=float,
        default=1.0,
        help="Delay between requests per client in seconds (default: 0.1)",
    )
    parser.add_argument(
        "--stagger",
        type=float,
        default=0.1,
        help="Delay between starting clients in seconds (default: 0.1)",
    )

    args = parser.parse_args()

    print_banner()
    print(f"🎯 Test configuration:")
    print(f"   Clients: {args.clients}")
    print(f"   Requests per client: {args.requests}")
    print(f"   Delay between requests: {args.delay}s")
    print(f"   Client start stagger: {args.stagger}s")
    print(f"   Total requests: {args.clients * args.requests}")
    print()

    # Start API server process
    print("🚀 Starting API mock server...")
    api_process = multiprocessing.Process(target=api_server)
    api_process.daemon = True  # Dies with parent
    api_process.start()

    time.sleep(1)  # Give API server time to start

    print("🤖 Starting client processes...")
    start_time = time.time()

    try:
        # Start client processes using multiprocessing Pool
        with multiprocessing.Pool(args.clients) as pool:
            # Prepare client arguments
            client_args = [
                (client_id, args.requests, args.delay, client_id * args.stagger)
                for client_id in range(args.clients)
            ]

            # Submit all client jobs
            results = pool.starmap(client_worker, client_args)

            # Aggregate results
            all_results = defaultdict(int)
            for client_id, client_results in results:
                for key, value in client_results.items():
                    all_results[key] += value

            total_requests = args.clients * args.requests
            print_results(all_results, start_time, total_requests)

    except KeyboardInterrupt:
        print("\n🛑 Test interrupted by user")
    except Exception as e:
        print(f"\n❌ Test failed: {e}")
    finally:
        # Clean up API server
        if api_process.is_alive():
            print("🔄 Shutting down API server...")
            api_process.terminate()
            api_process.join(timeout=2)
            if api_process.is_alive():
                print("⚠️  Force killing API server...")
                api_process.kill()


if __name__ == "__main__":
    # Handle Ctrl+C gracefully
    signal.signal(signal.SIGINT, lambda s, f: sys.exit(0))
    main()

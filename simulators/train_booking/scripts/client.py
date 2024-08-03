
import socket
import argparse

def main():
    parser = argparse.ArgumentParser(description='Connect to a server using a specified host and port.')
    parser.add_argument('--host', type=str, default='127.0.0.1', help='The host to connect to.')
    parser.add_argument('--port', type=int, default=8080, help='The port to connect to.')
    args = parser.parse_args()

    HOST = args.host
    PORT = args.port

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        print(f"Connected to {HOST}:{PORT}")

if __name__ == "__main__":
    main()

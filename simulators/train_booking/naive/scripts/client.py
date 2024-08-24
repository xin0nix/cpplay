import socket
import argparse
import string
import random

import ExchangeFormat_pb2

def generate_pseudo_uuid(length=8):
    """
    Generate a random string of printable ASCII characters.
    Good enough for a pet project, uuid is too long.

    Args:
        length (int, optional): The length of the string. Defaults to 8.

    Returns:
        str: A random string of printable ASCII characters.
    """
    printable_chars = string.ascii_letters + string.digits
    random_string = ''.join(random.choice(printable_chars) for _ in range(length))
    return random_string.encode('ascii')

def main():
    parser = argparse.ArgumentParser(
        description="Connect to a server using a specified host and port."
    )
    parser.add_argument(
        "--host", type=str, default="127.0.0.1", help="The host to connect to."
    )
    parser.add_argument(
        "--port", type=int, default=8080, help="The port to connect to."
    )
    args = parser.parse_args()

    HOST = args.host
    PORT = args.port

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        print(f"Connected to {HOST}:{PORT}")
        
        client = ExchangeFormat_pb2.ClientMetaData()
        client.uuid = generate_pseudo_uuid()
        client.correlation_id = generate_pseudo_uuid()

        profile = ExchangeFormat_pb2.ProfileRequest()
        profile.firstName = "joe"
        profile.lastName = "strawinsky"

        req = ExchangeFormat_pb2.Request()
        req.client.CopyFrom(client)
        req.profile.CopyFrom(profile)

        print("Sending request: ", req)
        s.sendall(req.SerializeToString())


if __name__ == "__main__":
    main()

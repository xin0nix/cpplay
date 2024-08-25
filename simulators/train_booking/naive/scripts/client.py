import socket
import argparse
import string
import random

import ExchangeFormat_pb2


def generate_pseudo_uuid(length=8):
    printable_chars = string.ascii_letters + string.digits
    random_string = "".join(random.choice(printable_chars) for _ in range(length))
    return random_string.encode("ascii")


def get_random_name():
    first_names = [
            "Aaron",
            "Adam",
            "Alan",
            "Albert",
            "Alex",
            "Carl",
            "Christopher",
            "Charles",
            "Conrad",
            "Craig",
            "Jasper",
            "James",
            "Jeffrey",
            "John",
            "Jonathan",
            "Joseph",
            "Brian",
            "Burton",
            "Buddy",
            "William",
            "Walter",
            "Warren",
            "Anya",
            "Ariel",
            "Arya",
            "Ash",
            "Ainslie",
            "Audrey",
            "Ava",
            "Badra",
            "Beatrice",
            "Belinda",
            "Reagan",
            "Reese",
            "Riley",
            "Robyn",
            "Rose",
            "Rosie",
            "Rosemary",
            "Roxanne",
            "Ruby",
            "Sadie",
        ]
    last_names = [
            "Smith",
            "Johnson",
            "Williams",
            "Brown",
            "Jones",
            "Miller",
            "Davis",
            "Garcia",
            "Rodriguez",
            "Wilson",
            "Martinez",
            "Taylor",
            "Anderson",
            "Thomas",
            "Hernandez",
            "Moore",
            "Lee",
            "Gonzalez",
            "Harris",
            "Martin",
            "Jackson",
            "Thompson",
        ]
    first_name = random.choice(list(first_names))
    last_name = random.choice(list(last_names))
    return (first_name, last_name)


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
        firstName, lastName = get_random_name()
        profile.firstName = firstName
        profile.lastName = lastName

        req = ExchangeFormat_pb2.Request()
        req.client.CopyFrom(client)
        req.profile.CopyFrom(profile)

        print("Sending request: ", req)
        s.sendall(req.SerializeToString())


if __name__ == "__main__":
    main()

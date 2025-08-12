import socket


def send_message():
    host = "127.0.0.1"  # <<< fixed typo
    port = 8080

    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        message = "Hello from the client!"
        s.connect((host, port))
        s.sendall(message.encode("utf-8"))
        print("Message sent!")
        received = s.recv(4096)
        print(f"Received from server: {received.decode('utf-8')}")


if __name__ == "__main__":
    send_message()

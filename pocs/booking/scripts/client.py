import socket
import argparse
import uuid
import Exchange_pb2
import time
import os
import csv
from typing import Dict
from multiprocessing import Process


class Client:
    def __init__(self, host: int, port: int, log_dir: str):
        self.host = host
        self.port = port
        self.log_dir = log_dir
        self.id = str(uuid.uuid4())
        self.history: Dict[int, int] = dict()

    def serialize(self) -> None:
        log_dir = self.log_dir
        os.makedirs(log_dir, exist_ok=True)
        file_path = os.path.join(log_dir, f"{self.id}_history.csv")
        with open(file_path, "w", newline="") as f:
            writer = csv.writer(f)
            for ticket, latency in self.history.items():
                writer.writerow([ticket, latency])

    # Возвращает True если это был последний запрос ("Done")
    def make_request(self) -> bool:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            start = time.time_ns()
            s.connect((self.host, self.port))
            # print(f"Connected to {self.host}:{self.port}")

            req = Exchange_pb2.Request()
            req.uuid = self.id

            # print("Sending request: ", req)
            s.sendall(req.SerializeToString())

            resp = s.recv(4096)
            response = Exchange_pb2.Response()
            response.ParseFromString(resp)
            print("Билет {t}".format(t=response.ok.ticket_num))
            if response.HasField("done"):
                print("Мест не осталось")
                return True
            elif response.HasField("error"):
                raise Exception(f"Произошла ошибка {response.error.what}")
            else:
                ticket_num = response.ok.ticket_num
                latency = time.time_ns() - start
                self.history[ticket_num] = latency
                return False


def spawn_client(client: Client):
    try:
        done = False
        while not done:
            done = client.make_request()
    except Exception:
        print("Произошла ошибка")
    client.serialize()


def main():
    parser = argparse.ArgumentParser(
        description="Клиент может подключиться к серверу и в цикле слать запросы на бронирование."
    )
    parser.add_argument(
        "--host",
        type=str,
        default="127.0.0.1",
        help="Хост, к которому необходимо подключиться.",
    )
    parser.add_argument("--port", type=int, default=8080, help="Порт для подключения.")
    parser.add_argument(
        "--out-dir",
        type=str,
        default=".temp",
        help="Папка, в которую клиент запишет статистику",
    )
    parser.add_argument(
        "--diversity", type=int, default=4, help="Количество разных клиентов."
    )
    args = parser.parse_args()
    # Оставляю для отладки
    # spawn_client(Client(args.host, args.port, args.out_dir))
    workers = [
        Process(target=spawn_client, args=(Client(args.host, args.port, args.out_dir),))
        for _ in range(args.diversity)
    ]
    for w in workers:
        w.start()
    for w in workers:
        w.join()


if __name__ == "__main__":
    main()

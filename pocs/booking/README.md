## Идея
Место это просто пара число-uuid в памяти. Есть клиенты и сервер. Клиент делает запрос, а сервер ищет свободное место и бронирует его. Если мест нет - возвращает ошибку. Можно провести N экспериментов, увеличивая количество клиентов.

## Варианты реализации серверов

1. ThreadPool
[boost::thread_pool](https://www.boost.org/doc/libs/1_87_0/doc/html/boost_asio/reference/thread_pool.html) thread pool

Описание: Использует пул потоков фиксированного размера для асинхронного выполнения задач.

2. AsynchronousCoroutines
- [boost asio](https://www.boost.org/doc/libs/1_83_0/doc/html/boost_asio.html) через event loop и coroutine-ы

Описание: Реализация через библиотеку Boost.Asio с поддержкой корутин. Этот вариант позволяет писать асинхронный код, который выглядит как синхронный, улучшая читаемость и упрощая обработку событий135.

3. EventDrivenService
- [libevent](https://libevent.org/) (обвязка на 26 плюсах)

Описание: Использует библиотеку libevent для создания событийно-ориентированного сервера. Это решение подходит для высоконагруженных систем, позволяя эффективно обрабатывать большое количество соединений с минимальными затратами ресурсов14.

## Требования
Проверена только на `Ubuntu 22.04`
Основные пакеты
```sh
sudo apt install -y cmake build-essential ninja-build libprotobuf-dev protobuf-compiler libssl-dev
```
Установка clang-19 (есть пресет)
```sh
wget https://apt.llvm.org/llvm.sh
chmod +x llvm.sh
sudo apt update
sudo ./llvm.sh 19
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-19 19
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-19 19
sudo update-alternatives --install /usr/bin/clangd clangd /usr/bin/clangd-19 19
sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clangd-19 19
sudo update-alternatives --install /usr/bin/ld.lld lld /usr/bin/ld.lld-19 19
sudo update-alternatives --install /usr/bin/lldb lldb /usr/bin/lldb-19 19
```
Установка gcc-14
```sh
sudo add-apt-repository ppa:ubuntu-toolchain-r/ppa -y
sudo apt update
sudo apt install g++-14
sudo apt install gcc-14
```

## Как запустить

Чтобы выбрать подход нужно указать его название с помощью `--backend`. На выбор "tpool" и "coro".
Проверять естественно нужно в релизной сборке.
Решение один тред на одно подключение:
```sh
App --port 8080 --address 127.0.0.1 --max-backlog 10 --db-path $PWD/.temp/db.csv --capacity 5000 --delay 50 --threads 2 --backend tpool
```
Решение через корутины (в несколько раз быстрее):
```sh
App --port 8080 --address 127.0.0.1 --max-backlog 10 --db-path $PWD/.temp/db.csv --capacity 5000 --delay 50 --threads 2 --backend coro
```
В соседнем терминале нужно запустить питона (в примере ниже 10 процессов):
```
cd scripts
ptyhon3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
time python client.py --diversity 10 --out-dir=$PWD/../.temp/
```

В моем эксперименте на локальной машине в приере с параметрами выше получилось большое преимущество у корутин:
```sh
real    0m25.499s
user    0m0.640s
sys     0m0.635s
```
Вместо:
```sh
real    2m5.797s
user    0m0.723s
sys     0m0.935s
```
У подхода с отдельным потоком на клиента. То есть корутины отработали в 5 раз быстрее.
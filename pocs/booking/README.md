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
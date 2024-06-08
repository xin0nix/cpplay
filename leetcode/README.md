## Build

Make sure you have standard things like libstdc++, clang etc installed.

Make (or reuse) the build directory
```sh
mkdir -p build && cd build
```

Configure, better use clang and ninja-build
```sh
cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B . ..
```

Compile everything
```sh
ninja all
```

Use this super-script for testing
```sh
for B in $(ls build/bin); do ./build/bin/$B; done | grep '\[  FAILED  \]'
```

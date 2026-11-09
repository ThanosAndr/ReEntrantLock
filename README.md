# ReEntrantLock

A C++14 implementation of a reentrant (recursive) lock with unit tests.

`concurrency::ReEntrantLock` can be locked several times by the thread that
owns it and is released once `unlock()` has been called the same number of
times. It meets the standard *Lockable* requirements (`lock`, `unlock`,
`try_lock`), so it works with `std::lock_guard` and `std::unique_lock`.

> **Note:** This is a learning project, written to explore how a reentrant
> lock works under the hood using a mutex and a condition variable. For
> production code, prefer the standard library's `std::recursive_mutex`.

Tools used: CMake (3.16+), googletest (git submodule).

## Usage
```cpp
#include <iostream>
#include <mutex>
#include <stdexcept>
#include <thread>

#include "ReEntrantLock.hpp"

concurrency::ReEntrantLock lock;
int counter = 0;

void increment() {
    std::lock_guard<concurrency::ReEntrantLock> guard(lock);
    ++counter;
}

void incrementTwice() {
    std::lock_guard<concurrency::ReEntrantLock> guard(lock);
    increment();  // locks again from the same thread: no deadlock
    increment();
}

int main() {
    std::thread t1(incrementTwice);
    std::thread t2(incrementTwice);
    t1.join();
    t2.join();
    std::cout << counter << '\n';  // prints 4

    // try_lock returns immediately instead of waiting.
    if (lock.try_lock()) {
        std::cout << "locked, depth " << lock.get_count() << '\n';  // depth 1
        lock.unlock();
    }

    // Misuse is reported instead of being undefined behaviour.
    try {
        lock.unlock();  // not locked
    } catch (const std::logic_error& e) {
        std::cout << "error: " << e.what() << '\n';  // "Already unlocked"
    }
}
```

To use it from your own CMake project, link against `ReEntrantLockLib`:

```cmake
target_link_libraries(my_app PRIVATE ReEntrantLockLib)
```

## Getting the code
googletest is a git submodule, so clone with:

    git clone --recursive <repo-url>

or, in an existing clone:

    git submodule update --init --recursive

## To build
    bash build.sh            # Release
    bash build.sh Debug      # or any CMAKE_BUILD_TYPE

## To run the application
    bash runApp.sh

## To run the unit tests
    bash runTests.sh

## Checking for data races
    cmake -S . -B build-tsan -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fsanitize=thread"
    cmake --build build-tsan --parallel
    (cd build-tsan && ctest --output-on-failure)

# Interprocess Mutex

---
IPMutex is an interprocess mutex that implements [BasicLockable](https://en.cppreference.com/w/cpp/named_req/BasicLockable) for use with [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard).

## System Requirements
It is tested in wsl-ubuntu.

## How to use
Add the `IPMMutex.h` header to your project.

## How to build example
you need cmake.

```
mkdir build
cd build
cmake ..
cmake --build .
```

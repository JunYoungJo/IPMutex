# Interprocess Mutex

---
IPMutex is an interprocess mutex that implements [BasicLockable](https://en.cppreference.com/w/cpp/named_req/BasicLockable) for use with [std::lock_guard](https://en.cppreference.com/w/cpp/thread/lock_guard).

## System Requirements
It is tested in wsl-ubuntu.

## How to use
Add the `IPMMutex.h` header to your project.

### Creates a mutex using key.
```c++
std::string key = "ipmutex";
ipm::IPMutex mutex{key}; // init using key
```

### Create a mutex using the default constructor.
```c++
ipm::IPMutex mutex{}; // init using the default constructor.
```
When using the default constructor, pid is used as the key.

See [Mutex requirements](https://en.cppreference.com/w/cpp/named_req/Mutex) for why a default constructor is required.


### using std::lock_guard
```c++
{
    std::lock_guard lock{mutex};
    // do something.
}
```

>[!NOTE]
>Resources owned by `ipm::IPMutex` are automatically released by a RAII-style mechanism.

>[!CAUTION]
>Because of the behavior when shm_open() is called with the same key after shm_unlink(), the mutex object created first must be released last.
For more information, see [shm_unlink()](https://pubs.opengroup.org/onlinepubs/9699919799/functions/shm_unlink.html).

## How to build example
you need cmake.

```shell
mkdir build
cd build
cmake ..
cmake --build .
```

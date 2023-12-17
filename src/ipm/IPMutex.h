//
// Created by junyoung on 2023-12-16.
//

#ifndef IPMUTEX_IPMUTEX_H
#define IPMUTEX_IPMUTEX_H

#include <fcntl.h>
#include <pthread.h>
#include <sstream>
#include <string>
#include <sys/mman.h>
#include <thread>
#include <unistd.h>


namespace ipm {
    using Mutex = pthread_mutex_t;

    ///
    /// @brief Interprocess mutex that can be used in std::lock_guard.
    ///
    class IPMutex {
    public:

        ///
        /// \brief Default constructor.
        /// \throw  std::system_error If the construction is unsuccessful.
        ///
        IPMutex() : IPMutex("") {}

        ///
        /// \brief Constructor using key.
        /// \param key Key in shared memory used for mutex.
        /// \throw  std::system_error If the construction is unsuccessful.
        ///
        explicit IPMutex(const std::string &key) {

            if (key.empty()) {
                std::stringstream temporary;
                temporary << getpid();
                _key += temporary.str();
            } else {
                _key += key;
            }

            const size_t size = sizeof(Mutex);
            const mode_t openMode = static_cast<mode_t>(S_IRUSR) | static_cast<mode_t>(S_IWUSR);
            int err = 0;

            if (_descriptor = shm_open(_key.c_str(), O_CREAT | O_EXCL | O_RDWR, openMode); _descriptor < 0) {
                /// If shared memory with the same name as identifier already exists
                if (errno == EEXIST) {
                    /// Opens already existing shared memory
                    _descriptor = shm_open(_key.c_str(), O_RDWR, openMode);
                }
            } else {
                _isOwner = true;
                /// If shared memory is created
                if (ftruncate(_descriptor, static_cast<off_t>(size)) != 0) {
                    /// If ftruncate is failed
                    err = errno;
                    _descriptor = -1;
                }
            }

            if (_descriptor >= 0) {
                /// If shared memory creation did not fail
                if (auto *raw = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, _descriptor, 0); raw != MAP_FAILED) { ///Suppressed MISRA-CPP-5-2-4
                    /// Suppressed MISRA-CPP-5-2-8
                    _mutex = static_cast<pthread_mutex_t *>(raw);
                } else {
                    err = errno;
                    _descriptor = -1;
                }
            }

            if (_isOwner && _descriptor >= 0) {
                pthread_mutexattr_t mutexAttribute;
                (void) pthread_mutexattr_init(&mutexAttribute);
                (void) pthread_mutexattr_setpshared(&mutexAttribute, PTHREAD_PROCESS_SHARED);

                if (auto result = pthread_mutex_init(_mutex, &mutexAttribute); result != 0) {
                    err = errno;
                    _descriptor = -1;
                }
            }

            if (_descriptor < 0) {
                const auto &errorCategory = std::generic_category();
                throw std::system_error(std::error_code(err, errorCategory));
            }
        }

        virtual ~IPMutex() {
            release();
        }

        IPMutex(const IPMutex &other) = delete;

        IPMutex(IPMutex &&other) = delete;

        IPMutex &operator=(const IPMutex &other) = delete;

        IPMutex &operator=(IPMutex &&other) = delete;

        ///
        /// \brief Locks the mutex.
        /// \throw std::system_error When errors occur.
        ///

        void lock() {
            if (auto result = pthread_mutex_lock(_mutex); result != 0) {
                int err = errno;
                const auto &errorCategory = std::system_category();
                throw std::system_error(std::error_code(err, errorCategory));
            }
        }

        ///
        /// \brief Unlocks the mutex.
        /// \throw noexcept
        ///
        void unlock() noexcept {
            (void) pthread_mutex_unlock(_mutex);
        }

        ///
        /// \brief Tries to lock the mutex.
        /// \return Returns true if the mutex is locked. Otherwise returns false.
        ///
        bool try_lock() noexcept {
            return pthread_mutex_trylock(_mutex) == 0;
        }

        ///
        /// \brief Returns whether "this" is the owner of the mutex.
        /// \return Returns true if “this” is the first mutex created with “key”. Otherwise returns false.
        ///
        [[nodiscard]] bool isOwner() const { return _isOwner; }

        ///
        /// \brief Returns key.
        /// \return Key
        ///
        [[nodiscard]] std::string getKey() const {
            return _key;
        }

    private:
        std::string _key = "/";
        int _descriptor = -1;
        bool _isOwner = false;
        Mutex *_mutex = nullptr;

        void release() {
            if (_mutex != nullptr) {
                (void) munmap(_mutex, sizeof(Mutex));
                _mutex = nullptr;
            }
            if (!_key.empty() && _isOwner) {
                (void) shm_unlink(_key.c_str());
            }
        }
    };
} // namespace ipm

#endif //IPMUTEX_IPMUTEX_H

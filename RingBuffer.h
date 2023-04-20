#pragma once
#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <array>
#include <mutex>
#include <memory>
#include <iterator>

namespace rb {
#include "Iterator.h"

    template <typename T, std::size_t N>
    class RingBuffer final {
    public:
        using iterator = Iterator<T>;
        using const_iterator = Iterator<const T>;
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;

       public:
    explicit RingBuffer() : read_(0), write_(0) {}
        template <typename InputIt>
        RingBuffer(InputIt first, InputIt last) : buffer_{ first, last } {}

        reference operator[](size_type pos) {
            return buffer_[pos % N];
        }
        const_reference operator[](size_type pos) const {
            return buffer_[pos % N];
        }

        reference front() {
            return buffer_[read_];
        }
        const_reference front() const {
            return buffer_[read_];
        }
        reference back() {
            return buffer_[write_];
        }
        const_reference back() const {
            return buffer_[write_];
        }

        iterator begin() {
            return iterator(&buffer_[read_], &buffer_.front(), N);
        }
        const_iterator begin() const {
            return const_iterator(&buffer_[read_], &buffer_.front(), N);
        }
        const_iterator cbegin() const {
            return const_iterator(&buffer_[read_], &buffer_.front(), N);
        }

        iterator end() {
            return iterator(&buffer_[write_], &buffer_.front(), N); 
        }
        const_iterator end() const {
            return const_iterator(&buffer_[write_], &buffer_.front(), N);
        }
        const_iterator cend() const {
            return const_iterator(&buffer_[write_], &buffer_.front(), N);
        }

        bool empty() const {
            return buffer_.empty();
        }
        size_type size() const {
            return buffer_.size();
        }
        void clear() {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_.clear();
        }

        void push_back(const value_type& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_[write_] = value;
            write_ = (write_ + 1) % N;
        }
        void push_back(value_type&& value) {
            std::lock_guard<std::mutex> lock(mutex_);
            buffer_[write_] = std::move(value);
            write_ = (write_ + 1) % N;
        }
        void pop_back() {
            std::lock_guard<std::mutex> lock(mutex_);
            read_ = (read_ + 1) % N;
        }

    private:
        std::array<T, N> buffer_{};
        size_type read_, write_;
        std::mutex mutex_;
    };
}

#endif // RINGBUFFER_HPP
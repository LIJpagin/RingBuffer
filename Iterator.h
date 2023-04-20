#ifndef ITERATOR_H
#define ITERATOR_H

#include <iterator>
#include <memory>

template<typename T>
class Iterator {
public:
    template <typename T, std::size_t N>
    friend class RingBuffer;

    Iterator(T* ptr, T* buffer, std::size_t size) : ptr_(ptr), buffer_(buffer), size_(size) {}

    Iterator& operator++() {
        ptr_ = &buffer_[(ptr_ - buffer_ + 1) % size_];
        return *this;
    }
    Iterator operator++(int) {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }
    Iterator& operator+=(std::size_t n) {
        ptr_ = &buffer_[(ptr_ - buffer_ + n) % size_];
        return *this;
    }
    Iterator operator+(std::size_t n) const {
        Iterator result = *this;
        result += n;
        return result;
    }
    Iterator& operator--() {
        ptr_ = &buffer_[(ptr_ - buffer_ - 1) % size_];
        return *this;
    }
    Iterator operator--(int) {
        Iterator tmp = *this;
        --(*this);
        return tmp;
    }
    Iterator& operator-=(std::size_t n) {
        ptr_ = &buffer_[(ptr_ - buffer_ - n + size_) % size_];
        return *this;
    }
    Iterator operator-(std::size_t n) const {
        Iterator result = *this;
        result -= n;
        return result;
    }
    std::size_t operator-(const Iterator& other) const {
        std::ptrdiff_t diff = ptr_ - other.ptr_;
        if (diff < 0)
            diff += size_;
        return static_cast<std::size_t>(diff);
    }

    T& operator[](std::size_t n) const {
        return *(*this + n);
    }
    bool operator<(const Iterator& other) const {
        return (*this - other) < size_;
    }
    bool operator>(const Iterator& other) const {
        return other < *this;
    }
    bool operator<=(const Iterator& other) const {
        return !(*this > other);
    }
    bool operator>=(const Iterator& other) const {
        return !(*this < other);
    }
    bool operator==(const Iterator& other) const {
        return ptr_ == other.ptr_;
    }
    bool operator!=(const Iterator& other) const {
        return !(*this == other);
    }
    T& operator*() const {
        return *ptr_;
    }
    T* operator->() const {
        return ptr_;
    }

private:
    T* ptr_;
    T* buffer_;
    std::size_t size_;
};

#endif // ITERATOR_H
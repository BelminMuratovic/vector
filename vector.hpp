#pragma once

#include <algorithm>
#include <iostream>
#include <stdexcept>

template <typename T>
class vector
{
  size_t capacity_;
  size_t size_;
  T *arr_;

public:
  vector(size_t n = 10)
  {
    capacity_ = n;
    size_ = 0;
    arr_ = new T[capacity_];
  }
  vector(const std::initializer_list<T> &list)
  {
    capacity_ = list.size();
    size_ = list.size();
    arr_ = new T[capacity_];
    std::copy(list.begin(), list.end(), arr_);
  }
  vector(const vector &second)
  {
    capacity_ = second.capacity_;
    size_ = second.size_;
    arr_ = new T[capacity_];
    std::copy(second.arr_, second.arr_ + size_, arr_);
  }
  vector &operator=(const vector &second)
  {
    if (&second == this)
      return *this;
    delete[] arr_;
    capacity_ = second.capacity_;
    size_ = second.size_;
    arr_ = new T[capacity_];
    std::copy(second.arr_, second.arr_ + size_, arr_);
    return *this;
  }
  vector(vector &&second)
  {
    capacity_ = second.capacity_;
    size_ = second.size_;
    arr_ = second.arr_;
    second.arr_ = nullptr;
    second.capacity_ = 0;
    second.size_ = 0;
  }
  vector &operator=(vector &&second)
  {
    if (&second == this)
      return *this;
    delete[] arr_;
    capacity_ = second.capacity_;
    size_ = second.size_;
    arr_ = second.arr_;
    second.arr_ = nullptr;
    second.capacity_ = 0;
    second.size_ = 0;
    return *this;
  }
  ~vector() { delete[] arr_; }

  vector &push_back(T n)
  {
    if (size_ == capacity_)
    {
      capacity_ *= 2;
      T *ptr = new T[capacity_];
      std::copy(arr_, arr_ + size_, ptr);
      delete[] arr_;
      arr_ = ptr;
      ptr = nullptr;
    }
    arr_[size_++] = n;
    return *this;
  }

  T &at(size_t n) const
  {
    if (n >= size_)
      throw std::out_of_range("Indeksiranje van granica!");
    return arr_[n];
  }
  T &operator[](size_t n) const { return arr_[n]; }
  void clear() { size_ = 0; }
  size_t size() const { return size_; }
  bool empty() const { return size_ == 0; }
  const T &back() const { return arr_[size_ - 1]; }
  const T &front() const { return arr_[0]; }
  T &back() { return arr_[size_ - 1]; }
  T &front() { return arr_[0]; }

  void resize(size_t n)
  {
    if (n > capacity_)
    {
      capacity_ *= 2;
      T *ptr = new T[capacity_];
      std::copy(arr_, arr_ + size_, ptr);
      delete[] arr_;
      arr_ = ptr;
      ptr = nullptr;
    }
    for (int i = size_; i < n; ++i)
    {
      arr_[i] = T{};
    }
    size_ = n;
  }

  vector &pop_back()
  {
    --size_;
    return *this;
  }
  size_t capacity() const { return capacity_; }
  bool operator==(const vector &second) const
  {
    if (size_ != second.size_)
      return false;
    for (int i = 0; i < size_; ++i)
    {
      if (arr_[i] != second.arr_[i])
        return false;
    }
    return true;
  }
  bool operator!=(const vector &second) const
  {
    if (size_ != second.size_)
      return true;
    for (int i = 0; i < size_; ++i)
    {
      if (arr_[i] != second.arr_[i])
        return true;
    }
    return false;
  }

  class iterator;
  iterator begin() const { return arr_; }
  iterator end() const { return arr_ + size_; }
  iterator find(const T &element) const
  {
    for (int i = 0; i < size_; ++i)
    {
      if (arr_[i] == element)
        return arr_ + i;
    }
    return end();
  }
  vector &insert(iterator i, T element)
  {
    if (size_ == capacity_)
    {
      capacity_ *= 2;
      T *ptr = new T[capacity_];
      std::copy(arr_, arr_ + size_, ptr);
      size_t n = i - arr_;
      delete[] arr_;
      arr_ = ptr;
      ptr = nullptr;
      i = typename vector<T>::iterator(begin() + n);
    }
    for (auto it = end() - 1; it != i - 1; --it)
    {
      *(it + 1) = *it;
    }
    *i = element;
    ++size_;
    return *this;
  }
  iterator erase(iterator pos)
  {
    for (auto i = pos; i != end(); ++i)
    {
      *i = *(i + 1);
    }
    --size_;
    return pos;
  }
  iterator erase(iterator beginIt, iterator endIt)
  {
    size_t n = endIt - beginIt;
    for (auto i = beginIt; i != endIt; ++i)
    {
      *i = *(i + n);
    }
    size_ -= n;
    return beginIt;
  }

  template <typename P>
  friend std::ostream &operator<<(std::ostream &out, const vector<P> &v);
};

template <typename T>
std::ostream &operator<<(std::ostream &out, const vector<T> &v)
{
  for (int i = 0; i < v.size(); ++i)
  {
    out << v.arr_[i] << " ";
  }
  return out << std::endl;
}

template <typename T>
class vector<T>::iterator
    : public std::iterator<std::random_access_iterator_tag, T>
{
  T *ptr_ = nullptr;

public:
  iterator(T *ptr) : ptr_{ptr} {}
  T &operator*() const { return *ptr_; }
  T *operator->() const { return ptr_; }
  iterator &operator++()
  {
    ++ptr_;
    return *this;
  }
  iterator operator++(int) { return ptr_++; }
  iterator &operator--()
  {
    --ptr_;
    return *this;
  }
  iterator operator--(int) { return ptr_--; }
  iterator operator+(size_t index) { return ptr_ + index; }
  iterator operator-(size_t index) { return ptr_ - index; };
  size_t operator-(iterator other) { return ptr_ - other.ptr_; };

  bool operator==(const iterator &it) const { return ptr_ == it.ptr_; }
  bool operator!=(const iterator &it) const { return ptr_ != it.ptr_; }
  bool operator<(const iterator &it) const { return ptr_ < it.ptr_; };

  ~iterator() {}
};

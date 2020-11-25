// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_SHAREDPTR_HPP_
#define INCLUDE_SHAREDPTR_HPP_

#include <atomic>
#include <cstddef>
#include <iostream>
#include <utility>

using std::atomic_uint;

template <typename T>
class SharedPtr {
  T* ptr;
  atomic_uint* refs;

 public:
  SharedPtr();
  explicit SharedPtr(T* ptr_);
  SharedPtr(const SharedPtr& r);
  SharedPtr(SharedPtr&& r) noexcept;
  ~SharedPtr();
  auto operator=(const SharedPtr& r) -> SharedPtr&;
  auto operator=(SharedPtr&& r) noexcept -> SharedPtr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr_);
  void swap(SharedPtr& r);
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же
  // управляемый объект
  [[nodiscard]] auto use_count() const -> size_t;
};

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr_) : ptr(ptr_) {
  refs = new atomic_uint{1};
}

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr) {
  refs = new atomic_uint{1};
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& r) : ptr(r.ptr), refs(r.refs) {
  (*refs)++;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& r) noexcept : ptr(r.ptr), refs(r.refs) {
  r.refs = nullptr;
  r.ptr = nullptr;
}

template <typename T>
SharedPtr<T>::~SharedPtr<T>() {
  if (refs != nullptr) {
    if (--(*refs) == 0) {
      delete refs;
      delete ptr;
    }
  }
}

template <typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr& {
  if (ptr != nullptr) {
    delete ptr;
  }
  refs = r.refs;
  if (r.ptr != ptr) {
    (*refs)++;
    ptr = r.ptr;
  }
  return *this;
}

template <typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r) noexcept -> SharedPtr& {
  if (r != this) {
    delete refs;
    if (ptr != nullptr) {
      delete ptr;
    }
    refs = r.refs;
    ptr = r.ptr;
    r.refs = nullptr;
    r.ptr = nullptr;
  }
  return *this;
}

template <typename T>
auto SharedPtr<T>::operator*() const -> T& {
  return *ptr;
}

template <typename T>
auto SharedPtr<T>::operator->() const -> T* {
  return ptr;
}

template <typename T>
auto SharedPtr<T>::use_count() const -> size_t {
  return *refs;
}

template <typename T>
auto SharedPtr<T>::get() -> T* {
  return ptr;
}

template <typename T>
void SharedPtr<T>::reset() {
  if (--(*refs) == 0) {
    delete ptr;
  }
  refs = nullptr;
  ptr = nullptr;
}

template <typename T>
void SharedPtr<T>::swap(SharedPtr& r) {
  std::swap(refs, r.refs);
  std::swap(ptr, r.ptr);
}

template <typename T>
void SharedPtr<T>::reset(T* ptr_) {
  if (--(*refs) == 0) {
    delete refs;
    delete ptr;
  }
  refs = new atomic_uint{1};
  ptr = ptr_;
}

template <typename T>
SharedPtr<T>::operator bool() const {
  return ptr != nullptr;
}

#endif  // INCLUDE_SHAREDPTR_HPP_

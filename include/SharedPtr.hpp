// Copyright 2020 Your Name <your_email>

#ifndef INCLUDE_HEADER_HPP_
#define INCLUDE_HEADER_HPP_

#include <atomic>
#include <iostream>
#include <cstddef>

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
  auto operator=(SharedPtr&& r)  noexcept -> SharedPtr&;

  // проверяет, указывает ли указатель на объект
  operator bool() const;
  auto operator*() const -> T&;
  auto operator->() const -> T*;

  auto get() -> T*;
  void reset();
  void reset(T* ptr_);
  void swap(SharedPtr& r);
  // возвращает количество объектов SharedPtr, которые ссылаются на тот же управляемый объект
  [[nodiscard]] auto use_count() const -> size_t;
};

#endif // INCLUDE_HEADER_HPP_

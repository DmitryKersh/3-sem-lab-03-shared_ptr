#include "SharedPtr.hpp"

template <typename T>
SharedPtr<T>::SharedPtr(T* ptr_) : ptr(ptr_){
  refs = new atomic_uint{1};
}

template <typename T>
SharedPtr<T>::SharedPtr() : ptr(nullptr) {
  refs = new atomic_uint{1};
}

template <typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& r) : ptr(r.ptr), refs(r.refs){
  *refs++;
}

template <typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& r) noexcept : ptr(r.ptr), refs(r.refs){
  r.refs = nullptr;
  r.ptr = nullptr;
}

template <typename T>
SharedPtr<T>::~SharedPtr<T>() {
  if (--(*refs) == 0){
    delete refs;
    delete ptr;
  }
}

template <typename T>
auto SharedPtr<T>::operator=(const SharedPtr& r) -> SharedPtr& {
  if (ptr != nullptr){
    delete ptr;
  }
  refs = r.refs;
  if (r != this) {
    (*refs)++;
    ptr = r.ptr;
  }
  return *this;
}

template <typename T>
auto SharedPtr<T>::operator=(SharedPtr&& r)  noexcept -> SharedPtr& {
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
  return ptr;
}

template <typename T>
auto SharedPtr<T>::operator->() const -> T* {
  return *ptr;
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
  if (--(*refs) == 0){
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
  if (--(*refs) == 0){
    delete refs;
    delete ptr;
  }
  refs = new atomic_uint {1};
  ptr = ptr_;
}

template <typename T>
SharedPtr<T>::operator bool() const {
  return ptr == nullptr;
}

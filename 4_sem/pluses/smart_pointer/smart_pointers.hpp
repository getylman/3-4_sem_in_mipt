#include <cstdint>
#include <memory>
#include <iostream>

template <typename TempT>
class WeakPtr;

template <typename TempT>
class SharedPtr {
public:
  //==============Constructors=================
  constexpr SharedPtr() noexcept;
  template <typename TempY>
  explicit SharedPtr(TempY* ptr);
  SharedPtr(const SharedPtr& sptr) noexcept;
  SharedPtr(SharedPtr&& sptr) noexcept;
  template <typename TempY, class Deleter>
  SharedPtr(TempY* ptr, Deleter del);
  template <typename TempY, class Deleter, class Alloc>
  SharedPtr(TempY* ptr, Deleter del, Alloc alloc);
  ~SharedPtr() noexcept;
  SharedPtr& operator=(const SharedPtr& sptr) noexcept;
  SharedPtr& operator=(SharedPtr&& sptr) noexcept;
  //===========================================
  //================Observers==================
  uint64_t use_count() const noexcept;
  TempT* get() const noexcept;
  TempT* operator*() const noexcept;
  TempT& operator->() const noexcept;
  //===========================================
  //================Modifiers==================
  void reset() noexcept;
  //===========================================
  friend class WeakPtr<TempT>;
private:
  //=================Fields====================
  //===========================================
};
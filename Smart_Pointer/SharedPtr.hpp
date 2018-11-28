#include <iostream>
#include <mutex>
#include <utility>

namespace cs540 {

  class RefCountBase {
    int count;
    std::mutex r_mtx;
  public:
    RefCountBase() : count{1} {}
    virtual ~RefCountBase() { // Virtual is necessary to ensure that derived destructors are called first

    }
    void increment_count() {
      std::unique_lock<std::mutex> lock(r_mtx);
      count++;
    }
    void decrement_count() {
      std::unique_lock<std::mutex> lock(r_mtx);
      --count;
      if(count == 0) {lock.unlock(); delete this;} // Delete this ref_count if reference count is zero. Owned object will be deleted in the destructor
    }
    int get_count() {
      return count;
    }
  };

  template<typename T>
  class RefCount : public RefCountBase {
    T* s_ptr;
  public:
    RefCount() : RefCountBase{}, s_ptr{nullptr} {}
    RefCount(T* p) : RefCountBase{}, s_ptr{p} {}
    virtual ~RefCount() {
      delete s_ptr; // Delete the owned object if this ref_count is destroyed
    }
  };

  template<typename T>
  class SharedPtr {
    RefCountBase* ref_count;
    T* s_ptr;

  template <typename U>
  friend class SharedPtr;
  public:

    void increment_count() {
      if(s_ptr != nullptr && ref_count != nullptr) {
        ref_count->increment_count();
      }
    }

    void decrement_count() {
      if(s_ptr != nullptr && ref_count != nullptr) {
        if(ref_count->get_count() == 1) {
          ref_count->decrement_count();
          s_ptr = nullptr;
          ref_count = nullptr;
        } else ref_count->decrement_count();
      }
    }

// --------------------- Constructors, Assignment Operators and Destructors -------------------------


    // Default Constructor
    SharedPtr() : ref_count{nullptr}, s_ptr{nullptr} {}

    // Explicit Constructor to create a smart pointer that points to U*
    template<typename U>
    SharedPtr(U *ptr) : s_ptr{ptr}, ref_count{new RefCount<U>(ptr)} {}

    // Copy Constructor that copies from SharedPtr<T> to SharedPtr<T>
    SharedPtr(const SharedPtr &p) : s_ptr{p.s_ptr}, ref_count{p.ref_count} {
      if(p!=nullptr) increment_count();
    }

    // Copy Constructor that copies from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr(const SharedPtr<U> &p) : s_ptr{p.s_ptr}, ref_count{p.ref_count} {
      if(p!=nullptr) increment_count();
    }

    template<typename U>
    SharedPtr(T* ptr, const SharedPtr<U> &sp) : s_ptr{ptr}, ref_count{sp.ref_count} {
      increment_count();
    }

    // Move Semantics Constructor to move from SharedPtr<T> to SharedPtr<T>
    // No need to increment since pointer is moved
    SharedPtr(SharedPtr &&p) : s_ptr{std::move(p.s_ptr)}, ref_count{std::move(p.ref_count)} {
      p.ref_count = nullptr;
      p.s_ptr = nullptr;
    }

    // Move Sementics Constructor to move from SharedPtr<U> to SharedPtr<T>
    // No need to increment since pointer is moved
    template<typename U>
    SharedPtr(SharedPtr<U> &&p) : s_ptr{std::move(p.s_ptr)}, ref_count{std::move(p.ref_count)} {
      p.ref_count = nullptr;
      p.s_ptr = nullptr;
    }

    // Copy assignment from SharedPtr<T> to SharedPtr<T>
    SharedPtr &operator=(const SharedPtr &p) {
      if(&p == this) return *this;
      decrement_count();
      ref_count = p.ref_count;
      s_ptr = p.s_ptr;
      increment_count();
      return *this;
    }

    // Copy assignment from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr &operator=(const SharedPtr<U> &p) {
      decrement_count();
      ref_count = p.ref_count;
      s_ptr = p.s_ptr;
      increment_count();
      return *this;
    }

    // Copy assignment using move semantics from SharedPtr<T> to SharedPtr<U>
    SharedPtr &operator=(SharedPtr &&p) {
      if(&p == this) return *this;
      decrement_count();
      ref_count = std::move(p.ref_count);
      s_ptr = std::move(p.s_ptr);
      p.ref_count = nullptr;
      p.s_ptr = nullptr;
      return *this;
    }

    // Copy assignment using move semantics from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr &operator=(SharedPtr<U> &&p){
      decrement_count();
      ref_count = std::move(p.ref_count);
      s_ptr = std::move(p.s_ptr);
      p.ref_count = nullptr;
      p.s_ptr = nullptr;
      return *this;
    }

    // Destructor
    ~SharedPtr() {
      decrement_count();
    }

    // -------------------------------  Modifiers -------------------------------
    // Decrement ref count of current obj and reset ptr to null
    void reset() {
      decrement_count();
      ref_count = nullptr;
      s_ptr = nullptr;
    }

    // Replace the owned object with another pointer
    template<typename U>
    void reset(U *p) {
      decrement_count();
      ref_count = new RefCount<U>(p);
      s_ptr = p;
    }

    // ------------------------------- Observers  -------------------------------
    // Returns a pointer to owned Object
    T *get() const {
      return s_ptr;
    }

    // Returns a reference to the owned object
    T &operator*() const {
      return *s_ptr;
    }

    // Returns a pointer to owned Object
    T *operator->() const {
      return s_ptr;
    }

    // Returns true if the SharedPtr is not null
    explicit operator bool() const {
      return s_ptr != nullptr;
    };

  };

  // -------------------------------  Non-member Functions ----------------------

  // Compare two SharedPtr with different (or same) template params
  template<typename T1, typename T2>
  bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2) {
    return p1.get() == p2.get();
  }

  // Next two functions are for comparisons with nullptr
  template<typename T>
  bool operator==(const SharedPtr<T> &p, std::nullptr_t) {
    return p.get() == nullptr;
  }

  template<typename T>
  bool operator==(std::nullptr_t, const SharedPtr<T> &p) {
    return p.get() == nullptr;
  }

  // Compare two SharedPtr with different (or same) template params for inequality
  template<typename T1, typename T2>
  bool operator!=(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2) {
    return p1.get() != p2.get();
  }

  // Next two functions are for inquality check with nullptr
  template<typename T>
  bool operator!=(const SharedPtr<T> &p, std::nullptr_t) {
    return p.get() != nullptr;
  }

  template<typename T>
  bool operator!=(std::nullptr_t, const SharedPtr<T> &p) {
    return p.get() != nullptr;
  }

  // Static pointer cast from SharedPtr of type U to T
  template <typename T, typename U>
  SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
    return SharedPtr<T>(static_cast<T*>(sp.get()), sp);
  }

  // Dynamic cast
  template <typename T, typename U>
  SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
    auto p = dynamic_cast<T*>(sp.get());
    if(p) {
      return SharedPtr<T>(p, sp);
    }
    return SharedPtr<T>();;
  }

}

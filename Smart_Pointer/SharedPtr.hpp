#include <iostream>
#include <utility>

namespace cs540 {
  template<typename T>
  class RefCount {
    int count;
    T* s_ptr;
  public:
    RefCount() : s_ptr(nullptr), count(0) {}
    RefCount(T* p) : s_ptr(p), count(1) {}
    ~RefCount() {
      delete s_ptr;
    }
    RefCount* increment_count() {
      if(s_ptr != nullptr)
        ++count;
      return this;
    }
    RefCount* decrement_count() {
      if(s_ptr != nullptr && count != 0)
        --count;
      if(count == 0) delete this;
      return this;
    }
    int get_count() {
      return count;
    }
    T* get() const {
      return s_ptr;
    }
  };

  template<typename T>
  class SharedPtr {
    RefCount<T>* ref_count;
    template<typename U>
    friend class SharedPtr;
  public:

// --------------------- Constructors, Assignment Operators and Destructors -------------------------


    // Default Constructor
    SharedPtr() : ref_count() {}

    // Explicit Constructor to create a smart pointer that points to U*
    template<typename U>
    explicit SharedPtr(U *ptr) : ref_count(ptr) {};

    // Copy Constructor that copies from SharedPtr<T> to SharedPtr<T>
    SharedPtr(const SharedPtr &p) : ref_count(p.ref_count->increment_count()) {}

    // Copy Constructor that copies from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr(const SharedPtr<U> &p) : ref_count(p.ref_count->increment_count()) {}

    // Move Semantics Constructor to move from SharedPtr<T> to SharedPtr<T>
    // No need to increment since pointer is moved
    SharedPtr(SharedPtr &&p) : ref_count(std::move(p.ref_count)) {}

    // Move Sementics Constructor to move from SharedPtr<U> to SharedPtr<T>
    // No need to increment since pointer is moved
    template<typename U>
    SharedPtr(SharedPtr<U> &&p) : ref_count(std::move(p.ref_count)) {}

    // Copy assignment from SharedPtr<T> to SharedPtr<T>
    SharedPtr &operator=(const SharedPtr &p) {
      if(&p == this) return *this;
      ref_count->decrement_count();
      ref_count = p.ref_count->increment_count();
    }

    // Copy assignment from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr &operator=(const SharedPtr<U> &p) {
      ref_count->decrement_count();
      ref_count = p.ref_count->increment_count();
    }

    // Copy assignment using move semantics from SharedPtr<T> to SharedPtr<U>
    SharedPtr &operator=(SharedPtr &&p) {
      if(&p == this) return *this;
      ref_count->decrement_count();
      ref_count = std::move(p.ref_count);
      p.ref_count = nullptr;
    }

    // Copy assignment using move semantics from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr &operator=(SharedPtr<U> &&p){
      ref_count->decrement_count();
      ref_count = std::move(p.ref_count);
      p.ref_count = nullptr;
    }

    // Destructor
    ~SharedPtr() {
      if(ref_count != nullptr) {
        ref_count->decrement_count();
        if(ref_count->get_count() == 0) delete ref_count;
      }
    }

    // -------------------------------  Modifiers -------------------------------
    // Decrement ref count of current obj and reset ptr to null
    void reset() {
      ref_count->decrement_count();
      ref_count = nullptr;
    }

    // Replace the owned object with another pointer
    template<typename U>
    void reset(U *p) {
      ref_count->decrement_count();
      ref_count = new RefCount<U>(p);
    }

    // ------------------------------- Observers  -------------------------------\
    // Returns a pointer to owned Object
    T *get() const {
      if(ref_count != nullptr) return ref_count->get();
      else return nullptr;
    }

    // Returns a reference to the owned object
    T &operator*() const {
      return *(ref_count->get());
    }

    // Returns a pointer to owned Object
    T *operator->() const {
      if(ref_count != nullptr) return ref_count->get();
      return nullptr;
    }

    // Returns true if the SharedPtr is not null
    explicit operator bool() const {
      if(ref_count != nullptr && ref_count->get() != nullptr) return true;
      else return false;
    };

  };

  // -------------------------------  Non-member Functions ----------------------

  // Compare two RefCounts with different (or same) template params
  // Necessary for == between SharedPtrS
  template<typename T1, typename T2>
  bool operator==(const RefCount<T1> &r1, const RefCount<T2> &r2) {
    return r1.s_ptr == r2.s_ptr;
  }

  // Compare two SharedPtr with different (or same) template params
  template<typename T1, typename T2>
  bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2) {
    return p1 == p2;
  }

  // Next four functions are for comparisons with nullptr
  template<typename T>
  bool operator==(const RefCount<T> &r, std::nullptr_t) {
    return r.s_ptr == nullptr;
  }

  template<typename T>
  bool operator==(std::nullptr_t, const RefCount<T> &r) {
    return r.s_ptr == nullptr;
  }

  template<typename T>
  bool operator==(const SharedPtr<T> &p, std::nullptr_t) {
    return p.ref_count == nullptr;
  }

  template<typename T>
  bool operator==(std::nullptr_t, const SharedPtr<T> &p) {
    return p.ref_count == nullptr;
  }

  // Compare two RefCounts with different (or same) template params for inequality
  // Necessary for != between SharedPtrS
  template<typename T1, typename T2>
  bool operator!=(const RefCount<T1> &r1, const RefCount<T2> &r2) {
    return r1.s_ptr != r2.s_ptr;
  }

  // Compare two SharedPtr with different (or same) template params for inequality
  template<typename T1, typename T2>
  bool operator!=(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2) {
    return p1 != p2;
  }

  // Next four functions are for inquality check with nullptr
  template<typename T>
  bool operator!=(const RefCount<T> &r, std::nullptr_t) {
    return r.s_ptr != nullptr;
  }

  template<typename T>
  bool operator!=(std::nullptr_t, const RefCount<T> &r) {
    return r.s_ptr != nullptr;
  }

  template<typename T>
  bool operator!=(const SharedPtr<T> &p, std::nullptr_t) {
    return p.ref_count != nullptr;
  }

  template<typename T>
  bool operator!=(std::nullptr_t, const SharedPtr<T> &p) {
    return p.ref_count != nullptr;
  }

  // Static pointer cast from SharedPtr of type U to T
  template <typename T, typename U>
  SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp) {
    return SharedPtr<T>(static_cast<T*>(sp.get()));
  }

  // Dynamic cast
  template <typename T, typename U>
  SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp) {
    SharedPtr<T> sp1 = SharedPtr<T>();
    auto p = dynamic_cast<T*>(sp.get());
    if(p) {
      sp1.reset(p);
    }
    return sp1;
  }

}

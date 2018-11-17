namespace cs540 {
  template<typename T>
  class RefCount {
    int count;
    T* s_ptr;
  public:
    RefCount() : s_ptr(NULL), count(0) {}
    RefCount(T* p) : s_ptr(p), count(1) {}
    ~RefCount() {
      delete s_ptr;
    }
    RefCount* increment_count() {
      if(s_ptr != NULL)
        ++count;
      return this;
    }
    RefCount* decrement_count() {
      if(s_ptr != NULL && count != 0)
        --count;
      if(count == 0) delete this;
      return this;
    }
    int get_count() {
      return count;
    }
  }
  template<typename T>
  class SharedPtr {
    RefCount* ref_count;
    template<typename U>
    friend class SharedPtr<U>
    friend class SharedPtr
  public:

// --------------------- Constructors, Assignment Operators and Destructors -------------------------


    // Default Constructor
    SharedPtr() : ref_count() {}

    // Explicit Constructor to create a smart pointer that points to U*
    template<typename U>
    explicit SharedPtr(U *ptr) : ref_count(ptr) {}

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
    SharedPtr &opeartor=(SharedPtr &&p) {
      if(&p == this) return *this;
      ref_count->decrement_count();
      ref_count = std::move(p.ref_count);
      p.ref_count = NULL;
    }

    // Copy assignment using move semantics from SharedPtr<U> to SharedPtr<T>
    template<typename U>
    SharedPtr &operator=(SharedPtr<U> &&p){
      ref_count->decrement_count();
      ref_count = std::move(p.ref_count);
      p.ref_count = NULL;
    }

    // Destructor
    ~SharedPtr() {
      if(ref_count != NULL) {
        ref_count->decrement_count();
        if(ref_count->get_count() == 0) delete ref_count;
      }
    }

    // -------------------------------  Modifiers -------------------------------
    // Decrement ref count of current obj and reset ptr to null
    void reset() {
      ref_count->decrement_count();
      ref_count = NULL;
    }

    // Replace the owned object with another pointer
    template<typename U>
    void reset(U *p) {
      ref_count->decrement_count();
      ref_count = new RefCount(p);
    }

    // ------------------------------- Observers  -------------------------------\
    


  };
}

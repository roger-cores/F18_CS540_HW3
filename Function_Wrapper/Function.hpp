#include <iostream>

namespace cs540 {

  class BadFunctionCall : public std::exception {
  public:
    virtual const char* what() const throw() {
      return "Function Call Was Invalid";
    }
  }

  template<typename ResultType, typename ... ArgumentTypes>
  class CallableAbstract<ResultType(ArgumentTypes...)> {
    virtual ~CallableAbstract() = default;
    virtual ResultType operator()(ArgumentTypes... args) = 0;
    virtual CallableAbstract<ResultType(ArgumentTypes...)>* clone() = 0;
  };

  template <typename FunctionType, typename ResultType, typename ... ArgumentTypes>
  class Callable : public CallableAbstract<ResultType(ArgumentTypes...)> {
  private:
    FunctionType callable;
  public:
    Callable(const FunctionType& callable) : callable(callable) {}

    virtual CallableAbstract<ResultType(ArgumentTypes...)>* clone() override {
      return new Callable<FunctionType, ResultType, ArgumentTypes...>(callable);
    }

    virtual ResultType operator()(ArgumentTypes... args) override {
      return callable(std::forward<ArgumentTypes>(args)...);
    }

    virtual ~Callable() override = default;

  };

  template <typename ResultType, typename ... ArgumentTypes>
  class Function<ResultType(ArgumentTypes...)> {
    CallableAbstract<ResultType(ArgumentTypes...)> *callable;
  public:
    Function() callable(nullptr) {}

    template <typename FunctionType> Function(FunctionType type) {
      callable = new Callable<FunctionType, ResultType, ArgumentTypes...>(type);
    }

    Function(const Function& fn) {
      if(fn.callable != nullptr) {
        callable = fn.callable->clone();
      } else callable = nullptr;
    }

    Function &operator=(const Function& fn) {
      if(&fn == this) return *this; //self assignment

      if(callable != nullptr) {
        delete callable; //delete existing
      }

      if(fn.callable != nullptr) {
        callable = fn.callable->clone();
      } else {
        callable = nullptr;
      }

      return *this;
    }

    ~Function() {
      if(callable != nullptr) {
        delete callable;
        callable = nullptr;
      }
    }

    ResultType operator()(ArgumentTypes ... args) {
      if(callable == nullptr) {
        throw BadFunctionCall();
      }
      return ((*callable_object)(std::forward<ArgumentTypes>(args)...));
    }

    explicit operator bool() const {
      if(callable != nullptr) return true;
      return false;
    }


  };

  template<typename ResultType, typename... ArgumentTypes>
  bool operator==(const Function<ResultType(ArgumentTypes...)> &fn, std::nullptr_t) {
    return !f;
  }

  template<typename ResultType, typename... ArgumentTypes>
  bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &fn) {
    return !f;
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator!=(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
    return bool(f);
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator!=(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f) {
    return bool(f);
  }

}

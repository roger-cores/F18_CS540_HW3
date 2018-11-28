#ifndef CS540_F_HPP
#define CS540_F_HPP

#include <iostream>
#include <exception>

namespace cs540 {

  class BadFunctionCall : public std::exception {
      public:
          virtual const char* what() const throw() {
              return "Function Call Was Invalid";
          }
  };

  template <typename>
  class CallableAbstract;

  template <typename ResultType, typename ... ArgumentTypes>
  class CallableAbstract<ResultType(ArgumentTypes...)> {
      public:
          virtual ~CallableAbstract() = default;
          virtual ResultType operator()(ArgumentTypes... args) = 0;
          virtual CallableAbstract<ResultType(ArgumentTypes...)>* clone() = 0;

  };

  template <typename FunctionType, typename ResultType, typename ... ArgumentTypes>
  class Callable : public CallableAbstract<ResultType(ArgumentTypes...)> {
      private:
          FunctionType callable_t;
      public:
          virtual ResultType operator()(ArgumentTypes... args) override {
              return callable_t(std::forward<ArgumentTypes>(args)...);
          }
          virtual CallableAbstract<ResultType(ArgumentTypes...)>* clone() override {
              return new Callable<FunctionType, ResultType, ArgumentTypes...>(callable_t);
          }
          Callable(const FunctionType& target) : callable_t(target) {}
          virtual ~Callable() override = default;
  };

  template <typename>
  class Function;

  template <typename ResultType, typename ... ArgumentTypes>
  class Function<ResultType(ArgumentTypes...)> {
      private:
          CallableAbstract<ResultType(ArgumentTypes...)> *callable;
      public:
          Function();
          template <typename FunctionType> Function(FunctionType);
          Function(const Function &);
          Function &operator=(const Function &);
          ~Function();
          ResultType operator()(ArgumentTypes...);
          explicit operator bool() const;
  };

  template <typename ResultType, typename ... ArgumentTypes>
  Function<ResultType(ArgumentTypes...)>::Function() :
      callable(nullptr) {}

  template <typename ResultType, typename ... ArgumentTypes>
  template <typename FunctionType>
  Function<ResultType(ArgumentTypes...)>::Function(FunctionType target) {
      callable = new Callable<FunctionType, ResultType, ArgumentTypes...>(target);
  }

  template <typename ResultType, typename ... ArgumentTypes>
  Function<ResultType(ArgumentTypes...)>::Function(const Function &other_ob) {
      if (other_ob.callable != nullptr) {
          callable = other_ob.callable->clone();
          return;
      }
      callable = nullptr;
  }

  template <typename ResultType, typename ... ArgumentTypes>
  Function<ResultType(ArgumentTypes...)>&
  Function<ResultType(ArgumentTypes...)>::operator=(const Function &other_ob){
      if (&other_ob == this) return *this;

      if (callable != nullptr) {
          delete callable;
      }

      if (other_ob.callable != nullptr) {
          callable = other_ob.callable->clone();
      } else callable = nullptr;

      return *this;
  }

  template <typename ResultType, typename ... ArgumentTypes>
  Function<ResultType(ArgumentTypes...)>::~Function(){
      if (callable != nullptr){
          delete callable;
          callable = nullptr;
      }
  }

  template <typename ResultType, typename ... ArgumentTypes>
  ResultType Function<ResultType(ArgumentTypes...)>::operator()(ArgumentTypes... args){
      if (callable == nullptr){
          throw BadFunctionCall();
      }
      return ((*callable)(std::forward<ArgumentTypes>(args)...));
  }

  template <typename ResultType, typename ... ArgumentTypes>
  Function<ResultType(ArgumentTypes...)>::operator bool() const {
    if (callable != nullptr) return true;
    return false;
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator==(const Function<ResultType(ArgumentTypes...)> &f, std::nullptr_t) {
      return !f;
  }

  template <typename ResultType, typename... ArgumentTypes>
  bool operator==(std::nullptr_t, const Function<ResultType(ArgumentTypes...)> &f) {
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

#endif

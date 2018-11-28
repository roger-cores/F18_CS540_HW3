#include <iostream>

namespace cs540 {
  template <typename ResultType, typename ... ArgumentTypes>
  class Function<ResultType(ArgumentTypes ...)> {
  public:
    Function();

    template <typename FunctionType> Function(FunctionType);

    Function(const Function&);

    Function &operator=(const Function&);

    ~Function();

    ResultType operator()(ArgumentTypes ...)

    explicit operator bool() const;
  };
}

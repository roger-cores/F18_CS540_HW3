#ifndef CS540_ARRAY
#define CS540_ARRAY

#include <iostream>

namespace cs540 {
  class Array {
  private:
    MyInt *arr;
  public:
    Array(std::initializer_list<MyInt> list) { //creates from initializer list
      arr = new MyInt[list.size()];
      int i = 0;
      for(auto val : list) {
        //create array from list
        arr[i] = MyInt(val);
        i++;
      }
    }

    // friend std::ostream &operator<<(std::ostream &os, const MyInt &mi) {
    //     //print
    // }
  };
}

#endif

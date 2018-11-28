#ifndef CS540_ARRAY
#define CS540_ARRAY

#include <iostream>
#include "MyInt.hpp"

namespace cs540 {
  class Array {
    MyInt* data;

  public:
    int count;
    Array() : data(nullptr), count(0) {}

    Array(std::initializer_list<MyInt> list) {
      data = new MyInt[list.size()];
      count = 0;
      for(MyInt num : list) {
        data[count] = num;
        count++;
      }
    }

    Array(const Array& arr) {
      data = new MyInt[arr.count];
      count = 0;
      for(int i=0; i<arr.count; i++){
        data[i] = MyInt(arr[i]);
        count++;
      }
    }

    Array(const Array&& arr) {
      data = std::move(arr.data);
      count = std::move(arr.count);
    }

    Array& operator=(const Array &arr) {
      if(&arr == this) return *this;

      if(count!=0) {
        delete[] data;
        data = nullptr;
        count = 0;
      }

      data = new MyInt[arr.count];
      count = 0;
      for (int i=0; i<arr.count; i++) {
        data[i] = MyInt(arr[i]);
        count++;
      }

      return *this;
    }

    Array& operator=(Array &&arr) {
      if(&arr == this) return *this;
      if(count!=0) {
        delete[] data;
        data = nullptr;
        count = 0;
      }

      data = std::move(arr.data);
      count = std::move(arr.count);
      return *this;
    }

    MyInt &operator[] (int index) const {
      return data[index];
    }

  };

  std::ostream& operator<<(std::ostream &os, const Array &arr) {
    for(int i=0; i<arr.count;i++){
      if(i!=arr.count-1) os<<(arr[i])<<", "<<std::flush;
      else os<<(arr[i])<<std::flush;
    }
    return os;
  }
}



#endif

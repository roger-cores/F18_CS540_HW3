#ifndef CS540_ARRAY
#define CS540_ARRAY

#include <iostream>
#include <chrono>
#include "MyInt.hpp"

namespace cs540 {
  class Array {
    MyInt* data;

  public:
    int count;

    ~Array() {
      delete[] data;
      data = nullptr;
    }

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

    Array(Array&& arr) {
      data = std::move(arr.data);
      count = std::move(arr.count);
      arr.data = nullptr;
      arr.count = 0;
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
      arr.data = nullptr;
      arr.count = 0;
      return *this;
    }

    MyInt &operator[] (int index) const {
      return data[index];
    }

    static void move_performance_test() {
      using Milli = std::chrono::duration<double, std::ratio<1,1000>>;
      using TimePoint = std::chrono::time_point<std::chrono::system_clock>;
      using namespace std::chrono;
      TimePoint start, end;
      Array arr1{};
      Array arr2{};
      int max_elems = 10000000;
      arr1.data = new MyInt[max_elems];
      arr2.data = new MyInt[max_elems];
      for(int i=0; i<max_elems; i++){
        arr1.data[i] = i;
        arr2.data[i] = i;
      }

      start = system_clock::now();
      Array copied_arr1{arr1};
      end = system_clock::now();
      Milli copy_time = end - start;

      std::cout << "Copy Constr took " << copy_time.count() << " ms" << std::endl;
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

/*
Copyright (c) 2013 J. Daniel Garcia <josedaniel.garcia@uc3m.es>

Permission is hereby granted, free of charge, to any person obtaining a copy 
of this software and associated documentation files (the "Software"), to deal 
in the Software without restriction, including without limitation the rights 
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell 
copies of the Software, and to permit persons to whom the Software is 
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
 */
#ifndef YAPL_TBB_EXECUTOR_H
#define YAPL_TBB_EXECUTOR_H

#include "cube_index.h"
#include <tbb/tbb.h>
#include <cstddef>

namespace yapl {

class spin_mutex {
public:
  void lock() { m_.lock(); }
  bool try_lock() { return m_.try_lock();  }
  void unlock() { m_.unlock(); }
private:
  tbb::spin_mutex m_;
};

template <class T>
class tbb_executor {
public:
  using mutex_type = spin_mutex;

  template <typename F>
  static void apply(F f, T * b, size_t n);

  template <typename F>
  static void apply(F f, T * b, size_t nx, size_t ny, size_t nz);

  template <typename F>
  static void apply_ordered(F f, T * b, size_t n);

  template <typename F>
  static void apply_ordered(F f, T * b, size_t nx, size_t ny, size_t nz);
};

template <class T>
template <class F>
void tbb_executor<T>::apply(F f, T * b, size_t n)
{
  using namespace tbb;
  parallel_for(blocked_range<size_t>(0,n),
    [f,b](const blocked_range<size_t> & r) {
      for (auto i=r.begin();i!=r.end();++i) {
        f(b[i]);
      }
    }
  );
}

template <class T>
template <class F>
void tbb_executor<T>::apply(F f, T * b, size_t nx, size_t ny, size_t nz)
{
  for (size_t k=0; k!=nz; ++k) {
    for (size_t j=0; j!=ny; ++j) {
      for (size_t i=0; i!=nx; ++i, ++b) {
        f(*b, cube_index{i,j,k});
      }
    }
  }
}

template <class T>
template <class F>
void tbb_executor<T>::apply_ordered(F f, T * b, size_t n)
{
  auto end = b + n;
  for (;b!=end;++b) {
    f(*b);
  }
}

template <class T>
template <class F>
void tbb_executor<T>::apply_ordered(F f, T * b, size_t nx, size_t ny, size_t nz)
{
  for (size_t k=0; k!=nz; ++k) {
    for (size_t j=0; j!=ny; ++j) {
      for (size_t i=0; i!=nx; ++i, ++b) {
        f(*b, cube_index{i,j,k});
      }
    }
  }
}

}

#endif

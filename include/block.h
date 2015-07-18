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
#ifndef YAPL_BLOCK_H
#define YALP_BLOCK_H

#include "cube_index.h"
#include <memory>
#include <iostream>

namespace yapl {

template <class T, class P>
class block {
public:
  block(size_t n);

  block(const block &) = delete;
  block & operator=(const block &) = delete;

  block(block &&);
  block & operator=(block &&);

  T & operator[](size_t i) { return mem_[i]; }
  const T & operator[](size_t i) const { return mem_[i]; }

  template <class F>
  void apply(F f, size_t n);

  template <class F>
  void apply_ordered(F f, size_t n);

  template <class F>
  void apply(F f, size_t n) const;

  template <class F>
  void apply_ordered(F f, size_t n) const;

  template <class F>
  void apply(F f, const cube_index & i);

  template <class F>
  void apply_ordered(F f, const cube_index & i);

  template <class F>
  void apply(F f, const cube_index & i) const;

  template <class F>
  void apply_ordered(F f, const cube_index & i) const;

  template <class F, int I>
  void apply_plane(F f, size_t p, const cube_index & i);

private:
  std::unique_ptr<T[]> mem_;
};

template <class T, class P>
block<T,P>::block(size_t n)
:
mem_{new T[n]{}}
{
}

template <class T, class P>
block<T,P>::block(block && b)
:
mem_{std::move(b.mem_)}
{
}

template <class T, class P>
block<T,P> & block<T,P>::operator=(block && b)
{
  mem_ = std::move(b.mem_);
  return *this;
}

template <class T, class P>
template <class F>
void block<T,P>::apply(F f, size_t n)
{
  P::executor_type::apply(f, mem_.get(), n);
}

template <class T, class P>
template <class F>
void block<T,P>::apply_ordered(F f, size_t n)
{
  P::executor_type::apply_ordered(f, mem_.get(), n);
}

template <class T, class P>
template <class F>
void block<T,P>::apply(F f, size_t n) const
{
  P::executor_type::apply(f, mem_.get(), n);
}

template <class T, class P>
template <class F>
void block<T,P>::apply_ordered(F f, size_t n) const
{
  P::executor_type::apply_ordered(f, mem_.get(), n);
}

template <class T, class P>
template <class F>
void block<T,P>::apply(F f, const cube_index & idx)
{
  P::executor_type::apply(f, mem_.get(), idx.get<0>(), idx.get<1>(), idx.get<2>());
}

template <class T, class P>
template <class F>
void block<T,P>::apply_ordered(F f, const cube_index & idx)
{
  P::executor_type::apply_ordered(f, mem_.get(), idx.get<0>(), idx.get<1>(), idx.get<2>());
}

template <class T, class P>
template <class F>
void block<T,P>::apply(F f, const cube_index & idx) const
{
  P::executor_type::apply(f, mem_.get(), idx.get<0>(), idx.get<1>(), idx.get<2>());
}

template <class T, class P>
template <class F>
void block<T,P>::apply_ordered(F f, const cube_index & idx) const
{
  P::executor_type::apply_ordered(f, mem_.get(), idx.get<0>(), idx.get<1>(), idx.get<2>());
}

template <class T, class P, int I>
struct block_plane_traits;

template <class T, class P>
struct block_plane_traits<T,P,0> {
  template <class F>
  static void apply_plane(F f, T * v, size_t p, const cube_index & idx) {
    auto begin = v + p;
    auto end = begin + idx.volume();
    for (auto i=begin; i!=end; i+= idx.get<0>()) {
      f(*i);
    }
  }
};

template <class T, class P>
struct block_plane_traits<T,P,1> {
  template <class F>
  static void apply_plane(F f, T * v, size_t p, const cube_index & idx) {
    for (size_t z=0; z!=idx.get<2>(); ++z) {
      auto begin = v + idx.get<0>() * (z * idx.get<1>() + p);
      auto end = begin + idx.get<0>();
      for (auto i=begin; i!=end; ++i) {
        f(*i);
      }
    }
  }
};

template <class T, class P>
struct block_plane_traits<T,P,2> {
  template <class F>
  static void apply_plane(F f, T * v, size_t p, const cube_index & idx) {
    auto begin = v + p * idx.get<0>() * idx.get<1>();
    auto end = begin + idx.get<0>() * idx.get<1>();
    for (auto i=begin; i!=end; ++i) {
      f(*i);
    }
  }
};

template <class T, class P>
template <class F, int I>
void block<T,P>::apply_plane(F f, size_t p, const cube_index & i)
{
  block_plane_traits<T,P,I>::apply_plane(f,mem_.get(),p,i);
}

}

#endif

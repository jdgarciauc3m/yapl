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
#ifndef YAPL_CUBE_INDEX_H
#define YAPL_CUBE_INDEX_H

#include <type_traits>
#include <cstddef>
#include <algorithm>
#include <tuple>
#include <iostream>

namespace yapl {

class cube_index {

  template <int I,typename RT>
  using requires_dim = typename std::enable_if<I>=0 && I<3,RT>::type;

public:
  cube_index() = delete;

  cube_index(size_t i, size_t j, size_t k) : index_{i,j,k} {}

  cube_index(const std::tuple<size_t, size_t, size_t> & t) : index_{std::get<0>(t), std::get<1>(t), std::get<2>(t)} {}

  cube_index(const cube_index &) = default;
  cube_index & operator=(const cube_index &) = default;

  cube_index(cube_index &&) = default;
  cube_index & operator=(cube_index &&) = default;

  operator std::tuple<size_t, size_t, size_t>() const { return std::tie(index_[0], index_[1], index_[2]); }
  std::tuple<size_t, size_t, size_t> as_tuple() const { return static_cast<std::tuple<size_t, size_t, size_t>>(*this); }

  bool operator==(const cube_index & i) const { return std::equal(index_, index_+3, i.index_); }
  bool operator<(const cube_index & i) const { 
    return (index_[0]<i.index_[0]) && (index_[1]<i.index_[1]) &&  (index_[2]<i.index_[2]);
  }

  template <int I>
  requires_dim<I,size_t> get() const { return index_[I]; } 

  cube_index box(std::tuple<int,int,int> x) const {
    int i = std::get<0>(x);
    int j = std::get<1>(x);
    int k = std::get<2>(x);
    cube_index r{
      (i<0)?0:static_cast<size_t>(i),
      (j<0)?0:static_cast<size_t>(j),
      (k<0)?0:static_cast<size_t>(k),
    };
    if (r.index_[0] >= index_[0]) { r.index_[0] = index_[0]-1; }
    if (r.index_[1] >= index_[1]) { r.index_[1] = index_[1]-1; }
    if (r.index_[2] >= index_[2]) { r.index_[2] = index_[2]-1; }
    return r;
  }

  size_t volume() const { return index_[0] * index_[1] * index_[2]; }

  cube_index bound_lower(const cube_index & b) const;
  cube_index bound_upper(const cube_index & b) const;
  cube_index bound_upper_unique(const cube_index & b) const;

  friend std::ostream & operator<<(std::ostream & o, const cube_index & i) {
    return o << "( " << i.index_[0] << " , " << i.index_[1] << " , " << i.index_[2] << " )";
  }

private:

  static size_t bound_lower(size_t i, size_t l) { return (i<=l)?l:i-1; }
  static size_t bound_upper(size_t i, size_t l) { return (i>=l)?l:i+1; }

private:
 
  size_t index_[3];
};

inline cube_index cube_index::bound_lower(const cube_index & b) const
{
  return {
    bound_lower(index_[0],b.index_[0]),
    bound_lower(index_[1],b.index_[1]),
    bound_lower(index_[2],b.index_[2]),
  };
}

inline cube_index cube_index::bound_upper(const cube_index & b) const
{
  return {
    bound_upper(index_[0],b.index_[0]),
    bound_upper(index_[1],b.index_[1]),
    bound_upper(index_[2],b.index_[2]),
  };
}

inline cube_index cube_index::bound_upper_unique(const cube_index & b) const
{
  return {
    bound_upper(index_[0],b.index_[0]),
    bound_upper(index_[1],b.index_[1]),
    (index_[2]>=b.index_[2])?b.index_[2]:index_[2]
  };
}


}

#endif

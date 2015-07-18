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
#ifndef YAPL_CUBE_H
#define YAPL_CUBE_H

#include "cube_index.h"
#include "cube_mapping.h"
#include "block.h"
#include <memory>
#include <type_traits>
#include <iostream>
#include <algorithm>

#ifndef NDEBUG
#include <cassert>
#endif

namespace yapl {

template <class T, class P> class cube;

template <class T, class P>
std::ostream & operator<<(std::ostream & os, const cube<T,P> & c);

template <class T, class P>
class cube {
public:
  template <int I,typename RT>
  using requires_dim = typename std::enable_if<I>=0 && I<3,RT>::type;

public:
  cube() = delete;

  cube(size_t nx, size_t ny, size_t nz);
  cube(const cube_index & i);

  // No copy allowed
  cube(const cube &) = delete;
  cube & operator=(const cube &) = delete;
  
  // No move allowed
  cube(cube &&) = delete;
  cube & operator=(cube &&) = delete;

  void swap(cube & c);

  size_t size_x() const { return sizes_.get<0>(); }
  size_t size_y() const { return sizes_.get<1>(); }
  size_t size_z() const { return sizes_.get<2>(); }

  cube_index size() const { return sizes_; }

  template <int I>
  requires_dim<I,size_t> size() const { return sizes_.get<I>(); }

  full_cube_mapping<block<T,P>> all()             { return {&grid_, sizes_}; }
  const_full_cube_mapping<block<T,P>> all() const { return {&grid_, sizes_}; }

  full_cube_ordered_mapping<block<T,P>> all_ordered() { return {&grid_, sizes_}; }
  const_full_cube_ordered_mapping<block<T,P>> all_ordered() const { return {&grid_, sizes_}; }

  template <int I>
  requires_dim<I,plane_cube_mapping<block<T,P>,I>> plane(size_t p) { return {&grid_, p, size<0>(), size<1>(), size<2>()}; }

  template <typename F>
  void for_all_neighbours(size_t i, size_t j, size_t k, F f);

  template <typename F>
  void for_all_neighbours(const cube_index & i, F f);

  template <typename F>
  void for_all_neighbours_unique(size_t i, size_t j, size_t k, F f);

  template <typename F>
  void for_all_neighbours_unique(const cube_index & i, F f);

  T ** fill_neighbours_unique(size_t i, size_t j, size_t k, T ** it);

  T ** fill_neighbours_unique(const cube_index & i, T ** it);

  T & operator()(size_t i, size_t j, size_t k);
  T operator()(size_t i, size_t j, size_t k) const;

  T & operator()(const cube_index & i);
  T operator()(const cube_index & i) const;

  friend std::ostream & operator<< <>(std::ostream & os, const cube & c);

private:
  size_t index(size_t i, size_t j, size_t k) const;
  size_t index(const cube_index & i) const;

private:
  cube_index sizes_;
  size_t nelems_;
  block<T,P> grid_;
};

template <class T, class P>
void swap(cube<T,P> & c1, cube<T,P> & c2)
{
  c1.swap(c2);
}

template <class T, class P>
cube<T,P>::cube(size_t nx, size_t ny, size_t nz)
:
sizes_{nx,ny,nz},
nelems_{nx*ny*nz},
grid_{nelems_}
{
}

template <class T, class P>
cube<T,P>::cube(const cube_index & i)
:
sizes_{i},
nelems_{i.get<0>() * i.get<1>() * i.get<2>()},
grid_{nelems_}
{
}

template <class T, class P>
void cube<T,P>::swap(cube & c)
{
  std::swap(sizes_, c.sizes_);
  std::swap(nelems_, c.nelems_);
  std::swap(grid_, c.grid_);
}

template <class T, class P>
template <typename F>
void cube<T,P>::for_all_neighbours(size_t i, size_t j, size_t k, F f)
{
  size_t xmin = std::max(0, int(i-1));
  size_t ymin = std::max(0, int(j-1));
  size_t zmin = std::max(0, int(k-1));
  size_t xmax = std::min(i+1, size_x());
  size_t ymax = std::min(j+1, size_y());
  size_t zmax = std::min(k+1, size_z());
  for (size_t z=zmin;z<=zmax;++z) {
    for (size_t y=ymin;y<=ymax;++y) {
      for (size_t x=xmin;x<=xmax;x++) {
        if (x==i && y==j && z==k) continue;
        f(grid_[index(x,y,z)]);
      }
    }
  }
}

template <class T, class P>
template <typename F>
void cube<T,P>::for_all_neighbours(const cube_index & i, F f)
{
  cube_index imin = i.bound_lower(cube_index{0,0,0});
  cube_index imax = i.bound_upper(size());
  for (size_t z=imin.get<2>(); z<=imax.get<2>(); ++z) {
    for (size_t y=imin.get<1>(); y<=imax.get<1>(); ++y) {
      for (size_t x=imin.get<0>(); x<=imax.get<0>(); ++x) {
        cube_index current{x,y,z};
        if (current == i) continue;
        f(grid_[index(current)]);
      }
    }
  }
}

template <class T, class P>
template <typename F>
void cube<T,P>::for_all_neighbours_unique(size_t i, size_t j, size_t k, F f)
{                                      
  size_t xmin = std::max(0, int(i-1)); 
  size_t ymin = std::max(0, int(j-1));
  size_t zmin = std::max(0, int(k-1));
  size_t xmax = std::min(i+1, size_x());
  size_t ymax = std::min(j+1, size_y());
  size_t zmax = std::min(k, size_z());
  for (size_t x=xmin;x<=xmax;x++) {
    for (size_t y=ymin;y<=ymax;++y) {
      for (size_t z=zmin;z<=zmax;++z) {
        if (y==j+1 && z==k) continue; 
        if (x==i+1 && y==j && z==k) continue;
        if (x==i && y==j && z==k) continue;
        f(grid_[index(x,y,z)]);
      }
    }
  }
}

template <class T, class P>
template <typename F>
void cube<T,P>::for_all_neighbours_unique(const cube_index & i, F f)
{                                      
  cube_index imin = i.bound_lower(cube_index{0,0,0});
  cube_index imax = i.bound_upper_unique(size());
  for (size_t x=imin.get<0>(); x<=imax.get<0>(); ++x) {
    for (size_t y=imin.get<1>(); y<=imax.get<1>(); ++y) {
      for (size_t z=imin.get<2>(); z<=imax.get<2>(); ++z) {
        if (y==i.get<1>()+1 && z==i.get<2>()) continue; 
        if (x==i.get<0>()+1 && y==i.get<1>() && z==i.get<2>()) continue;
        cube_index current{x,y,z};
        if (current == i) continue;
        f(grid_[index(current)]);
      }
    }
  }
}

template <class T, class P>
T ** cube<T,P>::fill_neighbours_unique(size_t i, size_t j, size_t k, T ** it) {
  size_t xmin = std::max(0, int(i-1)); 
  size_t ymin = std::max(0, int(j-1));
  size_t zmin = std::max(0, int(k-1));
  size_t xmax = std::min(i+1, size_x());
  size_t ymax = std::min(j+1, size_y());
  size_t zmax = std::min(k, size_z());
  for (size_t x=xmin;x<=xmax;x++) {
    for (size_t y=ymin;y<=ymax;++y) {
      for (size_t z=zmin;z<=zmax;++z) {
        if (y==j+1 && z==k) continue; 
        if (x==i+1 && y==j && z==k) continue;
        if (x==i && y==j && z==k) continue;
        *it++ = &grid_[index(x,y,z)];
      }
    }
  }
  return it;
}

template <class T, class P>
T ** cube<T,P>::fill_neighbours_unique(const cube_index & idx, T ** it) {
  cube_index imin = idx.bound_lower(cube_index{0,0,0}); 
  cube_index imax = idx.bound_upper_unique(size()); 
  for (size_t x=imin.get<0>(); x<=imax.get<0>(); x++) {
    for (size_t y=imin.get<1>(); y<=imax.get<1>(); ++y) {
      for (size_t z=imin.get<2>(); z<=imax.get<2>(); ++z) {
        if (y==idx.get<1>()+1 && z==idx.get<2>()) continue; 
        if (x==idx.get<0>()+1 && y==idx.get<1>() && z==idx.get<2>()) continue;

        cube_index current{x,y,z};
        if (current == idx) continue;
        *it++ = &grid_[index(current)];
      }
    }
  }
  return it;
}

template <class T, class P>
T & cube<T,P>::operator()(size_t i, size_t j, size_t k)
{
#ifndef NDEBUG
  assert(i<size_x());
  assert(j<size_y());
  assert(k<size_z());
#endif
  return grid_[index(i,j,k)];
}

template <class T, class P>
T cube<T,P>::operator()(size_t i, size_t j, size_t k) const
{
#ifndef NDEBUG
  assert(i<size_x());
  assert(j<size_y());
  assert(k<size_z());
#endif
  return grid_[index(i,j,k)];
}

template <class T, class P>
T & cube<T,P>::operator()(const cube_index & idx)
{
#ifndef NDEBUG
  assert(idx < sizes_);
#endif
  return grid_[index(idx)];
}

template <class T, class P>
T cube<T,P>::operator()(const cube_index & idx) const
{
#ifndef NDEBUG
  assert(idx < sizes_);
#endif
  return grid_[index(idx)];
}

template <class T, class P>
size_t cube<T,P>::index(size_t i, size_t j, size_t k) const
{
  return i + size_x() * (j + k * size_y());
}

template <class T, class P>
size_t cube<T,P>::index(const cube_index & i) const
{
  return i.get<0>() + size<0>() * (i.get<1>() + i.get<2>() * size<1>());
}

template <class T, class P>
std::ostream & operator<<(std::ostream & os, const cube<T,P> & c)
{
  for (size_t z = 0; z<c.size_z(); ++z) {
    for (size_t y = 0; y<c.size_y(); ++y) {
      for (size_t x = 0; x<c.size_x(); ++x) {
        os << "(" << x << "," << y << "," << z << ")= " << c.grid_[c.index(x,y,z)] << std::endl;
      }
    }
  }
  return os;
}

}

#endif

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
#ifndef YAPL_CUBE_MAPPING_H
#define YAPL_CUBE_MAPPING_H

#include "cube_index.h"
#include <memory>

namespace yapl {

template <class S>
class cube_mapping_base {
public:
  cube_mapping_base(S * ps, size_t x, size_t y, size_t z) : sizes_{x,y,z}, pstruc_{ps} {}
  cube_mapping_base(S * ps, const cube_index & sz) : sizes_{sz}, pstruc_{ps} {}
protected:
  cube_index sizes_;
  S * pstruc_;
};

template <class S>
class full_cube_mapping : public cube_mapping_base<S> {
public:

  // TODO: Change when moved to support for inheriting constructors
  full_cube_mapping(S * ps, size_t nx, size_t ny, size_t nz) : cube_mapping_base<S>{ps,nx,ny,nz} {}
  full_cube_mapping(S * ps, const cube_index & sz) : cube_mapping_base<S>{ps,sz} {}

  template <class F>
  void apply(F f);

  template <class F>
  void apply_indexed(F f);

protected:
  using cube_mapping_base<S>::pstruc_;
  using cube_mapping_base<S>::sizes_;
};

template <class S>
template <class F>
void full_cube_mapping<S>::apply(F f)
{
  pstruc_->apply(f, sizes_.volume());
}

template <class S>
template <class F>
void full_cube_mapping<S>::apply_indexed(F f)
{
  pstruc_->apply(f, sizes_);
}

template <class S>
class full_cube_ordered_mapping : public full_cube_mapping<S> {
public:

  // TODO: Change when moved to support for inheriting constructors
  full_cube_ordered_mapping(S * ps, size_t nx, size_t ny, size_t nz) : full_cube_mapping<S>{ps,nx,ny,nz} {}
  full_cube_ordered_mapping(S * ps, const cube_index & sz) : full_cube_mapping<S>{ps,sz} {}

  template <class F>
  void apply(F f);

  template <class F>
  void apply_indexed(F f);

protected:
  using full_cube_mapping<S>::pstruc_;
  using full_cube_mapping<S>::sizes_;
};

template <class S>
template <class F>
void full_cube_ordered_mapping<S>::apply(F f)
{
  pstruc_->apply_ordered(f, sizes_.volume());
}

template <class S>
template <class F>
void full_cube_ordered_mapping<S>::apply_indexed(F f)
{
  pstruc_->apply_ordered(f, sizes_);
}


template <class S>
class const_cube_mapping_base {
public:
  const_cube_mapping_base(const S * ps, size_t x, size_t y, size_t z) : sizes_{x,y,z}, pstruc_{ps} {}
  const_cube_mapping_base(const S * ps, const cube_index & sz) : sizes_{sz}, pstruc_{ps} {}
protected:
  cube_index sizes_;
  const S * pstruc_;
};

template <class S>
class const_full_cube_mapping : public const_cube_mapping_base<S> {
public:

  // TODO: Change when moved to support for inheriting constructors
  const_full_cube_mapping(const S * ps, size_t nx, size_t ny, size_t nz) : const_cube_mapping_base<S>{ps,nx,ny,nz} {}
  const_full_cube_mapping(const S * ps, const cube_index & sz) : const_cube_mapping_base<S>{ps,sz} {}

  template <class F>
  void apply(F f);

  template <class F>
  void apply_indexed(F f);

protected:
  using const_cube_mapping_base<S>::pstruc_;
  using const_cube_mapping_base<S>::sizes_;
};

template <class S>
template <class F>
void const_full_cube_mapping<S>::apply(F f)
{
  pstruc_->apply(f, sizes_.volume());
}

template <class S>
template <class F>
void const_full_cube_mapping<S>::apply_indexed(F f)
{
  pstruc_->apply(f, sizes_);
}

template <class S, int I>
class plane_cube_mapping : public cube_mapping_base<S> {
public:
  plane_cube_mapping(S * ps, size_t i, size_t nx, size_t ny, size_t nz) :
    cube_mapping_base<S>{ps,nx,ny,nz}, index_{i} {}
  plane_cube_mapping(S * ps, size_t i, const cube_index & sz) :
    cube_mapping_base<S>{ps,sz} {}

  template <class F>
  void apply(F f);

protected:
  size_t index_;
  using cube_mapping_base<S>::pstruc_;
  using cube_mapping_base<S>::sizes_;
};

template <class S, int I>
template <class F>
void plane_cube_mapping<S,I>::apply(F f)
{
  pstruc_->template apply_plane<F,I>(f, index_, sizes_);
}

template <class S>
class const_full_cube_ordered_mapping : public const_full_cube_mapping<S> {
public:

  // TODO: Change when moved to support for inheriting constructors
  const_full_cube_ordered_mapping(const S * ps, size_t nx, size_t ny, size_t nz) : const_full_cube_mapping<S>{ps,nx,ny,nz} {}
  const_full_cube_ordered_mapping(const S * ps, const cube_index & sz) : const_full_cube_mapping<S>{ps,sz} {}

  template <class F>
  void apply(F f);

  template <class F>
  void apply_indexed(F f);

protected:
  using const_full_cube_mapping<S>::pstruc_;
  using const_full_cube_mapping<S>::sizes_;
};

template <class S>
template <class F>
void const_full_cube_ordered_mapping<S>::apply(F f)
{
  pstruc_->apply_ordered(f, sizes_.volume());
}

template <class S>
template <class F>
void const_full_cube_ordered_mapping<S>::apply_indexed(F f)
{
  pstruc_->apply_ordered(f, sizes_);
}


}

#endif

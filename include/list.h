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
#ifndef YAPL_LIST_H
#define YALP_LIST_H

#include "block_list.h"
#include "list_mapping.h"
#include <utility>
#include <vector>
#include <cstddef>
#include <utility>

namespace yapl {

template <class S, class P>
class list {
public:
  using structure_type = S;
  using element_type = typename S::element_type;
  using full_mapping = full_list_mapping<structure_type>;
  using const_full_mapping = const_full_list_mapping<structure_type>;

  list();

  // No copy allowed
  list(const list &) = delete;
  list & operator=(const list &) = delete;

  // No move alloweed
  list(list &&) = delete;
  list & operator=(list &&) = delete;

  size_t size() const { return struc_.size(); }

  const element_type & at(size_t i) const { return struc_.at(i); }

  full_mapping all() { return {&struc_}; }
  const_full_mapping all() const { return {&struc_}; }

  void clear() { struc_.clear(); }

  template <class T>
  void add(const T & x) { struc_.add(x); }

  template <class ... U>
  void add_construct(U && ... u) { struc_.add_construct(std::forward<U>(u)...); }

private:
  structure_type struc_;
};

template <class T, class P>
list<T,P>::list()
:
struc_{}
{
}

}

#endif

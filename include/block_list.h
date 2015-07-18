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
#ifndef YAPL_BLOCK_LIST_H
#define YAPL_BLOCK_LIST_H

#include <cstddef>

#include <vector>

namespace yapl {

template <class T, class P>
class block_list {
public:

  block_list();

  block_list(const block_list &) = delete;
  block_list & operator=(const block_list &) = delete;

  block_list(block_list &&) = delete;
  block_list & operator=(block_list &&) = delete;

  ~block_list();

  void clear();

  size_t size() const { return num_elems_; }

  T & at(size_t i);

  template <class ... U>
  void add(U && ... u);

  template <class F>
  void apply(F f);

  template <class F>
  void apply(F f) const;

  template <class BF, class PF>
  void apply_cartesian_unique(BF bf, PF pf);

private:

  static constexpr size_t CHUNK_SIZE = 16;
  struct chunk {
    std::vector<T> vec_;
    chunk * next_;
    chunk() : vec_{}, next_{nullptr} {}
  };

  chunk * first_;
  chunk * last_;
  size_t num_elems_;
};

template <class T, class P>
constexpr size_t block_list<T,P>::CHUNK_SIZE;

template <class T, class P>
block_list<T,P>::block_list()
:
first_{nullptr},
last_{nullptr},
num_elems_{0}
{
}

template <class T, class P>
block_list<T,P>::~block_list()
{
  clear();
}

template <class T, class P>
void block_list<T,P>::clear()
{
  while (first_ != nullptr) {
    chunk * tmp = first_;
    first_ = first_->next_;
    delete tmp;
  }
  last_ = nullptr;
  num_elems_ = 0;
}

template <class T, class P>
template <class ... U>
void block_list<T,P>::add(U && ... u)
{
  const size_t offset = num_elems_ % CHUNK_SIZE;
  if (offset == 0) {
    chunk * tmp = new chunk;
    if (first_ == nullptr) {
      first_ = tmp;
    }
    else {
      last_->next_ = tmp;
    }
    last_ = tmp;
  }
  //new (&last_->vec_[offset]) T{u...};
  last_->vec_.emplace_back(std::forward<U>(u)...);
  num_elems_++;
}

template <class T, class P>
T & block_list<T,P>::at(size_t i)
{
  const size_t offset = i % CHUNK_SIZE;
  const size_t block_id = i / CHUNK_SIZE;
  chunk * current = first_;
  for (size_t i=0;i!=block_id;++i) {
    current = current->next_;
  }
  return current->vec_[offset];
}

template <class T, class P>
template <class F>
void block_list<T,P>::apply(F f)
{
  for (auto pblock = first_; pblock != nullptr; pblock = pblock->next_) {
    size_t nblock = (pblock == last_)?num_elems_ % CHUNK_SIZE: CHUNK_SIZE;
    auto end = pblock->vec_.data() + nblock;
    for (auto i=pblock->vec_.data(); i!=end;++i) {
      f(*i);
    }
  }
}

template <class T, class P>
template <class F>
void block_list<T,P>::apply(F f) const
{
  for (auto pblock = first_; pblock != nullptr; pblock = pblock->next_) {
    size_t nblock = (pblock == last_)?num_elems_ % CHUNK_SIZE: CHUNK_SIZE;
    auto end = pblock->vec_.data() + nblock;
    for (auto i=pblock->vec_.data(); i!=end;++i) {
      f(*i);
    }
  }
}

template <class T, class P>
template <class BF, class PF>
void block_list<T,P>::apply_cartesian_unique(BF bf, PF pf)
{
  for (auto pblock = first_; pblock != nullptr; pblock=pblock->next_) {
    size_t nblock = (pblock == last_) ? num_elems_ % CHUNK_SIZE: CHUNK_SIZE;
    auto end = pblock->vec_.data() + nblock;
    for (auto i=pblock->vec_.data(); i!=end; ++i) {
      for (chunk * pblock2 = first_; pblock2 != pblock->next_; pblock2=pblock2->next_) {
        size_t nblock2 = (pblock2 == pblock) ? i-pblock->vec_.data() : CHUNK_SIZE;
        auto end2 = pblock2->vec_.data() + nblock2;
        for (auto j=pblock2->vec_.data(); j!=end2; ++j) {
          bf(*i,*j);
        }
      }

      pf(*i);
    }
  }
}

}

#endif

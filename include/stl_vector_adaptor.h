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
#ifndef YAPL_STL_VECTOR_ADAPTOR_H
#define YAPL_STL_VECTOR_ADAPTOR_H

#include <vector>
#include <mutex>

namespace yapl {

template <class T, class P>
class stl_vector_adaptor {
public:
  using element_type = T;
  using policy_type = P;
  using mutex_type = typename P::executor_type::mutex_type;

  stl_vector_adaptor() : vec_{}, mtx_{} {}

  mutex_type & get_mutex() const { return mtx_; }

  size_t size() const { 
    std::lock_guard<mutex_type> lock{mtx_};
    return vec_.size(); 
  }

  const T & at(size_t i) const { 
    std::lock_guard<mutex_type> lock{mtx_};
    return vec_.at(i); 
  }

  void clear() { 
    std::lock_guard<mutex_type> lock{mtx_};
    vec_.clear(); 
    vec_.shrink_to_fit(); 
  }

  void add(const T & x) {
    std::lock_guard<mutex_type> lock{mtx_};
    vec_.push_back(x);
  }

  template <class ... U>
  void add_construct(U && ... u) { 
    std::lock_guard<mutex_type> lock{mtx_};
    vec_.emplace_back(std::forward<U>(u)...); 
  }

  template <typename F>
  void apply(F f) {
    mtx_.lock();
    auto first = vec_.begin();
    auto last = vec_.end();
    mtx_.unlock();
    for (auto i=first; i!=last; ++i) {
      f(*i);
    }
  }

  template <typename F>
  void apply(F f) const {
    mtx_.lock();
    auto first = vec_.begin();
    auto last = vec_.end();
    mtx_.unlock();
    for (auto i=first; i!=last; ++i) {
      f(*i);
    }
  }

  template <typename BF, typename M, typename MF>
  void apply_cartesian_unique(BF bf, M om, MF mf) {
    auto first = vec_.begin();
    auto last = vec_.end();
    for (auto i=first; i!= last; ++i) {
      {
        std::lock_guard<mutex_type> lock{mtx_};
        for (auto j=first; j!=i; ++j) {
          bf(*i,*j);
        }
      }

      {
        om.apply([this,bf,mf,i](typename M::element_type & x) {
          auto em = mf(x);

          //std::lock(mtx_, other_mutex);
          mtx_.lock();
          em.pstruc_->mtx_.lock();
          //other_mutex.lock();
          em.apply([bf,i](element_type & x) {
            bf(*i, x);
          });
          //other_mutex.unlock();
          em.pstruc_->mtx_.unlock();
          mtx_.unlock();
        });
      }
    }
  }

private:
  std::vector<T> vec_;
  mutable mutex_type mtx_;
};

}

#endif

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
#ifndef YAPL_LIST_MAPPING_H
#define YAPL_LIST_MAPPING_H

namespace yapl {

template <class S>
class full_list_mapping {
public:
  using element_type = typename S::element_type;
  using mutex_type = typename S::mutex_type;

  full_list_mapping(S * ps) : pstruc_{ps} {}

  mutex_type & get_mutex() { return pstruc_->get_mutex(); } 

  template <class F>
  void apply(F f) { pstruc_->apply(f); }

  template <class BF, class LM, class MF>
  void apply_cartesian_unique(BF bf, LM lm, MF mf) { pstruc_->apply_cartesian_unique(bf,lm,mf); }

public:
  S * pstruc_;
};

template <class S>
class const_full_list_mapping {
public:
  using element_type = typename S::element_type;
  using mutex_type = typename S::mutex_type;

  const_full_list_mapping(const S * ps) : pstruc_{ps} {}

  mutex_type & get_mutex() { return pstruc_->get_mutex(); } 

  template <class F>
  void apply(F f) { pstruc_->apply(f); }

  template <class BF, class LM, class MF>
  void apply_cartesian_unique(BF bf, LM lm, MF mf) { pstruc_->apply_cartesian_unique(bf,lm,mf); }

private:
  const S * pstruc_;
};

}

#endif

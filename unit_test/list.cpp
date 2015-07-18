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
#include "list.h"
#include "stl_vector_adaptor.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class list_test : public ::testing::Test {
public:
  using list_type = list<stl_vector_adaptor<T, default_policy<T>>, default_policy<T>>;
};

using my_test_types = ::testing::Types<int>;
TYPED_TEST_CASE(list_test, my_test_types);

TYPED_TEST(list_test, creation)
{
  typename TestFixture::list_type l;
  EXPECT_EQ(0, l.size());
}

TYPED_TEST(list_test, add)
{
  typename TestFixture::list_type l;
  for (int i=0; i<50; ++i) {
    l.add(i);
  }
  EXPECT_EQ(50, l.size());
}

TYPED_TEST(list_test, clear)
{
  typename TestFixture::list_type l;
  for (int i=0; i<50; ++i) {
    l.add(i);
  }
  EXPECT_EQ(50, l.size());
  l.clear();
  EXPECT_EQ(0, l.size());
}


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
#include "block.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class block_test : public ::testing::Test {
public:
  using block_type = block<T,default_policy<T>>;
};

typedef ::testing::Types<float, double> my_test_types;
TYPED_TEST_CASE(block_test, my_test_types);

TYPED_TEST(block_test, access)
{
  typename TestFixture::block_type b(10);
  for (int i=0; i<10; ++i) {
    auto x = b[i];
    ASSERT_EQ(TypeParam{},x);
  }
}

TYPED_TEST(block_test, const_access)
{
  typename TestFixture::block_type b(10);
  const typename TestFixture::block_type & c = b;
  for (int i=0; i<10; ++i) {
    auto x = c[i];
    ASSERT_EQ(TypeParam{},x);
  }
}

TYPED_TEST(block_test, init_apply)
{
  typename TestFixture::block_type b(4);
  int n=0;
  b.apply([&n](TypeParam & x) {x=n++; }, 4);
  ASSERT_EQ(0, b[0]);
  ASSERT_EQ(1, b[1]);
  ASSERT_EQ(2, b[2]);
  ASSERT_EQ(3, b[3]);
}

TYPED_TEST(block_test, apply_const)
{
  typename TestFixture::block_type b(4);
  TypeParam n{}; 
  b.apply([&n](TypeParam & x) {x=n++; }, 4);
  const typename TestFixture::block_type & c = b;
  n = TypeParam{};
  c.apply([&n](TypeParam & x) { n+=x; }, 4);
  EXPECT_EQ(6, n);
}

TYPED_TEST(block_test, init_apply_index)
{
  cube_index size{2,3,4};
  typename TestFixture::block_type b(size.volume());
  int n=0;
  b.apply([&n](TypeParam & x, const cube_index &) {x=n++; }, size);
  ASSERT_EQ(1, b[1]);
}

TYPED_TEST(block_test, swap)
{
  typename TestFixture::block_type b1(4);
  typename TestFixture::block_type b2(7);
  b1[0]=1.0;
  b2[0]=-1.0;
  swap(b1,b2);
  ASSERT_EQ(-1.0, b1[0]);
  ASSERT_EQ(1.0, b2[0]);
}

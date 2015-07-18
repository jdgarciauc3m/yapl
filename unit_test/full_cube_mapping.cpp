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
#include "cube_mapping.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class full_cube_mapping_test : public ::testing::Test {
public:
  using block_type = block<T, default_policy<T>>;
};

typedef ::testing::Types<float, double> my_test_types;
TYPED_TEST_CASE(full_cube_mapping_test, my_test_types);

TYPED_TEST(full_cube_mapping_test, apply)
{
  using block_type = typename TestFixture::block_type;
  block_type b(24);
  full_cube_mapping<block_type> all(&b,2,3,4);
  all.apply([](TypeParam& x) { x=2;});
  for (int i=0; i<10; ++i) {
    auto x = b[i];
    ASSERT_EQ(TypeParam{2},x);
  }
}

TYPED_TEST(full_cube_mapping_test, apply_indexed)
{
  using block_type = typename TestFixture::block_type;
  block_type b(24);
  full_cube_mapping<block_type> all(&b,2,3,4);
  all.apply_indexed([](TypeParam& x, const cube_index & i) { 
    x=i.get<0>() + i.get<1>() + i.get<2>();
  });

  all.apply_indexed([](TypeParam& x,  const cube_index & i) { 
    size_t add =i.get<0>() + i.get<1>() + i.get<2>();
    ASSERT_EQ(add, x);
  });

}

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
#include "policy.h"
#include "block.h"
#include "cube_mapping.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class const_full_cube_mapping_test : public ::testing::Test {
public:
  using block_type = block<T, default_policy<T>>;
};

typedef ::testing::Types<float, double> my_test_types;
TYPED_TEST_CASE(const_full_cube_mapping_test, my_test_types);

TYPED_TEST(const_full_cube_mapping_test, apply)
{
  using block_type = typename TestFixture::block_type;
  const block_type b(24);
  const_full_cube_mapping<block_type> all(&b,2,3,4);
  all.apply([](TypeParam& x) { 
    ASSERT_EQ(TypeParam{}, x);
  });
}

TYPED_TEST(const_full_cube_mapping_test, apply_cindex)
{
  using block_type = typename TestFixture::block_type;
  const block_type b(24);
  const_full_cube_mapping<block_type> all(&b,{2,3,4});
  all.apply([](TypeParam& x) { 
    ASSERT_EQ(TypeParam{}, x);
  });
}

TYPED_TEST(const_full_cube_mapping_test, apply_indexed)
{
  using block_type = typename TestFixture::block_type;
  const block_type b(24);
  const_full_cube_mapping<block_type> all(&b,2,3,4);
  all.apply_indexed([](const TypeParam& x, const cube_index & ) { 
    ASSERT_EQ(TypeParam{}, x);
  });
}
TYPED_TEST(const_full_cube_mapping_test, apply_cindex_indexed)
{
  using block_type = typename TestFixture::block_type;
  const block_type b(24);
  const_full_cube_mapping<block_type> all(&b, {2,3,4});
  all.apply_indexed([](const TypeParam& x, const cube_index &) { 
    ASSERT_EQ(TypeParam{}, x);
  });
}

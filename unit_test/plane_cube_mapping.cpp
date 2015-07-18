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
class plane_cube_mapping_test : public ::testing::Test {
public:
  using block_type = block<T, default_policy<T>>;
};

typedef ::testing::Types<float, double> my_test_types;
TYPED_TEST_CASE(plane_cube_mapping_test, my_test_types);

TYPED_TEST(plane_cube_mapping_test, apply_plane0)
{
  using block_type = typename TestFixture::block_type;
  block_type b(24);
  plane_cube_mapping<block_type,0> px0(&b,0,2,3,4);
  plane_cube_mapping<block_type,0> px1(&b,1,2,3,4);
  px0.apply([](TypeParam& x) { x=2;});
  px1.apply([](TypeParam& x) { x=4;});
  for (int i=0; i<24; ++i) {
    auto x = b[i];
    if (i%2==0) {
      ASSERT_EQ(TypeParam{2},x);
    }
    else {
      ASSERT_EQ(TypeParam{4},x);
    }
  }
}

TYPED_TEST(plane_cube_mapping_test, apply_plane1)
{
  using block_type = typename TestFixture::block_type;
  block_type b(24);
  plane_cube_mapping<block_type,1> py0(&b,0,2,3,4);
  plane_cube_mapping<block_type,1> py1(&b,1,2,3,4);
  plane_cube_mapping<block_type,1> py2(&b,2,2,3,4);
  py0.apply([](TypeParam& x) { x=2;});
  py1.apply([](TypeParam& x) { x=4;});
  py2.apply([](TypeParam& x) { x=6;});
  for (int i=0; i<24; ++i) {
    auto x = b[i];
    size_t idx = (i/2)%3;
    if (idx == 0) {
      ASSERT_EQ(TypeParam{2},x);
    }
    else if (idx == 1) {
      ASSERT_EQ(TypeParam{4},x);
    }
    else {
      ASSERT_EQ(TypeParam{6},x);
    }
  }
}

TYPED_TEST(plane_cube_mapping_test, apply_plane2)
{
  using block_type = typename TestFixture::block_type;
  block_type b(24);
  plane_cube_mapping<block_type,2> pz0(&b,0,2,3,4);
  plane_cube_mapping<block_type,2> pz1(&b,1,2,3,4);
  plane_cube_mapping<block_type,2> pz2(&b,2,2,3,4);
  plane_cube_mapping<block_type,2> pz3(&b,3,2,3,4);
  pz0.apply([](TypeParam& x) { x=2;});
  pz1.apply([](TypeParam& x) { x=4;});
  pz2.apply([](TypeParam& x) { x=6;});
  pz3.apply([](TypeParam& x) { x=8;});
  for (int i=0; i<24; ++i) {
    auto x = b[i];
    size_t idx = (i/6);
    if (idx == 0) {
      ASSERT_EQ(TypeParam{2},x);
    }
    else if (idx == 1) {
      ASSERT_EQ(TypeParam{4},x);
    }
    else if (idx == 2) {
      ASSERT_EQ(TypeParam{6},x);
    }
    else {
      ASSERT_EQ(TypeParam{8},x);
    }
  }
}




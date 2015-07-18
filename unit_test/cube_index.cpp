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
#include "cube_index.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

TEST(cube_index_test, default_construct)
{
  cube_index i{3,4,2};
  ASSERT_EQ(3, i.get<0>());
  ASSERT_EQ(4, i.get<1>());
  ASSERT_EQ(2, i.get<2>());
}

TEST(cube_index_test, copy_construct)
{
  cube_index i{3,4,2};
  cube_index j{i};
  ASSERT_EQ(3, i.get<0>());
  ASSERT_EQ(4, i.get<1>());
  ASSERT_EQ(2, i.get<2>());
  ASSERT_EQ(3, j.get<0>());
  ASSERT_EQ(4, j.get<1>());
  ASSERT_EQ(2, j.get<2>());
}

TEST(cube_index_test, copy)
{
  cube_index i{3,4,2};
  cube_index j{0,0,0};
  j=i;
  ASSERT_EQ(3, i.get<0>());
  ASSERT_EQ(4, i.get<1>());
  ASSERT_EQ(2, i.get<2>());
  ASSERT_EQ(3, j.get<0>());
  ASSERT_EQ(4, j.get<1>());
  ASSERT_EQ(2, j.get<2>());
}

TEST(cube_index, create_from_tuple)
{
  cube_index i{std::make_tuple(3,4,2)};
  ASSERT_EQ(3, i.get<0>());
  ASSERT_EQ(4, i.get<1>());
  ASSERT_EQ(2, i.get<2>());
}

TEST(cube_index, convert_to_tuple)
{
  cube_index i{3,4,2};
  std::tuple<size_t,size_t,size_t> t{i};
  ASSERT_EQ(3, get<0>(t));
  ASSERT_EQ(4, get<1>(t));
  ASSERT_EQ(2, get<2>(t));
}

TEST(cube_index, as_tuple)
{
  cube_index i{3,4,2};
  auto t = i.as_tuple();
  ASSERT_EQ(3, get<0>(t));
  ASSERT_EQ(4, get<1>(t));
  ASSERT_EQ(2, get<2>(t));
}

TEST(cube_index_test, free_swap)
{
  cube_index i{3,4,2};
  cube_index j{0,0,0};
  swap(i,j);
  ASSERT_EQ(0, i.get<0>());
  ASSERT_EQ(0, i.get<1>());
  ASSERT_EQ(0, i.get<2>());
  ASSERT_EQ(3, j.get<0>());
  ASSERT_EQ(4, j.get<1>());
  ASSERT_EQ(2, j.get<2>());
}

TEST(cube_index_test, equality_false)
{
  cube_index i{3,4,2};
  cube_index j{0,0,0};
  bool r = (i==j);
  EXPECT_FALSE(r);
}

TEST(cube_index_test, equality_true)
{
  cube_index i{3,4,2};
  cube_index j{3,4,2};
  bool r = (i==j);
  EXPECT_TRUE(r);
}

TEST(cube_index_text, box1) 
{
  cube_index i{3,4,2};
  auto j = i.box(std::make_tuple<int>(-1,4,5));
  EXPECT_EQ(0, j.get<0>());
  EXPECT_EQ(3, j.get<1>());
  EXPECT_EQ(1, j.get<2>());
}

TEST(cube_index_test, bound_lower1)
{
  cube_index i{3,4,2};
  cube_index lw = i.bound_lower({0,0,0});
  EXPECT_EQ((cube_index{2,3,1}), lw);
}

TEST(cube_index_test, bound_lower2)
{
  cube_index i{1,0,1};
  auto lw = i.bound_lower({0,0,0});
  EXPECT_EQ((cube_index{0,0,0}), lw);
}

TEST(cube_index_test, bound_upper1)
{
  cube_index i{3,4,2};
  cube_index lw = i.bound_upper({5,5,5});
  EXPECT_EQ((cube_index{4,5,3}), lw);
}

TEST(cube_index_test, bound_upper2)
{
  cube_index i{4,5,3};
  auto lw = i.bound_upper({5,5,5});
  EXPECT_EQ((cube_index{5,5,4}), lw);
}



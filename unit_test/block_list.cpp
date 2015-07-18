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
#include "block_list.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class block_list_test : public ::testing::Test {
public:
  using block_list_type = block_list<T, default_policy<T>>;
};

using my_test_types = ::testing::Types<int>;
TYPED_TEST_CASE(block_list_test, my_test_types);

TYPED_TEST(block_list_test, creation)
{
  typename TestFixture::block_list_type bl;
  EXPECT_EQ(0, bl.size());
}

TYPED_TEST(block_list_test, add_few)
{
  typename TestFixture::block_list_type bl;
  for (int i=0;i<5;i++) {
    bl.add(i);
  }

  for (int i=0;i<5;i++) {
    EXPECT_EQ(i, bl.at(i));
  }
  
}

TYPED_TEST(block_list_test, add_many)
{
  typename TestFixture::block_list_type bl;
  for (int i=0;i<50;i++) {
    bl.add(i);
  }

  for (int i=0;i<50;i++) {
    EXPECT_EQ(i, bl.at(i));
  }
  EXPECT_EQ(50, bl.size());
}

TYPED_TEST(block_list_test, apply)
{
  typename TestFixture::block_list_type bl;
  for (int i=0;i<50;i++) {
    bl.add(i);
  }

  std::vector<int> v;
  bl.apply([&v](int x) {
    v.push_back(x);
  });

  EXPECT_EQ(50, v.size());
  for (int i=0;i<50;i++) {
    EXPECT_EQ(i, v[i]);
  }

}

TYPED_TEST(block_list_test, apply_cartesian_unique)
{
  typename TestFixture::block_list_type bl;
  for (int i=0;i<50;i++) {
    bl.add(i);
  }

  std::vector<std::pair<int,int>> v;
  std::vector<int> w;
  bl.apply_cartesian_unique(
    [&v](int x, int y) {
      v.push_back(make_pair(x,y));
    },
    [&w](int x) {
      w.push_back(x);
    }
  ); 


  EXPECT_EQ(49*50/2, v.size());

  auto itv = v.begin();
  auto itw = w.begin();
  for (int i=0;i<50; ++i) {
    for (int j=0;j<i; ++j) {
      EXPECT_EQ(make_pair(i,j), *itv++);
    }
    EXPECT_EQ(i, *itw++);
  }
}

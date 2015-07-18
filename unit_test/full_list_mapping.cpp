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
#include "stl_vector_adaptor.h"
#include "list_mapping.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;


template <typename T>
class full_list_mapping_test : public ::testing::Test {
public:
  using list_type = stl_vector_adaptor<T, default_policy<T>>;
  using olist_type = stl_vector_adaptor<list_type*, default_policy<T>>;
};

typedef ::testing::Types<int> my_test_types;
TYPED_TEST_CASE(full_list_mapping_test, my_test_types);

TYPED_TEST(full_list_mapping_test, apply)
{
  using list_type = typename TestFixture::list_type;
  list_type b;
  for (int i=0;i<50; ++i) {
    b.add(i);
  }
  full_list_mapping<list_type> all(&b);
  all.apply([](TypeParam& x) { x=2;});
  for (int i=0; i<10; ++i) {
    auto x = b.at(i);
    ASSERT_EQ(2,x);
  }
}

TYPED_TEST(full_list_mapping_test, apply_cartesian_unique)
{
/*
  using list_type = typename TestFixture::list_type;
  using olist_type = typename TestFixture::olist_type;
  list_type b;
  for (int i=0;i<50; ++i) {
    b.add(i);
  }

  olist_type others;

  full_list_mapping<list_type> all(&b);
  std::vector<std::pair<int,int>> v;
  std::vector<int> w;
  all.apply_cartesian_unique(
    [&v](int x, int y) {
      v.push_back(make_pair(x,y));
    },
    others.all(),
    [](typename olist_type::element_type & ) {
    }
  );

  EXPECT_EQ(49*50/2, v.size());

  auto itv = v.begin();
  auto itw = w.begin();
  for (int i=0;i<50; ++i) {
    for (int j=0;j<i; ++j) {
      EXPECT_EQ(make_pair(i,j), *itv++);
    }
  }
*/
}

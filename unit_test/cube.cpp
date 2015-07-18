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
#include "cube.h"
#include "policy.h"
#include <gtest/gtest.h>

using namespace yapl;
using namespace std;

template <typename T>
class cube_test : public ::testing::Test {
public:
  using cube_type = cube<T, default_policy<T>>;
};

typedef ::testing::Types<float, double> my_test_types;
TYPED_TEST_CASE(cube_test, my_test_types);

TYPED_TEST(cube_test, get_sizes)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,4,5};
  EXPECT_EQ(3, c.size_x());
  EXPECT_EQ(4, c.size_y());
  EXPECT_EQ(5, c.size_z());
  EXPECT_EQ(3, c.template size<0>());
  EXPECT_EQ(4, c.template size<1>());
  EXPECT_EQ(5, c.template size<2>());
}

TYPED_TEST(cube_test, get_sizes_index)
{
  using cube = typename TestFixture::cube_type;
  cube c{cube_index{3,4,5}};
  EXPECT_EQ((cube_index{3,4,5}), c.size());
  EXPECT_EQ(3, c.size_x());
  EXPECT_EQ(4, c.size_y());
  EXPECT_EQ(5, c.size_z());
  EXPECT_EQ(3, c.template size<0>());
  EXPECT_EQ(4, c.template size<1>());
  EXPECT_EQ(5, c.template size<2>());
}

TYPED_TEST(cube_test, member_swap)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,4,5};
  cube d{2,7,3};
  c.swap(d);
  EXPECT_EQ((cube_index{3,4,5}), d.size());
  EXPECT_EQ((cube_index{2,7,3}), c.size());
}

TYPED_TEST(cube_test, free_swap)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,4,5};
  cube d{2,7,3};
  swap(c,d);
  EXPECT_EQ((cube_index{3,4,5}), d.size());
  EXPECT_EQ((cube_index{2,7,3}), c.size());
}

TYPED_TEST(cube_test, fill)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,4,5};
  auto all = c.all();
  all.apply_indexed([](TypeParam & x, const cube_index & i) {
    x = TypeParam(i.get<0>() + i.get<1>() + i.get<2>());
  });

  for (size_t i=0; i<c.size_x(); ++i) {
    for (size_t j=0;j<c.size_y(); ++j) {
       for (size_t k=0;k<c.size_z(); ++k) {
        EXPECT_FLOAT_EQ(i+j+k, c(i,j,k));
      }
    }
  }
}

TYPED_TEST(cube_test, ordered_fill)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,4,5};
  auto all = c.all_ordered();
  all.apply_indexed([](TypeParam & x, const cube_index & i) {
    x = TypeParam(i.get<0>() + i.get<1>() + i.get<2>());
  });

  for (size_t i=0; i<c.size_x(); ++i) {
    for (size_t j=0;j<c.size_y(); ++j) {
       for (size_t k=0;k<c.size_z(); ++k) {
        EXPECT_FLOAT_EQ(i+j+k, c(i,j,k));
      }
    }
  }
}

TYPED_TEST(cube_test, set_xplane_0)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<0>(0).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,1));
}

TYPED_TEST(cube_test, set_xplane_1)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<0>(1).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,1));
}

TYPED_TEST(cube_test, set_xplane_2)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<0>(2).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,1));
}

TYPED_TEST(cube_test, set_yplane_0)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<1>(0).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,1));
}

TYPED_TEST(cube_test, set_yplane_1)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<1>(1).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,1));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,1));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,1));
}

TYPED_TEST(cube_test, set_zplane_0)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<2>(0).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,0));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,0));
}

TYPED_TEST(cube_test, set_zplane_1)
{
  using cube = typename TestFixture::cube_type;
  cube c{3,2,2};
  TypeParam n=10.0;
  c.all().apply([&n](TypeParam & f) { f=n; n+=0.5; });
  c.template plane<2>(1).apply([](TypeParam & f) { f=-1.0; });
  EXPECT_FLOAT_EQ(-1.0f, c(0,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(0,1,1));
  EXPECT_FLOAT_EQ(-1.0f, c(1,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(1,1,1));
  EXPECT_FLOAT_EQ(-1.0f, c(2,0,1));
  EXPECT_FLOAT_EQ(-1.0f, c(2,1,1));
}

TYPED_TEST(cube_test, for_all_neighbours)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  auto f = [](TypeParam & f) { f=-1.0; };
  c.for_all_neighbours(1,2,3, f);
  EXPECT_FLOAT_EQ(-1.0, c(0,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,1,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,2,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(0,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(1,1,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(1,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(-1.0, c(2,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,2,2));
  EXPECT_FLOAT_EQ(-1.0,  c(2,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,2,4));
}

TYPED_TEST(cube_test, for_all_neighbours_indexed)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  auto f = [](TypeParam & f) { f=-1.0; };
  cube_index idx{1,2,3};
  c.for_all_neighbours(idx, f);
  EXPECT_FLOAT_EQ(-1.0, c(0,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,1,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,2,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(0,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(0,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(1,1,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(1,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(1,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(-1.0, c(2,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,2,2));
  EXPECT_FLOAT_EQ(-1.0,  c(2,2,3));
  EXPECT_FLOAT_EQ(-1.0,  c(2,2,4));
}

TYPED_TEST(cube_test, for_all_neighbours_unique)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  auto f = [](TypeParam & f) { f=-1.0; };
  c.for_all_neighbours_unique(1,2,3, f);
  EXPECT_FLOAT_EQ(-1.0, c(0,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,1,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,2,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(0,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(1,1,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(0.0, c(2,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(2,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,2,4));
}

TYPED_TEST(cube_test, for_all_neighbours_unique_indexed)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  auto f = [](TypeParam & f) { f=-1.0; };
  c.for_all_neighbours_unique(cube_index{1,2,3}, f);

  EXPECT_FLOAT_EQ(-1.0, c(0,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,1,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,2,2));
  EXPECT_FLOAT_EQ(-1.0, c(0,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(0,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(0,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(0,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,1,2));
  EXPECT_FLOAT_EQ(-1.0, c(1,1,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,1,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,2,4));

  EXPECT_FLOAT_EQ(-1.0, c(1,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(1,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(1,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(0.0, c(2,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,3,2));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,3,4));

  EXPECT_FLOAT_EQ(-1.0, c(2,2,2));
  EXPECT_FLOAT_EQ(0.0,  c(2,2,3));
  EXPECT_FLOAT_EQ(0.0,  c(2,2,4));
}

TYPED_TEST(cube_test, fill_neighbours_unique)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  TypeParam * n[13];
  auto endp = c.fill_neighbours_unique(1,2,3, n);
  ASSERT_GE(13, std::distance(&n[0],endp));
  EXPECT_NE(endp, std::find(n, endp, &c(0,1,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(0,1,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,1,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(0,2,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(0,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,2,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(0,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,1,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(1,1,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,1,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,2,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,2,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,2,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,2,4)));
}

TYPED_TEST(cube_test, fill_neighbours_unique_indexed)
{
  using cube = typename TestFixture::cube_type;
  cube c{5,7,9};
  TypeParam * n[13];
  auto endp = c.fill_neighbours_unique({1,2,3}, n);
  ASSERT_GE(13, std::distance(&n[0],endp));
  EXPECT_NE(endp, std::find(n, endp, &c(0,1,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(0,1,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,1,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(0,2,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(0,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,2,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(0,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(0,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,1,2)));
  EXPECT_NE(endp, std::find(n, endp, &c(1,1,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,1,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,2,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,2,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(1,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(1,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,3,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,3,4)));

  EXPECT_NE(endp, std::find(n, endp, &c(2,2,2)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,2,3)));
  EXPECT_EQ(endp, std::find(n, endp, &c(2,2,4)));
}



#include "bin_search.hpp"
#include <gtest/gtest.h>
#include <vector>

TEST(CORRECTNESS, JUST_WORKS) {
    int size = 7;
    int arr[] = {1, 2, 3, 4, 5, 6, 7};

    int* begin = arr;
    int* end = arr + size;

    int* ptr = LowerBound(arr, arr + size, 1);

    EXPECT_EQ(arr, begin);
    EXPECT_EQ(arr + size, end);
    EXPECT_EQ(*ptr, 1);
}

TEST(CORRECTNESS, SOME_TESTS) {
    int size = 2;
    int arr[] = {1, 2};

    int* begin = arr;
    int* end = arr + size;

    int* ptr = LowerBound(arr, arr + size, 2);

    EXPECT_EQ(arr, begin);
    EXPECT_EQ(arr + size, end);
    EXPECT_EQ(*ptr, 2);

    int* ptr2 = LowerBound(arr, arr + size, 1);

    EXPECT_EQ(*ptr2, 1);
}

TEST(CORRECTNESS, NO_VALUE) {
    int size = 7;
    int arr[] = {2, 5, 8, 13, 21, 34, 55};

    int* begin = arr;
    int* end = arr + size;

    int* ptr = LowerBound(arr, arr + size, 15);

    EXPECT_EQ(arr, begin);
    EXPECT_EQ(arr + size, end);
    EXPECT_EQ(*ptr, 21);
}

TEST(CORNER, LESS_FIRST) {
    int size = 7;
    int arr[] = {2, 5, 8, 13, 21, 34, 55};

    int* begin = arr;
    int* end = arr + size;

    int* ptr = LowerBound(arr, arr + size, -5);

    EXPECT_EQ(arr, begin);
    EXPECT_EQ(arr + size, end);
    EXPECT_EQ(*ptr, 2);
}

TEST(CORNER, GREATER_ALL) {
    int size = 7;
    int arr[] = {2, 5, 8, 13, 21, 34, 55};

    int* begin = arr;
    int* end = arr + size;

    int* ptr = LowerBound(arr, arr + size, 123);

    EXPECT_EQ(arr, begin);
    EXPECT_EQ(arr + size, end);
    EXPECT_EQ(ptr, end);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
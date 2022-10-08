#include <iostream>
#include <random>

#include "gtest/gtest.h"

TEST(sample_test_case, sample_test)
{
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int>  distr(1, 6);

    EXPECT_NE(distr(generator), 6);
}
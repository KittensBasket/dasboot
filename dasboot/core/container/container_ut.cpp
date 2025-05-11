#include <gtest/gtest.h>

#include <iostream>
#include <iomanip>

#include <dasboot/core/container/container.hpp>

TEST(Containers, DummyTest) {
    EXPECT_EQ(true, true);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

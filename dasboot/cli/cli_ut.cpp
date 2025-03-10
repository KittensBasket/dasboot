#include <gtest/gtest.h>

#include <string>

#include <dasboot/cli/cli.hpp>

TEST(CliUt, NoTest) {
    ASSERT_EQ(0, 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

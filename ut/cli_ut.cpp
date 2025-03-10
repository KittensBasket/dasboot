#include <gtest/gtest.h>

#include <string>

#include <dasboot/cli/cli.hpp>

TEST(CliUt, Square) {
    ASSERT_EQ(NCli::Square(5), 25);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

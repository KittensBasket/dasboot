#include <gtest/gtest.h>

#include <iostream>
#include <iomanip>

#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>

TEST(CruUt, CheckMakeString) {
    std::string message;
    message += "word ";
    message += std::to_string(1);
    message += " 0.755";
    message += "\n";

    std::string str = MakeString() << std::fixed << std::setprecision(3) << "word " << 1 << " " << 0.755 << "\n";
    EXPECT_EQ(message, str);
}

// Write tests for NOs

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

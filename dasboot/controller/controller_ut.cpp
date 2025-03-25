#include <gtest/gtest.h>

#include <dasboot/controller/controller.hpp>

TEST(ControllerUt, BuildContainer) {
    NController::TController controller;
    NController::TBuildSettings buildSettings;
    ASSERT_EQ(controller.Build(buildSettings), false);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

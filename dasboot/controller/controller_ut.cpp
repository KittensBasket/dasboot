#include <gtest/gtest.h>

#include <dasboot/controller/controller.hpp>

TEST(ControllerUt, BuildContainerRequest) {
    NController::TController controller;
    NController::TBuildSettings buildSettings;
    buildSettings.PathToDasbootFile = "path/to/dasbootfile";
    buildSettings.ContainerName = "Test";

    NController::TRequest expectedRequest;
    expectedRequest.set_type("build");
    NController::TArg* arg1 = expectedRequest.add_args();
    arg1->set_name("PathToDasbootFile");
    arg1->set_value(buildSettings.PathToDasbootFile);

    NController::TArg* arg2 = expectedRequest.add_args();
    arg2->set_name("ContainerName");
    arg2->set_value(buildSettings.ContainerName.value());

    NController::TRequest resultRequest = controller.Build(buildSettings);
    EXPECT_EQ(expectedRequest.type(), resultRequest.type());
    EXPECT_EQ(expectedRequest.args(0).name(), resultRequest.args(0).name());
    EXPECT_EQ(expectedRequest.args(0).value(), resultRequest.args(0).value());
    EXPECT_EQ(expectedRequest.args(1).name(), resultRequest.args(1).name());
    EXPECT_EQ(expectedRequest.args(1).value(), resultRequest.args(1).value());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

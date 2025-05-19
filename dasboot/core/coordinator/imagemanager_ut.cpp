#include <gtest/gtest.h>

#include <dasboot/core/coordinator/imagemanager.hpp>

TEST(ImageCoordinator, TestInstall) {
    NImageManager::TImageManager imageManager;
    imageManager.Install(NImageManager::Images::Alpine);
    EXPECT_EQ(true, true);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    return result;
}

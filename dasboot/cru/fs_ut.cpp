#include <gtest/gtest.h>

#include <iostream>
#include <iomanip>

#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>

TEST(CruUt, CreateSimpleFile) {
    std::string path = "testfile.txt";
    NCommon::TStatus status = NOs::CreateFile(path);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsFileExists(path));
    NOs::RemoveFile(path);
    EXPECT_TRUE(!NOs::IsFileExists(path));
}

TEST(CruUt, CreateDeepFileError) {
    std::string path = "tmp/testfile.txt";
    NCommon::TStatus status = NOs::CreateFile(path);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Failed);
    EXPECT_EQ(status.Error, "Path 'tmp' doesnt exists");
    EXPECT_TRUE(!NOs::IsFileExists(path));
    EXPECT_TRUE(!NOs::IsDirectoryExists("tmp"));
}

TEST(CruUt, CreateDeepFile) {
    std::string path = "tmp/testfile.txt";
    NCommon::TStatus status = NOs::CreateFile(path, true, 0755);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsFileExists(path));
    NOs::RemoveFile(path);
    EXPECT_TRUE(!NOs::IsFileExists(path));
    NOs::RemoveDirectory("tmp");
    EXPECT_TRUE(!NOs::IsDirectoryExists("tmp"));
}

TEST(CruUt, CreateSimpleDirectory) {
    std::string path = "tmp";
    NCommon::TStatus status = NOs::CreateDirectory(path);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsDirectoryExists(path));
    NOs::RemoveDirectory(path);
    EXPECT_TRUE(!NOs::IsDirectoryExists(path));
}

TEST(CruUt, CreateDeepDirectoryError) {
    std::string path = "tmp/testdir";
    NCommon::TStatus status = NOs::CreateDirectory(path);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Failed);
    EXPECT_EQ(status.Error, "Path 'tmp' doesnt exists");
    EXPECT_TRUE(!NOs::IsDirectoryExists(path));
    NOs::RemoveDirectory(path);
    EXPECT_TRUE(!NOs::IsDirectoryExists(path));
}

TEST(CruUt, CreateDeepDirectory) {
    std::string path = "tmp/testdir";
    NCommon::TStatus status = NOs::CreateDirectory(path, true);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsDirectoryExists(path));
    NOs::RemoveDirectory(path);
    NOs::RemoveDirectory("tmp");
    EXPECT_TRUE(!NOs::IsDirectoryExists(path));
    EXPECT_TRUE(!NOs::IsDirectoryExists("tmp"));
}

TEST(CruUt, CreateDirectoryAndFiles) {
    std::string path = "tmp/testdir";
    NCommon::TStatus status = NOs::CreateDirectory(path, true);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsDirectoryExists(path));

    std::string filePath = path + "/testfile.txt";
    status = NOs::CreateFile(filePath, true);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_TRUE(NOs::IsFileExists(filePath));

    auto removeStatus = NOs::RemoveDirectory("tmp", false);

    EXPECT_EQ(removeStatus.Error, "");
    EXPECT_EQ(removeStatus.Code, NCommon::TStatus::ECode::Success);

    EXPECT_TRUE(!NOs::IsDirectoryExists(path));
    EXPECT_TRUE(!NOs::IsFileExists(filePath));
}

TEST(CruUt, CheckBackDirectory) {
    std::string testDir = "ut_tmp_dir";
    {
        auto status = NOs::CreateDirectory(testDir);
        EXPECT_EQ(status.Error, "");
        EXPECT_TRUE(NOs::IsDirectoryExists(testDir));
    }
    {
        auto status = NOs::ChangeDirectory(testDir);
        EXPECT_EQ(status.Error, "");
    }
    {
        auto status = NOs::ChangeDirectory("..");
        EXPECT_EQ(status.Error, "");
    }
    {
        auto status = NOs::RemoveDirectory(testDir);
        EXPECT_EQ(status.Error, "");
        EXPECT_TRUE(!NOs::IsDirectoryExists(testDir));    
    }
}

TEST(CruUt, CheckCopyFiles) {
    std::string from = "first";
    std::string fromFile = "first/file";
    std::string to = "second";
    std::string toFile = "second/file";

    NOs::CreateDirectory(from);
    NOs::CreateFile(fromFile);
    NOs::Copy(from, to);

    EXPECT_TRUE(NOs::IsFileExists(toFile));
    {
        auto status = NOs::RemoveDirectory(from, false);
        EXPECT_EQ(status.Error, "");
        EXPECT_TRUE(!NOs::IsDirectoryExists(from));
    }
    {
        auto status = NOs::RemoveDirectory(to, false);
        EXPECT_EQ(status.Error, "");
        EXPECT_TRUE(!NOs::IsDirectoryExists(to));
    }
}

TEST(CruUt, WriteReadFile) {
    std::string input = R"(print("HELLO WORLD!")
    i = 1
    print(i))";
    std::string file = "hello.py";
    auto createStatus = NOs::CreateFile(file, false, 0700);
    EXPECT_EQ(createStatus.Code, NCommon::TStatus::ECode::Success);
    auto writeStatus = NOs::WriteToFile(file, input);
    EXPECT_EQ(writeStatus.Code, NCommon::TStatus::ECode::Success);
    auto [result, status] = NOs::ReadFile(file);
    EXPECT_EQ(status.Code, NCommon::TStatus::ECode::Success);
    EXPECT_EQ(status.Error, "");
    EXPECT_EQ(result, input);
    NOs::RemoveFile(file);
}

int main(int argc, char** argv) {
    std::string testDir = "ut_tmp_dir";
    NOs::CreateDirectory(testDir);
    NOs::ChangeDirectory(testDir);

    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();

    NOs::ChangeDirectory("..");

    bool isEmpty = NOs::IsDirectoryEmpty(testDir);
    NOs::RemoveDirectory(testDir, false);

    if (!isEmpty) {
        throw std::runtime_error(MakeString() << "Directory '" << testDir << "' is not empty after tests.");
    }

    return result;
}

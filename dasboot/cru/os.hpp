#pragma once

#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include <linux/sched.h>    /* Definition of struct clone_args */
#include <sched.h>          /* Definition of CLONE_* constants */
#include <sys/syscall.h>    /* Definition of SYS_* constants */
#include <cstring>
#include <iostream>
#include <sys/prctl.h>
#include <signal.h>
#include <fstream>
#include <sstream>
#include <sys/mount.h>
#include <sys/stat.h>
#include <filesystem>
#include <vector>
#include <fcntl.h>

#include "common.hpp"

namespace NOs {
    namespace fs = std::filesystem;

    using TCloneArgs = clone_args;

    bool IsPathExists(const std::string& path);
    bool IsFile(const std::string& path);
    bool IsFileExists(const std::string& path);
    bool IsDirectory(const std::string& path);
    bool IsDirectoryExists(const std::string& path);
    bool IsDirectoryEmpty(const std::string& path);

    NCommon::TStatus WriteToFile(const std::string& path, const std::string& text);
    NCommon::TStatus Copy(const std::string& source, const std::string& target);

    NCommon::TStatus SetClearEnv();
    NCommon::TStatus Exec(const std::string& program, char* const argv[]);
    NCommon::TStatus Clone(const TCloneArgs& args);

    NCommon::TStatus Mount(const std::string& source, const std::string& target, const std::string& type, uint64_t flags);
    NCommon::TStatus Unmount(const std::string& putOld);
    NCommon::TStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot);

    NCommon::TStatus CreateFile(const std::string& path, bool isDeep = false, mode_t mode = 0755);
    NCommon::TStatus RemoveFile(const std::string& path);

    NCommon::TStatus ChangeDirectory(const std::string& path);
    NCommon::TStatus CreateDirectory(const std::string& path, bool isDeep = false, mode_t mode = 0755);
    NCommon::TStatus RemoveDirectory(const std::string& path, bool shouldBeEmpty = true);

    pid_t GetCurrentPid();
    uid_t GetCurrentUid();

    NCommon::TStatus SetSignalFromParentOnDie(int signal);

    class TPipe {
    public:
        TPipe();
        ~TPipe();
        TPipe(const TPipe&) = delete;
        TPipe& operator=(const TPipe&) = delete;

        void CloseRead() noexcept;
        void CloseWrite() noexcept;

        ssize_t Write(const std::string& s);
        std::string ReadAll(size_t limit = 65536);

    private:
        ssize_t Write(const void* data, size_t size);

        int Fd[2] = {-1, -1};
    };
} // namespace NOs

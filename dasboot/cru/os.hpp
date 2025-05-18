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
    using TStatus = NCommon::TStatus;

    std::string GetRandomTmpPath(const std::string& suffix = "");
    bool IsPathExists(const std::string& path);
    bool IsFile(const std::string& path);
    bool IsFileExists(const std::string& path);
    bool IsDirectory(const std::string& path);
    bool IsDirectoryExists(const std::string& path);
    bool IsDirectoryEmpty(const std::string& path);

    std::pair<TStatus, std::string> ReadFile(const std::string& path);
    TStatus WriteToFile(const std::string& path, const std::string& text);
    TStatus Copy(const std::string& source, const std::string& target);

    TStatus SetClearEnv();
    TStatus Exec(const std::string& program, const std::vector<std::string>& args = {});
    std::pair<pid_t, TStatus> Clone(const TCloneArgs& args);
    std::pair<pid_t, TStatus> Fork();

    TStatus Mount(const std::string& source, const std::string& target, const std::string& type, uint64_t flags = 0);
    TStatus Unmount(const std::string& putOld);
    TStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot);

    TStatus CreateFile(const std::string& path, bool isDeep = false, mode_t mode = 0755, int oflags = O_RDWR | O_CREAT | O_TRUNC);
    TStatus RemoveFile(const std::string& path);
    TStatus RunScriptAsString(const std::string& script);

    TStatus ChangeDirectory(const std::string& path);
    TStatus CreateDirectory(const std::string& path, bool isDeep = false, mode_t mode = 0755);
    TStatus RemoveDirectory(const std::string& path, bool shouldBeEmpty = true);

    TStatus WaitPid(pid_t pid);

    pid_t GetCurrentPid();
    uid_t GetCurrentUid();

    TStatus SetSignalFromParentOnDie(int signal);

    class TPipe {
    public:
        TPipe();
        ~TPipe();
        TPipe(const TPipe&) = delete;
        TPipe& operator=(const TPipe&) = delete;

        void CloseRead() noexcept;
        void CloseWrite() noexcept;

        ssize_t Write(const std::string& s);
        std::string ReadAll(size_t limit = 256);

        int GetWriteFd() noexcept;
        int GetReadFd() noexcept;

    private:
        ssize_t Write(const void* data, size_t size);

        int Fd[2] = {-1, -1};
    };
} // namespace NOs

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

#include "common.hpp"

namespace NOs {
    namespace fs = std::filesystem;

    using TCloneArgs = clone_args;

    bool IsPathExists(const std::string& path);
    bool IsFile(const std::string& path);
    bool IsFileExists(const std::string& path);
    bool IsDirectory(const std::string& path);
    bool IsDirectoryExists(const std::string& path);

    TCommonStatus WriteToFile(const std::string& path, const std::string& text);
    TCommonStatus CopyFile(const std::string& source, const std::string& target);

    TCommonStatus SetClearEnv();
    TCommonStatus Exec(const std::string& program, char* const argv[]);
    TCommonStatus Clone(const TCloneArgs& args);

    TCommonStatus Mount(const std::string& source, const std::string& target, const std::string& type, unsigned long flags);
    TCommonStatus Unmount(const std::string& putOld);
    TCommonStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot);

    TCommonStatus ChangeDirectory(const std::string& path);
    TCommonStatus MakeDirectory(const std::string& path, mode_t mode = 0755);
    TCommonStatus RemoveDirectory(const std::string& path);

    int GetCurrentPid();
    int GetCurrentUid();

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

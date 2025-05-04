#include "os.hpp"

namespace NOs {
    bool IsPathExists(const std::string& path) {
        return fs::exists(path);
    }

    bool IsFile(const std::string& path) {
        return fs::is_regular_file(path);
    }

    bool IsDirectory(const std::string& path) {
        return fs::is_directory(path);
    }

    bool IsFileExists(const std::string& path) {
        return IsPathExists(path) && IsFile(path);
    }

    bool IsDirectoryExists(const std::string& path) {
        return IsPathExists(path) && IsDirectory(path);
    }

    TCommonStatus WriteToFile(const std::string& path, const std::string& text) {
        if (!IsPathExists(path)) {
            std::string error = MakeString() << "Path " << path << " does not exists";
            return { TCommonStatus::ECode::Failed, std::move(error) };
        }

        if (!IsFile(path)) {
            std::string error = MakeString() << path << " is not file";
            return { TCommonStatus::ECode::Failed, std::move(error) };
        }

        std::ofstream fout(path, std::ofstream::out);
        fout << text;
        fout.flush();
        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus CopyFile(const std::string& source, const std::string& target) {
        if (!IsFileExists(source)) {
            std::string error = MakeString() << "Source file " << source << " does not exists";
            return { TCommonStatus::ECode::Failed, std::move(error) };
        }

        if (IsFileExists(target)) {
            std::string error = MakeString() << "Target file " << target << " already exists";
            return { TCommonStatus::ECode::Failed, std::move(error) };
        }

        fs::copy_file(source, target, fs::copy_options::overwrite_existing);
        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus SetClearEnv() {
        TCommonStatus errorStatus = { TCommonStatus::ECode::Failed, "Couldnt clear environment" };

        if (clearenv()) {
            return errorStatus;
        }

        setenv("PATH", "/usr/bin:/bin", 1);
        setenv("HOME", "/root", 1);

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus Exec(const std::string& program, char* const argv[]) {
        int result = execvp(program.c_str(), argv);

        if (result == -1) {
            return { TCommonStatus::ECode::Failed, "Exec() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus Clone(const TCloneArgs& args) {
        TCloneArgs args = {};

        int result = syscall(SYS_clone3, &args, sizeof(args));

        if (result == -1) {
            return { TCommonStatus::ECode::Failed, "Clone() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot) {
        if (syscall(SYS_pivot_root, ".", oldRoot.c_str())) {
            return { TCommonStatus::ECode::Failed, "PivotRoot() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus Mount(const std::string& source, const std::string& target, const std::string& type, unsigned long flags) {
        if (mount(source.c_str(), target.c_str(), type.c_str(), flags, nullptr)) {
            return { TCommonStatus::ECode::Failed, "Mount() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus Unmount(const std::string& putOld) {
        if (umount2(putOld.c_str(), MNT_DETACH)) {
            return { TCommonStatus::ECode::Failed, "Unmount() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus ChangeDirectory(const std::string& path) {
        if (chdir(path.c_str())) {
            return { TCommonStatus::ECode::Failed, "ChangeDirectory() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus MakeDirectory(const std::string& path, mode_t mode) {
        if (mkdir(path.c_str(), mode)) {
            return { TCommonStatus::ECode::Failed, "MakeDirectory() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    TCommonStatus RemoveDirectory(const std::string& path) {
        if (rmdir(path.c_str())) {
            return { TCommonStatus::ECode::Failed, "RemoveDirectory() failed" };
        }

        return { TCommonStatus::ECode::Success };
    }

    int GetCurrentPid() {
        return getpid();
    }

    int GetCurrentUid() {
        return getuid();
    }

// <TPipe>
    TPipe::TPipe() {
        if (::pipe(Fd) == -1) {
            throw std::runtime_error("pipe() failed");
        }
    }

    TPipe::~TPipe() {
        CloseRead();
        CloseWrite();
    }
    
    void TPipe::CloseRead() {
        if (Fd[0] != -1) {
            ::close(Fd[0]);
            Fd[0] = -1;
        }
    }

    void TPipe::CloseWrite() {
        if (Fd[1] != -1) {
            ::close(Fd[1]);
            Fd[1] = -1;
        }
    }

    ssize_t TPipe::Write(const std::string& s) {
        return Write(s.data(), s.size());
    }

    ssize_t TPipe::Write(const void* data, size_t size) {
        int resultSize = ::write(Fd[1], data, size);

        if (resultSize != size) {
            throw std::runtime_error("write() failed");
        }

        return resultSize;
    }

    std::string TPipe::ReadAll(size_t limit) {
        std::vector<char> buf(limit);
        ssize_t n = ::read(Fd[0], buf.data(), buf.size());

        if (n > 0) {
            return std::string(buf.data(), n);
        }

        if (n < 0) {
            throw std::runtime_error("ReadAll() failed");
        }

        return {};
    }
// </TPipe>
} // namespace NOs

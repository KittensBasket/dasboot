#include "os.hpp"

namespace NOs {
namespace {
    std::string GetPrefix(const std::string& path) {
        return fs::path(path).parent_path().string();
    }

    NCommon::TStatus PreCreate(const std::string& path, mode_t mode) {
        fs::path currentDir = "";

        for (const auto &part : fs::path(path) ) {
            currentDir /= part;

            if (!IsDirectoryExists(currentDir)) {
                if (mkdir(currentDir.c_str(), mode)) {
                    std::string error = MakeString() << "PreCreate() failed: " << strerror(errno);
                    return { NCommon::TStatus::ECode::Failed, std::move(error) };
                }
            }
        }

        return { NCommon::TStatus::ECode::Success };
    }
} // anonymous namespace

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

    bool IsDirectoryEmpty(const std::string& path) {
        return fs::is_empty(path);
    }

    NCommon::TStatus ChangeDirectory(const std::string& path) {
        if (!IsDirectoryExists(path)) {
            std::string error = MakeString() << "Directory " << path << " does not exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        try {
            fs::current_path(path);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "ChangeDirectory() failed: " << e.what();
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus CreateDirectory(const std::string& path, bool isDeep, mode_t mode) {
        std::string prefix = GetPrefix(path);
        if (isDeep) {
            auto status = PreCreate(prefix, mode);
            if (status.Code != NCommon::TStatus::ECode::Success) {
                return status;
            }
        }

        if (!prefix.empty() && !IsDirectoryExists(prefix)) {
            std::string error = MakeString() << "Path '" << prefix << "' doesnt exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        if (mkdir(path.c_str(), mode)) {
            return { NCommon::TStatus::ECode::Failed, "MakeDirectory() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus CreateFile(const std::string& path, bool isDeep, mode_t mode) {
        std::string prefix = GetPrefix(path);
        if (isDeep) {
            auto status = PreCreate(prefix, mode);
            if (status.Code != NCommon::TStatus::ECode::Success) {
                return status;
            }
        }

        if (!prefix.empty() && !IsDirectoryExists(prefix)) {
            std::string error = MakeString() << "Path '" << prefix << "' doesnt exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        if (creat(path.c_str(), mode) == -1) {
            std::string error = MakeString() << "CreateFile() failed";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus RemoveFile(const std::string& path) {
        if (!IsFileExists(path)) {
            std::string error = MakeString() << "File " << path << " does not exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        try {
            fs::remove(path);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "RemoveFile() failed: " << e.what();
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus RemoveDirectory(const std::string& path, bool shouldBeEmpty) {
        if (!IsDirectoryExists(path)) {
            std::string error = MakeString() << "Directory " << path << " does not exists";
            return {NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        if (shouldBeEmpty && !fs::is_empty(path)) {
            std::string error = MakeString() << "Directory " << path << " is not empty";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        try {
            if (!fs::remove_all(path)) {
                throw fs::filesystem_error("Failed to remove directory", std::make_error_code(std::errc::operation_canceled));
            }
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "RemoveDirectory() failed: " << e.what();
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus WriteToFile(const std::string& path, const std::string& text) {
        if (!IsPathExists(path)) {
            std::string error = MakeString() << "Path '" << path << "' does not exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        if (!IsFile(path)) {
            std::string error = MakeString() << '\'' << path << "' is not file";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        std::ofstream fout(path, std::ofstream::out);
        fout << text;
        fout.flush();
        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus Copy(const std::string& source, const std::string& target) {
        if (!IsPathExists(source)) {
            std::string error = MakeString() << "Source file \'" << source << "\' does not exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        if (IsPathExists(target)) {
            std::string error = MakeString() << "Target file \'" << target << "\' already exists";
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        try {
            fs::copy(source, target);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "Copy() failed: " << e.what();
            return { NCommon::TStatus::ECode::Failed, std::move(error) };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus SetClearEnv() {
        NCommon::TStatus errorStatus = { NCommon::TStatus::ECode::Failed, "Couldnt clear environment" };

        if (clearenv()) {
            return errorStatus;
        }

        setenv("PATH", "/usr/bin:/bin", 1);
        setenv("HOME", "/root", 1);

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus Exec(const std::string& program, char* const argv[]) {
        int result = execvp(program.c_str(), argv);

        if (result == -1) {
            return { NCommon::TStatus::ECode::Failed, "Exec() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus Clone(const TCloneArgs& args) {
        auto result = syscall(SYS_clone3, &args, sizeof(args));

        if (result == -1) {
            return { NCommon::TStatus::ECode::Failed, "Clone() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot) {
        if (syscall(SYS_pivot_root, rootfs.c_str(), oldRoot.c_str())) {
            return { NCommon::TStatus::ECode::Failed, "PivotRoot() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus Mount(const std::string& source, const std::string& target, const std::string& type, uint64_t flags) {
        if (mount(source.c_str(), target.c_str(), type.c_str(), flags, nullptr)) {
            return { NCommon::TStatus::ECode::Failed, "Mount() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    NCommon::TStatus Unmount(const std::string& putOld) {
        if (umount2(putOld.c_str(), MNT_DETACH)) {
            return { NCommon::TStatus::ECode::Failed, "Unmount() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
    }

    pid_t GetCurrentPid() {
        return getpid();
    }

    uid_t GetCurrentUid() {
        return getuid();
    }

    NCommon::TStatus SetSignalFromParentOnDie(int signal) {
        if (prctl(PR_SET_PDEATHSIG, signal) == -1) {
            return { NCommon::TStatus::ECode::Failed, "SetSignalFromParentOnDie() failed" };
        }

        return { NCommon::TStatus::ECode::Success };
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

    void TPipe::CloseRead() noexcept {
        if (Fd[0] != -1) {
            close(Fd[0]);
            Fd[0] = -1;
        }
    }

    void TPipe::CloseWrite() noexcept {
        if (Fd[1] != -1) {
            close(Fd[1]);
            Fd[1] = -1;
        }
    }

    ssize_t TPipe::Write(const std::string& s) {
        return Write(s.data(), s.size());
    }

    ssize_t TPipe::Write(const void* data, size_t size) {
        ssize_t resultSize = write(Fd[1], data, size);

        if (resultSize < 0 || static_cast<size_t>(resultSize) != size) {
            throw std::runtime_error("write() failed");
        }

        return resultSize;
    }

    std::string TPipe::ReadAll(size_t limit) {
        std::vector<char> buf(limit);
        ssize_t n = read(Fd[0], buf.data(), buf.size());

        if (n > 0) {
            return std::string(buf.data(), static_cast<size_t>(n));
        }

        if (n < 0) {
            throw std::runtime_error("ReadAll() failed");
        }

        return {};
    }
// </TPipe>
} // namespace NOs

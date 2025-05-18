#include <ctime> 
#include <random>

#include "os.hpp"

namespace NOs {
namespace {
    std::string GetPrefix(const std::string& path) {
        return fs::path(path).parent_path().string();
    }

   TStatus PreCreate(const std::string& path, mode_t mode) {
        fs::path currentDir = "";

        for (const auto &part : fs::path(path) ) {
            currentDir /= part;

            if (!IsDirectoryExists(currentDir)) {
                if (mkdir(currentDir.c_str(), mode)) {
                    std::string error = MakeString() << "PreCreate() failed: " << strerror(errno);
                    return { TStatus::ECode::Failed, std::move(error) };
                }
            }
        }

        return { TStatus::ECode::Success };
    }

    std::mt19937 RandomGenerator(static_cast<uint_fast32_t>(time(nullptr)));
} // anonymous namespace
    std::string GetRandomTmpPath(const std::string& suffix) {
        std::string result = "tmp/";
        for (size_t i = 0; i < 30; i++) {
            char c = RandomGenerator() % 26 + 'a';
            result += c;
        }
        result += suffix;
        return result;
    }

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

   TStatus ChangeDirectory(const std::string& path) {
        if (!IsDirectoryExists(path)) {
            std::string error = MakeString() << "Directory " << path << " does not exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        try {
            fs::current_path(path);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "ChangeDirectory() failed: " << e.what();
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

   TStatus CreateDirectory(const std::string& path, bool isDeep, mode_t mode) {
        std::string prefix = GetPrefix(path);
        if (isDeep) {
            auto status = PreCreate(prefix, mode);
            if (status.Code !=TStatus::ECode::Success) {
                return status;
            }
        }

        if (!prefix.empty() && !IsDirectoryExists(prefix)) {
            std::string error = MakeString() << "Path '" << prefix << "' doesnt exists";
            return {TStatus::ECode::Failed, std::move(error) };
        }

        if (IsDirectoryExists(path)) {
            std::string error = MakeString() << "Directory " << path << " already exists";
            return {TStatus::ECode::Ignored, std::move(error) };
        }

        if (mkdir(path.c_str(), mode)) {
            std::string error = MakeString() << "CreateDirectory() failed: " << strerror(errno);
            return {TStatus::ECode::Failed, std::move(error) };
        }

        return {TStatus::ECode::Success };
    }

    TStatus CreateFile(const std::string& path, bool isDeep, mode_t mode, int flags) {
        std::string prefix = GetPrefix(path);
        if (isDeep) {
            auto status = PreCreate(prefix, mode);
            if (status.Code !=TStatus::ECode::Success) {
                return status;
            }
        }

        if (!prefix.empty() && !IsDirectoryExists(prefix)) {
            std::string error = MakeString() << "Path '" << prefix << "' doesnt exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        int fd = open(path.c_str(), flags, mode);

        if (fd == -1) {
            std::string error = MakeString() << "CreateFile() failed";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        close(fd);

        return { TStatus::ECode::Success };
    }

    TStatus RunScriptAsString(const std::string& script) {
        std::string file = GetRandomTmpPath(".sh");
        if (auto status = NOs::CreateFile(file, true, 0777); status.Code != TStatus::Success) {
            status.Error = MakeString() << "RunScript failed. " << status.Error;
            return status;
        }

        if (auto status = NOs::WriteToFile(file, script); status.Code != TStatus::Success) {        
            status.Error = MakeString() << "RunScript failed. " << status.Error;
            NOs::RemoveFile(file);
            return status;
        }

        int ret = std::system(file.c_str());
        if (ret) {
            NOs::RemoveFile(file);
            return { TStatus::ECode::Failed, "RunScript failed: std::system(script) failed." };
        }

        if (auto status = NOs::RemoveFile(file); status.Code != TStatus::Success) {
            status.Error = MakeString() << "RunScript failed. " << status.Error;
            return status;
        }

        return { TStatus::ECode::Success };
    }

    TStatus RemoveFile(const std::string& path) {
        if (!IsFileExists(path)) {
            std::string error = MakeString() << "File " << path << " does not exists";
            return { TStatus::ECode::Ignored, std::move(error) };
        }

        try {
            fs::remove(path);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "RemoveFile() failed: " << e.what();
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

    TStatus RemoveDirectory(const std::string& path, bool shouldBeEmpty) {
        if (!IsDirectoryExists(path)) {
            std::string error = MakeString() << "Directory " << path << " does not exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        if (shouldBeEmpty && !fs::is_empty(path)) {
            std::string error = MakeString() << "Directory " << path << " is not empty";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        try {
            if (!fs::remove_all(path)) {
                throw fs::filesystem_error("Failed to remove directory", std::make_error_code(std::errc::operation_canceled));
            }
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "RemoveDirectory() failed: " << e.what();
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

    std::pair<TStatus, std::string> ReadFile(const std::string& path) {
        if (!IsPathExists(path)) {
            std::string error = MakeString() << "Path '" << path << "' does not exists";
            return {{ TStatus::ECode::Failed, std::move(error) }, ""};
        }

        if (!IsFile(path)) {
            std::string error = MakeString() << '\'' << path << "' is not file";
            return {{ TStatus::ECode::Failed, std::move(error) }, ""};
        }

        std::ifstream fin(path, std::ofstream::in);
        std::string result = "";
        while (fin >> result);
        return {{ TStatus::ECode::Success}, result};
    }

    TStatus WriteToFile(const std::string& path, const std::string& text) {
        if (!IsPathExists(path)) {
            std::string error = MakeString() << "Path '" << path << "' does not exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        if (!IsFile(path)) {
            std::string error = MakeString() << '\'' << path << "' is not file";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        std::ofstream fout(path, std::ofstream::out);
        fout << text;
        fout.flush();
        return { TStatus::ECode::Success };
    }

    TStatus Copy(const std::string& source, const std::string& target) {
        if (!IsPathExists(source)) {
            std::string error = MakeString() << "Source file \'" << source << "\' does not exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        if (IsPathExists(target)) {
            std::string error = MakeString() << "Target file \'" << target << "\' already exists";
            return { TStatus::ECode::Failed, std::move(error) };
        }

        try {
            fs::copy(source, target);
        } catch (const fs::filesystem_error& e) {
            std::string error = MakeString() << "Copy() failed: " << e.what();
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

    TStatus SetClearEnv() {
       TStatus errorStatus = { TStatus::ECode::Failed, "Couldnt clear environment" };

        if (clearenv()) {
            return errorStatus;
        }

        setenv("PATH", "/usr/bin:/bin", 1);
        setenv("HOME", "/root", 1);

        return { TStatus::ECode::Success };
    }

    TStatus Exec(const std::string& program, const std::vector<std::string>& args) {
        std::cout << "Exec() program: " << program << std::endl;

        std::vector<char*> argv;
        argv.reserve(args.size() + 2);
        argv.push_back(const_cast<char*>(program.c_str()));
        for (auto &s : args) {
            argv.push_back(const_cast<char*>(s.c_str()));
        }
        argv.push_back(nullptr);

        int result = execvp(program.c_str(), argv.data());

        std::cout << "Exec() failed: " << strerror(errno) << std::endl;

        if (result == -1) {
            std::string error = MakeString() << "Exec() failed: " << strerror(errno);
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

    std::pair<pid_t,TStatus> Fork() {
        int pid = fork();
        if (pid < 0) {
            return {pid, { TStatus::ECode::Failed, "Fork failed" }};
        }

        return {pid, { TStatus::ECode::Success }};
    }

    std::pair<pid_t,TStatus> Clone(const TCloneArgs& args) {
        auto result = syscall(SYS_clone3, &args, sizeof(args));

        if (result == -1) {
            std::string error = MakeString() << "Clone() failed: " << strerror(errno);
            return { -1, { TStatus::ECode::Failed, error }};
        }

        return { result, { TStatus::ECode::Success }};
    }

    TStatus PivotRoot(const std::string& rootfs, const std::string& oldRoot) {
        if (syscall(SYS_pivot_root, rootfs.c_str(), oldRoot.c_str())) {
            return { TStatus::ECode::Failed, "PivotRoot() failed" };
        }

        return { TStatus::ECode::Success };
    }

    TStatus Mount(const std::string& source, const std::string& target, const std::string& type, uint64_t flags) {
        if (mount(source.c_str(), target.c_str(), type.c_str(), flags, nullptr)) {
            std::string error = MakeString() << "Mount() failed: " << strerror(errno);
            return { TStatus::ECode::Failed, std::move(error) };
        }

        return { TStatus::ECode::Success };
    }

    TStatus Unmount(const std::string& putOld) {
        if (umount2(putOld.c_str(), MNT_DETACH)) {
            return { TStatus::ECode::Failed, "Unmount() failed" };
        }

        return { TStatus::ECode::Success };
    }

    pid_t GetCurrentPid() {
        return getpid();
    }

    uid_t GetCurrentUid() {
        return getuid();
    }

    TStatus SetSignalFromParentOnDie(int signal) {
        if (prctl(PR_SET_PDEATHSIG, signal) == -1) {
            return { TStatus::ECode::Failed, "SetSignalFromParentOnDie() failed" };
        }

        return { TStatus::ECode::Success };
    }

    TStatus WaitPid(pid_t pid) {
        int statusWait;
        waitpid(pid, &statusWait, 0);

        if (WIFEXITED(statusWait)) {
            return { TStatus::ECode::Success };
        }

        if (WIFSIGNALED(statusWait)) {
            return { TStatus::ECode::Failed, "Child process terminated by signal" };
        }

        if (WIFSTOPPED(statusWait)) {
            return { TStatus::ECode::Failed, "Child process stopped" };
        }

        if (WIFCONTINUED(statusWait)) {
            return { TStatus::ECode::Failed, "Child process continued" };
        }

        if (WIFEXITED(statusWait)) {
            return { TStatus::ECode::Failed, "Child process exited" };
        }

        return { TStatus::ECode::Failed, "Unknown child process status" };
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

    int TPipe::GetReadFd() noexcept {
        return Fd[0];
    }

    int TPipe::GetWriteFd() noexcept {
        return Fd[1];
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

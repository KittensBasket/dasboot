#include "container.hpp"

namespace NContainer { // TIsolateProcessCreator implemenation
    TStatus TIsolateProcessCreator::TSelfPreparator::PrepareProcessDie() {
        return NOs::SetSignalFromParentOnDie(SIGTERM);
    }

    TStatus TIsolateProcessCreator::TSelfPreparator::PreparePidNamespace() {
        auto statusCreateDir = NOs::CreateDirectory("/proc", false, 0775);
        if (statusCreateDir.Code == TStatus::Failed) { // can be TStatus::Ignored
            statusCreateDir.Error = "PreparePid: " + statusCreateDir.Error;
            return statusCreateDir;
        }

        auto statusMount = NOs::Mount("proc", "/proc", "proc");
        if (statusMount.Code == TStatus::Failed) {
            statusMount.Error = "PreparePid: " + statusMount.Error;
            return statusMount;
        }

        return { TStatus::Success };
    }

    TStatus TIsolateProcessCreator::TSelfPreparator::PrepareMountNamespace() {
        {
            auto status = NOs::Mount(RunTimeInfo.RootFsPath, RunTimeInfo.RootFsPath, "ext4", MS_BIND);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = NOs::ChangeDirectory(RunTimeInfo.RootFsPath);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        const std::string putOld = ".put_old";

        {
            auto status = NOs::CreateDirectory(putOld, false, 0777);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = NOs::PivotRoot(".", putOld);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = NOs::ChangeDirectory("/");
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = PreparePidNamespace();
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = NOs::Unmount(putOld);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        {
            auto status = NOs::RemoveDirectory(putOld, true);
            if (status.Code == TStatus::Failed) {
                status.Error = "PrepareMountNamespace: " + status.Error;
                return status;
            }
        }

        return { TStatus::Success };
    }

    TStatus TIsolateProcessCreator::TSelfPreparator::PrepareEnvironment() {
        return NOs::SetClearEnv();
    }

    TStatus TIsolateProcessCreator::TSelfPreparator::Prepare() {
        auto status = PrepareProcessDie();
        if (status.Code != TStatus::Success) {
            status.Error = "TSelfPreparator::Prepare: " + status.Error;
            return status;
        }

        status = PrepareMountNamespace();
        if (status.Code != TStatus::Success) {
            status.Error = "TSelfPreparator::Prepare: " + status.Error;
            return status;
        }

        status = PrepareEnvironment();
        if (status.Code != TStatus::Success) {
            status.Error = "TSelfPreparator::Prepare: " + status.Error;
            return status;
        }

        return { TStatus::Success };
    }



    TStatus TIsolateProcessCreator::TByPidPreparator::PrepareNetworkNamespace() {
        if (RunTimeInfo.NeedNetwork) {
            std::string source = "/etc/resolv.conf";
            std::string target = std::string(MakeString() << RunTimeInfo.RootFsPath << "/etc/resolv.conf");
            auto status = NOs::Copy(source, target);

            if (status.Code != TStatus::Success) {
                status.Error = "TByPidPreparator::Prepare: " + status.Error;
                return status;
            }
        } else {
            std::string target = std::string(MakeString() << RunTimeInfo.RootFsPath << "/etc/resolv.conf");
            auto status = NOs::RemoveFile(target); // it can be TStatus::Ignored

            if (status.Code == TStatus::Failed) {
                status.Error = "TByPidPreparator::Prepare: " + status.Error;
                return status;
            }
        }

        return { TStatus::Success };
    }

    TStatus TIsolateProcessCreator::TByPidPreparator::PrepareUserNamespace() {
        auto status = NOs::WriteToFile(
            MakeString() << "/proc/" << Pid << "/uid_map",
            MakeString() << "0 " << Uid << " 1" << '\n'
        );

        if (status.Code == NCommon::TStatus::ECode::Failed) {
            status.Error = "PreparePidNamespace: " + status.Error;
            return status;
        }

        status = NOs::WriteToFile(
            MakeString() << "/proc/" << Pid << "/setgroups",
            "deny\n"
        );

        if (status.Code == NCommon::TStatus::ECode::Failed) {
            status.Error = "PreparePidNamespace: " + status.Error;
            return status;
        }

        status = NOs::WriteToFile(
            MakeString() << "/proc/" << Pid << "/gid_map",
            MakeString() << "0 " << Uid << " 1" << '\n'
        );

        if (status.Code == NCommon::TStatus::ECode::Failed) {
            status.Error = "PreparePidNamespace: " + status.Error;
            return status;
        }

        return { TStatus::Success };
    }

    TStatus TIsolateProcessCreator::TByPidPreparator::Prepare() {
        {
            auto status = PrepareUserNamespace();
            if (status.Code == NCommon::TStatus::ECode::Failed) {
                status.Error = "TByPidPreparator: " + status.Error;
                return status;
            }
        }

        {
            auto status = PrepareNetworkNamespace();
            if (status.Code == NCommon::TStatus::ECode::Failed) {
                status.Error = "TByPidPreparator: " + status.Error;
                return status;
            }
        }

        return { TStatus::Success };
    }



    std::pair<pid_t, TStatus> TIsolateProcessCreator::CloneIsolatedProccess() {
        NOs::TCloneArgs args = {};
        args.flags = CLONE_NEWUTS | CLONE_NEWUSER | CLONE_NEWNS | CLONE_NEWPID;
        if (!Info.NeedNetwork) {
            args.flags |= CLONE_NEWNET;
        }

        args.exit_signal = SIGCHLD;

        auto uid = NOs::GetCurrentUid();
        NOs::TPipe pipe;
        auto [pid, statusClone] = NOs::Clone(std::move(args));

        if (statusClone.Code != TStatus::Success) {
            statusClone.Error = "TIsolateProcessCreator: " + statusClone.Error;
            return {-1, statusClone};
        }

        if (pid == 0) {
            pipe.CloseWrite();
            auto output = pipe.ReadAll();

            if (output == "Ready!") {
                TSelfPreparator preparator(Info);
                auto status = preparator.Prepare();

                if (status.Code != TStatus::Success) {
                    status.Error = "TIsolateProcessCreator: " + status.Error;
                    return {-1, status};
                }
            } else {
                TStatus status;
                status.Code = TStatus::Failed;
                status.Error = MakeString() << "Clone failed: " << output;
                return {-1, status};
            }
        } else if (pid > 0) {
            pipe.CloseRead();

            TByPidPreparator preparator(Info, uid, pid);
            auto status = preparator.Prepare();
            if (status.Code != TStatus::Success) {
                statusClone.Error = "TIsolateProcessCreator: " + statusClone.Error;
                pipe.Write(statusClone.Error);
                return {-1, statusClone};
            }

            pipe.Write("Ready!");
        }

        return { pid, { TStatus::Success } };
    }

    std::pair<pid_t, TStatus> TIsolateProcessCreator::Create() {
        auto [pidRunTime, statusClone] = CloneIsolatedProccess();

        if (statusClone.Code != TStatus::Success) {
            statusClone.Error = "Error in fork for runTime " + statusClone.Error;
            return {-1, statusClone};
        }
        
        return {pidRunTime, { TStatus::Success }};
    }
} // namespace NContainer

namespace NContainer { // TContainer implemenation
    TStatus TContainer::Build(const TBuildInfo& buildInfo) {
        State = EState::Building;
        NOs::TPipe pipe;

        TIsolateProcessCreator::TRunTimeInfo runTimeInfo;
        runTimeInfo.NeedNetwork = buildInfo.NeedNetwork;
        runTimeInfo.RootFsPath = MetaInfo.RootFsPath;
        TIsolateProcessCreator creator(runTimeInfo);

        auto [pid, status] = creator.Create();

        if (status.Code != TStatus::Success) {
            status.Error = MakeString() << "Build error. " << status.Error;
            State = EState::Exited;
            return status;
        }

        if (!pid) { // child
            pipe.CloseRead();
            if (!buildInfo.StaticScript.empty()) {
                auto status = NOs::RunScriptAsString(buildInfo.StaticScript);
                if (status.Code != TStatus::Success) {
                    status.Error = MakeString() << "RunScript failed. " << status.Error;
                    pipe.Write(status.Error);
                    exit(1);
                }
            }

            pipe.Write("Ready!");
            exit(0);
        } else {
            pipe.CloseWrite();
            auto output = pipe.ReadAll();

            if (output == "Ready!") {
                State = EState::Exited;
                return { TStatus::Success };
            } else {
                output = MakeString() << "The static script failed. " << output;
                return { TStatus::Failed, output };
            }
        }

        State = EState::Exited;
        return { TStatus::Failed };
    }

    TStatus TContainer::Exec(const TExecInfo& execInfo) {
        State = EState::Running;

        TIsolateProcessCreator::TRunTimeInfo runTimeInfo;
        runTimeInfo.NeedNetwork = execInfo.NeedNetwork;
        runTimeInfo.RootFsPath = MetaInfo.RootFsPath;
        TIsolateProcessCreator creator(runTimeInfo);

        NOs::TPipe pipe;

        auto [pid, statusCreate] = creator.Create();

        if (statusCreate.Code != TStatus::Success) {
            statusCreate.Error = MakeString() << "Build error. " << statusCreate.Error;
            State = EState::Exited;
            return statusCreate;
        }

        if (!pid) { // child
            pipe.CloseRead();
            if (execInfo.IsInteractive) {
                std::cout << "Interactive exec" << std::endl;
                NOs::Exec("/bin/sh");
                pipe.Write("Interactive exec failed.");
                exit(1);
            } else {
                if (!execInfo.DynamicScript.empty()) {
                    auto status = NOs::RunScriptAsString(execInfo.DynamicScript);
                    if (status.Code != TStatus::Success) {
                        pipe.Write("RunScriptAsString failed.");
                        exit(1);
                    }
                }

                pipe.Write("Ready!");
                exit(0);
            }
        } else {
            pipe.CloseWrite();
            auto output = pipe.ReadAll();

            if (execInfo.IsInteractive) {
                if (output != "") {
                    TStatus status;
                    status.Code = TStatus::Failed;
                    status.Error = MakeString() << "Exec failed. " << output;
                    State = EState::Exited;
                    return status;
                }
            } else {
                if (output != "Ready!") {
                    TStatus status;
                    status.Code = TStatus::Failed;
                    status.Error = MakeString() << "Exec failed. " << "The dynamic script failed. " << output;
                    State = EState::Exited;
                    return status;
                }
            }
        }
        
        State = EState::Exited;
        return { TStatus::Success };
    }
} // namespace NContainer

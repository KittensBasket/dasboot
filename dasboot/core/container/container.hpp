#pragma once

#include <iostream>
#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>

namespace NContainer {
    using TStatus = NCommon::TStatus;

    class TIsolateProcessCreator {
    public:
        struct TRunTimeInfo {
            std::string RootFsPath;
            bool NeedNetwork;
        };

    private:
        const TRunTimeInfo Info;

        // This class customizes a process with *current* pid. So, It is 'self'-preparator.
        // The reason for the existence of this class is that some namespaces are configured only from within the process.
        class TSelfPreparator {
        private:
            const TRunTimeInfo& RunTimeInfo; // be careful with reference

            TStatus PrepareProcessDie();
            TStatus PrepareMountNamespace();
            TStatus PrepareEnvironment();
            TStatus PreparePidNamespace();

        public:
            explicit TSelfPreparator(const TRunTimeInfo& info)
                : RunTimeInfo(info)
            {}

            TStatus Prepare();
        };

        // This class customizes a process with the *specified* pid.
        // The reason for the existence of this class is that some namespaces are configured only from outside the process.
        class TByPidPreparator {
        private:
            const TRunTimeInfo& RunTimeInfo; // be careful with reference
            pid_t Pid;
            uid_t Uid;

            TStatus PrepareNetworkNamespace();
            TStatus PrepareUserNamespace();

        public:
            explicit TByPidPreparator(const TRunTimeInfo& info, uid_t uid, pid_t pid)
                : RunTimeInfo(info)
                , Pid(pid)
                , Uid(uid)
            {}

            TStatus Prepare();
        };

        std::pair<pid_t, TStatus> CloneIsolatedProccess();

    public:
        std::pair<pid_t, TStatus> Create();
        explicit TIsolateProcessCreator(const TRunTimeInfo& info)
            : Info(info)
        {}
    };

    class TContainer {
    public:
        struct TMetaInfo {
            std::string RootFsPath;
            uint64_t id;
        };

        struct TBuildInfo {
            std::string StaticScript = "";
            bool NeedNetwork = false;
        };

        struct TExecInfo {
            std::string DynamicScript = "";
            bool NeedNetwork = false;
            bool IsInteractive = false;
        };

        enum class EState {
            NotInited,
            Building,
            Exited,
            Running
        };

    private:
        TMetaInfo MetaInfo;
        EState State = EState::NotInited;

    private:
        TContainer(const TContainer&) = delete;
        TContainer() = delete;
        TContainer& operator=(const TContainer&) = delete;

    public:
        explicit TContainer(const TMetaInfo& metaInfo)
            : MetaInfo(metaInfo)
        {}

        EState GetState() const {
            return State;
        }

        TStatus Build(const TBuildInfo& buildInfo);
        TStatus Exec(const TExecInfo& execInfo);
    };
} // namespace NContainer

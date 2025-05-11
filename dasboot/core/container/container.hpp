#pragma once

#include <iostream>
#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>

namespace NContainer {
    using TStatus = NCommon::TStatus;

    class TRunTimeCreator {
    public:
        struct TRunTimeInfo {
            std::string rootfs;
        };

    private:
        [[maybe_unused]] const TRunTimeInfo Info;

        // This class customizes a process with *current* pid. So, It is 'self'-preparator.
        // The reason for the existence of this class is that some namespaces are configured only from within the process.
        class TSelfPreparator {
        private:
            [[maybe_unused]] const TRunTimeInfo& RunTimeInfo; // be careful with reference

            TStatus PrepareProcessDie();
            TStatus PreparePidNamespace();
            TStatus PrepareMountNamespace();
            TStatus PrepareEnvironment();

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
            [[maybe_unused]] const TRunTimeInfo& RunTimeInfo; // be careful with reference

            TStatus PrepareNetworkNamespace();
            TStatus PrepareUserNamespace();

        public:
            explicit TByPidPreparator(const TRunTimeInfo& info)
                : RunTimeInfo(info)
            {}

            TStatus Prepare();
        };

        std::pair<pid_t, TStatus> CloneProccess();

    public:
        std::pair<pid_t, TStatus> Create();
        explicit TRunTimeCreator(const TRunTimeInfo& info)
            : Info(info)
        {}
    };

    class TContainer {
    public:
        enum class EImage {
            Alpine
        };

        struct TMetaInfo {
            std::string Path;
            uint64_t id;
            EImage Image = EImage::Alpine;
        };

    private:
        [[maybe_unused]] TMetaInfo MetaInfo;
        [[maybe_unused]] pid_t RunTimePid;

    private:
        TContainer(const TContainer&) = delete;
        TContainer() = delete;
        TContainer& operator=(const TContainer&) = delete;

    public:
        explicit TContainer(const TMetaInfo& metaInfo)
            : MetaInfo(metaInfo)
        {}

        /* Creates diskEnv */
        TStatus Build();
        /* Creates RunTime */
        std::pair<pid_t, TStatus> Exec();
    };
} // namespace NContainer

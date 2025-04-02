#pragma once

#include <string>
#include <optional>
#include <stdlib.h>

#include <../proto/messages.pb.h>

#define TZeroMQSocket int // temporary solution

namespace NController {
    using NMessages::TArg;
    using NMessages::TRequest;
    using NMessages::TResponse;

    using std::string;
    using std::optional;

    /* All of these used for request construction: */
    struct TReferencingRule {
        string Reference;
        bool IsReferencingById;

        TReferencingRule(const string& reference = NULL, const bool isReferencingById = true);
        ~TReferencingRule();
    };

    struct TBuildSettings {
        string PathToDasbootFile;

        optional<string> ContainerName;
    };

    struct TStartSettings {
        TReferencingRule Container;

        TStartSettings(const TReferencingRule& container);
        ~TStartSettings();
    };

    struct TStopSettings {
        TReferencingRule Container;

        TStopSettings(const TReferencingRule&  container);
        ~TStopSettings();
    };

    struct TRemoveSettings {
        TReferencingRule Container;

        TRemoveSettings(const TReferencingRule& container);
        ~TRemoveSettings();
    };

    struct TExecuteSettings {
        TReferencingRule Container;
        string Command;
        string Args;

        optional<bool> backgroundFlag;  

        TExecuteSettings(const TReferencingRule& container, const string command, const string args);
        ~TExecuteSettings();
    };

    struct TListSettings {
        optional<bool> showAllFlag;
    };

    /* Used for default configurations retrievment: */
    class TGlobalConfig final {
        public:
            TGlobalConfig();
            ~TGlobalConfig();

            string GetDaemonPort();

            string GetDefaultConfigPath();
    };

    class TController final {
        private:
            TGlobalConfig globalConfig;
            TZeroMQSocket daemonSocket;

        public:
            TController();
            ~TController();

            bool WriteToDaemon();
            bool ReadFromDaemon();

            // These ask globalConfig:
            bool Version();
            bool Info();
            bool Help();
            bool CommandHelp(const std::string command);

            // These work with Daemon:
            TRequest Build(const TBuildSettings& buildSettings);

            bool Start(const TStartSettings& startSettings);

            bool Stop(const TStopSettings& stopSettings);
            bool Remove(const TRemoveSettings& removeSettings);

            bool Execute(const TExecuteSettings& executeSettings);

            bool List(const TListSettings& listSettings);
    };
};

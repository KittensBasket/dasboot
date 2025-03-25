#pragma once

#include <optional>
#include <string>

#include <message.pb.h>

#define DAEMON_SOCK_ADDR "192.168.1.1:25565" // JTB, TBD
#define DEFAULT_CONFIG "/lib/dasboot/config/default_config.json" // JTB, TBD

#define TProtobuf bool

namespace NController {
    using std::string;
    using std::optional;

    struct TReferencingRule {
        string Reference;
        bool IsReferencingById;

        TReferencingRule(const string& reference = NULL, const bool isReferencingById = true) {
            Reference = reference;
            IsReferencingById = isReferencingById;
        }
        ~TReferencingRule();
    };

    struct TBuildSettings {
        string PathToDasbootFile;

        optional<string> ContainerName;
    };

    struct TStartSettings {
        TReferencingRule Container;

        TStartSettings(const TReferencingRule& container) {
            Container = container;
        }
        ~TStartSettings();
    };

    struct TStopSettings {
        TReferencingRule Container;

        TStopSettings(const TReferencingRule&  container) {
            Container = container;
        }
        ~TStopSettings();
    };

    struct TRemoveSettings {
        TReferencingRule Container;

        TRemoveSettings(const TReferencingRule& container) {
            Container = container;
        }
        ~TRemoveSettings();
    };

    struct TExecuteSettings {
        TReferencingRule Container;
        string Command;
        string Args;

        optional<bool> backgroundFlag;  

        TExecuteSettings(const TReferencingRule& container, const string command, const string args) {
            Container = container;
            Command = command;
            Args = args;
        }
        ~TExecuteSettings();
    };

    struct TListSettings {
        optional<bool> showAllFlag;
    };

    class TController final {
        private:
            std::string daemonSockAddr;
            int daemonSocket; // replace with lib socket type, JBD, TBD

            TProtobuf request;
            TProtobuf response;
        public:
            TController(const std::string daemonSockAddr = DAEMON_SOCK_ADDR) {};
            ~TController() {};

            bool WriteToDaemon();
            bool ReadFromDaemon();

            // These ask TGlobalConfig:
            TProtobuf Version();
            TProtobuf Info();
            TProtobuf Help();
            TProtobuf CommandHelp(const std::string command);

            // These work with Daemon:
            bool Build(const TBuildSettings& buildSettings);

            bool Start(const TStartSettings& startSettings);

            bool Stop(const TStopSettings& stopSettings);
            bool Remove(const TRemoveSettings& removeSettings);

            bool Execute(const TExecuteSettings& executeSettings);

            bool List(const TListSettings& listSettings);
    };

    class TGlobalConfig final {};
};

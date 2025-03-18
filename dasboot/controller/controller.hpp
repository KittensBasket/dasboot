#pragma once

#include <cstddef>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include <vector>

#define DAEMON_SOCK_ADDR "192.168.1.1:25565" // JTB, TBD
#define DEFAULT_CONFIG "/lib/dasboot/config/default_config.json" // JTB, TBD

namespace NController {
    using std::string;
    using std::ifstream;
    using std::vector;

    using TJson = nlohmann::json;

    class TController final {
        private:
            std::string daemonSockAddr;
            int daemonSocket; // replace with lib socket type, JBD, TBD

            TJson request;
            TJson response;

            // REPLACE BOOL WITH ERROR TYPE, JBD
            bool ConnectToDaemon();

        public:
            TController(const std::string daemonSockAddr = DAEMON_SOCK_ADDR);
            ~TController();

            TJson WriteToDaemon();
            TJson ReadFromDaemon();

            // These ask TGlobalConfig:
            TJson Version();
            TJson Info();
            TJson Help();
            TJson CommandHelp(const std::string command);

            // These ones ask Daemon:
            // Here, replace all bool with own error type (or maybe JSON), JBD, TBD
            // Replace rand() with custom random name or id generation
            bool Build();
            bool Build(const string& dasbootFilePath = DEFAULT_CONFIG, const std::string& containerName = std::to_string(rand()));
            bool Start(const string& containerName);
            bool Run(const string& dasbootFilePath = DEFAULT_CONFIG, const std::string& containerName = std::to_string(rand()));
            bool Stop(const string& containerName);
            bool Remove(const string& containerName);

            bool Execute(const string& containerName, const string& command, const vector<string>& args = {}, const bool backgroundFlag = false);
            bool Attach(const string& containerName, const bool noStdinFlag = false);

            bool List(const bool allFlag = false);
    };

    class TGlobalConfig final {};
};

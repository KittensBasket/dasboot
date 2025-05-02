#pragma once

#include <cstddef>
#include <string>
#include <optional>
#include <stdlib.h>

#include <dasboot/cli/cli.hpp>
#include <messages.pb.h>

#define TZeroMQSocket int // TODO: VoidZeroNull0, temporary solution

namespace NController {
    using NMessages::TArg;
    using NMessages::TRequest;
    using NMessages::TResponse;

    using std::string;

    /* TODO: USE PROTOBUF MESSAGES HERE */
    struct TProtobufMessage {
        NMessages::TBuildOptions ProtoBuildOptions;
        NMessages::TRunOptions ProtoRunOptions;
        NMessages::TStartOptions ProtoStartOptions;
        NMessages::TStopOptions ProtoStopOptions;
        NMessages::TPsOptions ProtoPsOptions;
        NMessages::TRmOptions ProtoRmOptions;
        NMessages::TExecOptions ProtoExecOptions;
        NMessages::TAttachOptions ProtoAttachOptions;
    };

    TProtobufMessage ConvertToProtobuf(const NCli::TMainSettings& mainSettings);

    
    /* Used for default configurations retrievment: */
    class TGlobalConfig final {
        public:
            string GetDaemonPort();

            string GetDefaultConfigPath();
    };

    class TController final {
        private:
            const TGlobalConfig GlobalConfig;
            TZeroMQSocket DaemonSocket;

        public:
            TController(TGlobalConfig& globalConfig) : GlobalConfig(globalConfig) {};

            bool WriteToDaemon();
            bool ReadFromDaemon();

            // These ask globalConfig:
            bool Version();
            bool Info();
            bool Help();
            bool CommandHelp(const std::string command);

            // These work with Daemon:
            TRequest Build();

            bool Start();

            bool Stop();
            bool Remove();

            bool Execute();

            bool List();
    };
};

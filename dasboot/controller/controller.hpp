#pragma once

#include <cstddef>
#include <string>
#include <optional>
#include <stdlib.h>
#include <zmq.hpp>

#include <messages.pb.h>

#include <dasboot/cru/common.hpp>

namespace NController {
    using std::string;

    class TController final {
        private:
            zmq::context_t Context;
            zmq::socket_t Socket;
            std::string Address;

        public:
            TController(const std::string& address);

            void StartConnection();
            void WriteToDaemon(std::string& message);
            std::string ReadFromDaemon();

            // command handlers
            void Run(const NMessages::TRunOptions& RunOptions);
            void Build(const NMessages::TBuildOptions& BuildOptions);
            void Start(const NMessages::TStartOptions& StartOptions);
            void Stop(const NMessages::TStopOptions& StopOptions);
            void Ps(const NMessages::TPsOptions& PsOptions);
            void Exec(const NMessages::TExecOptions& ExecOptions);
            void Rm(const NMessages::TRmOptions& RmOptions);
   };
};

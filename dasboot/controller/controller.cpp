#include <dasboot/controller/controller.hpp>

namespace NController {

    void TController::Run(const NMessages::TRunOptions& runOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_run_options()->CopyFrom(runOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Build(const NMessages::TBuildOptions& buildOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_build_options()->CopyFrom(buildOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Start(const NMessages::TStartOptions& startOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_start_options()->CopyFrom(startOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Stop(const NMessages::TStopOptions& stopOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_stop_options()->CopyFrom(stopOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Ps(const NMessages::TPsOptions& psOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_ps_options()->CopyFrom(psOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Exec(const NMessages::TExecOptions& execOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_exec_options()->CopyFrom(execOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Rm(const NMessages::TRmOptions& rmOptions) {
        std::string message;
        NMessages::TOptionsWrapper optionsWrapper;
        optionsWrapper.mutable_rm_options()->CopyFrom(rmOptions);
        optionsWrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }


    TController::TController(const std::string& address)
    : Context(1)
    , Socket(Context, ZMQ_REQ)
    , Address(address)
    {
    }


    void TController::StartConnection() {
        Socket.connect(Address);
    }

    void TController::WriteToDaemon(std::string& message) {
        Socket.send(zmq::buffer(message), zmq::send_flags::none);
    }

    std::string TController::ReadFromDaemon() {
        zmq::message_t message;
        auto result = Socket.recv(message);
        if (!result) {
            throw std::runtime_error("Failed to receive message");
            return "";
        }
        return std::string(static_cast<char*>(message.data()), message.size());
    }


} // namespace NController

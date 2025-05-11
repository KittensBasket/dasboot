#include <dasboot/controller/controller.hpp>

namespace NController {

    void TController::Run(const NMessages::TRunOptions& RunOptions) {
        std::string message;
        RunOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Build(const NMessages::TBuildOptions& BuildOptions) {
        std::string message;
        BuildOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Start(const NMessages::TStartOptions& StartOptions) {
        std::string message;
        StartOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Stop(const NMessages::TStopOptions& StopOptions) {
        std::string message;
        StopOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Ps(const NMessages::TPsOptions& PsOptions) {
        std::string message;
        PsOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Exec(const NMessages::TExecOptions& ExecOptions) {
        std::string message;
        ExecOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Rm(const NMessages::TRmOptions& RmOptions) {
        std::string message;
        RmOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }


    TController::TController(const std::string& adress)
    :
    Context(1), Socket(Context, ZMQ_REQ),  Adress(adress)
    {}


    void TController::StartConnection() {
        Socket.connect(Adress);
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

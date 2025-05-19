#include <dasboot/controller/controller.hpp>

namespace NController {

    void TController::Run(const NMessages::TRunOptions& RunOptions) {
        std::string message;
        RunOptions.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Build(const NMessages::TBuildOptions& BuildOptions) {
        NMessages::TBuildOptions* hoax = new NMessages::TBuildOptions;
        hoax->set_name(BuildOptions.name());
        hoax->set_dasboot_file(BuildOptions.dasboot_file());

        NMessages::TOptionsWrapper wrapper;
        wrapper.set_allocated_buildoptions(hoax);

        std::string message;
        wrapper.SerializeToString(&message);

        std::cout << BuildOptions.dasboot_file() << std::endl;
        std::cout << message << std::endl;

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
        NMessages::TPsOptions* hoax = new NMessages::TPsOptions;
        hoax->set_show_all(PsOptions.show_all());

        NMessages::TOptionsWrapper wrapper;
        wrapper.set_allocated_psoptions(hoax);

        std::string message;
        wrapper.SerializeToString(&message);
        StartConnection();
        WriteToDaemon(message);
    }

    void TController::Exec(const NMessages::TExecOptions& ExecOptions) {
        NMessages::TExecOptions* hoax = new NMessages::TExecOptions;
        hoax->set_name(ExecOptions.name());
        hoax->set_id(ExecOptions.id());
        hoax->set_exec_file(ExecOptions.exec_file());
        hoax->set_is_interactive(ExecOptions.is_interactive());
        hoax->set_detach(ExecOptions.detach());

        NMessages::TOptionsWrapper wrapper;
        wrapper.set_allocated_execoptions(hoax);

        std::string message;
        wrapper.SerializeToString(&message);
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

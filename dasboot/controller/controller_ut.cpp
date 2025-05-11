#include <gtest/gtest.h>

#include <dasboot/controller/controller.hpp>
#include <dasboot/cli/cli.hpp>
#include <messages.pb.h>

using string = std::string;
class TServer final {
    private:
        zmq::context_t Context;
        zmq::socket_t Socket;
        string Adress;

    public:
        explicit TServer(const string& adress)
        : Context(1), Socket(Context, ZMQ_REP), Adress(adress) 
        {}

        void StartConnection() {
            Socket.bind(Adress);
        }
        
        string GetMessage() {
            zmq::message_t message_1;

            [[maybe_unused]] auto result = Socket.recv(message_1, zmq::recv_flags::none);
            string request_str(static_cast<char*>(message_1.data()), message_1.size());
            return request_str;
        }

        void SendMessage() {
            zmq::message_t reply("OK", 2);
            Socket.send(reply, zmq::send_flags::none);
        }
};

namespace {
    TServer server("ipc:///tmp/testsocket");
} // anonymous namespace

TEST(ControllerUt, CommandBuild0) {
    NMessages::TBuildOptions ExpectedBuildOptions;
    string ExpectedString;
    ExpectedBuildOptions.set_name("Container_name");
    ExpectedBuildOptions.set_pathtodasbootfile("path/to/file");
    ExpectedBuildOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.BuildOptions.Name = "Container_name";
    settings.BuildOptions.PathToDasbootFile = "path/to/file";
    string command = "build";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();
    
    ASSERT_EQ(ServerMessage, ExpectedString);
}


TEST(ControllerUt, CommandBuild1) {
    NMessages::TBuildOptions ExpectedBuildOptions;
    string ExpectedString;
    ExpectedBuildOptions.set_name("Container");
    ExpectedBuildOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.BuildOptions.Name = "Container";
    string command = "build";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    server.StartConnection();
    auto result = RUN_ALL_TESTS();
    return result;
}

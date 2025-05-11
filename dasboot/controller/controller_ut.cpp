#include <gtest/gtest.h>

#include <dasboot/controller/controller.hpp>
#include <dasboot/cli/cli.hpp>
#include <messages.pb.h>

using string = std::string;
class TTestServer final {
    private:
        zmq::context_t Context;
        zmq::socket_t Socket;
        string Adress;

    public:
        explicit TTestServer(const string& adress)
        : Context(1), Socket(Context, ZMQ_REP), Adress(adress) 
        {
            Socket.bind(Adress); //Start connection
        }
        
        string GetMessage() {
            zmq::message_t message_1;

            (void)Socket.recv(message_1, zmq::recv_flags::none);
            string request_str(static_cast<char*>(message_1.data()), message_1.size());
            return request_str;
        }

        void SendMessage() {
            zmq::message_t reply("OK", 2);
            Socket.send(reply, zmq::send_flags::none);
        }
};

namespace {
    TTestServer server("ipc:///tmp/testsocket");
} // anonymous namespace

TEST(ControllerUt, CommandRun) {
    NMessages::TRunOptions ExpectedRunOptions;
    string ExpectedString;
    ExpectedRunOptions.set_name("Container_name");
    ExpectedRunOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.RunOptions.Name = "Container_name";
    string command = "run";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();
    
    ASSERT_EQ(ServerMessage, ExpectedString);
}

TEST(ControllerUt, CommandBuild) {
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

TEST(ControllerUt, CommandStart) {
    NMessages::TStartOptions ExpectedStartOptions;
    string ExpectedString;
    ExpectedStartOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    string command = "start";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}

TEST(ControllerUt, CommandStop) {
    NMessages::TStopOptions ExpectedStopOptions;
    string ExpectedString;
    ExpectedStopOptions.set_name("Container_name");
    ExpectedStopOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.StopOptions.Name = "Container_name";
    string command = "stop";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}

TEST(ControllerUt, CommandPs) {
    NMessages::TPsOptions ExpectedPsOptions;
    string ExpectedString;
    ExpectedPsOptions.set_show_all(true);
    ExpectedPsOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.PsOptions.ShowAll = true;
    string command = "ps";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}

TEST(ControllerUt, CommandRm) {
    NMessages::TRmOptions ExpectedRmOptions;
    string ExpectedString;
    ExpectedRmOptions.set_id("Container_id");
    ExpectedRmOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.RmOptions.Id = "Container_id";
    string command = "rm";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}

TEST(ControllerUt, CommandExec) {
    NMessages::TExecOptions ExpectedExecOptions;
    string ExpectedString;
    ExpectedExecOptions.set_name("Container_name");
    ExpectedExecOptions.set_id("Container_id");
    ExpectedExecOptions.set_detach(true);
    ExpectedExecOptions.SerializeToString(&ExpectedString);

    NCli::TMainSettings settings;
    settings.ExecOptions.Name = "Container_name";
    settings.ExecOptions.Id = "Container_id";
    settings.ExecOptions.Detach = true;
    string command = "exec";
    NCli::TSender Sender("ipc:///tmp/testsocket"); 
    Sender.SendMainSettings(settings, command);
    string ServerMessage = server.GetMessage();
    server.SendMessage();

    ASSERT_EQ(ServerMessage, ExpectedString);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

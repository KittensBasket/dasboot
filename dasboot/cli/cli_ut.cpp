#include <string>
#include <gtest/gtest.h>
#include <dasboot/cli/cli.hpp>
#include <dasboot/cru/os.hpp>
#include <nlohmann/json.hpp>

using string = std::string;

TEST(CliUt, ConstructHelp) {
    NCli::TParser parser("Some description");

    bool flag = false;

    parser.AddGlobalFlag("-f", "--flag", flag, "This is flag.");

    {
        string name = "super-command";
        parser.AddGlobalCommand(name, "Super command.");
        std::optional<string> opt;
        parser.AddLocalOption(name, "-o", "--option", opt, "Very good option.");
        bool commandFlag = false;
        parser.AddLocalFlag(name, "-f", "--flag", commandFlag, "Flag from super-command command.");
    }

    auto help = parser.GetHelp();

    auto excpected =
R"(Some description 


Usage: [OPTIONS] [SUBCOMMAND]


OPTIONS:
  -h,     --help              Print this help message and exit 
  -f,     --flag              This is flag. 

SUBCOMMANDS:
  super-command               Super command. 
)";

    ASSERT_EQ(help, excpected);
}

TEST(CliUt, DasbootHelp) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    auto help = parser->GetHelp();

    auto excpected =
R"(A small containerization utility, written in C/C++. Made as team pet project. 


Usage: [OPTIONS] [SUBCOMMAND]


OPTIONS:
  -h,     --help              Print this help message and exit 
  -v,     --version           Print version information and quit 

SUBCOMMANDS:
  info                        Display system-wide information 
  build                       Create a container 
  run                         Run container in interactive mode 
  start                       Launch a container by name or ID depending on specified options 
  stop                        Terminate a running container 
  ps                          Display all available containers. 
  rm                          Delete a container by name or ID depending on specified options 
  exec                        Run one or multiple commands inside an already running container 
  attach                      Connect a terminal to a running container by its ID or name for 
                              interactive management and output monitoring 
)";

    ASSERT_EQ(help, excpected);
}

TEST(CliUt, GetProtoCommandBuild0) {
    string DasbootFile = "DasbootFile", ScriptFile = "script.py";
    NOs::CreateFile(DasbootFile, false, 0700);
    NOs::CreateFile(ScriptFile, false, 0700);

    string input_1 = R"({
	"network" : false,
	"script_file" : "script.py"
})", 
                input_2 = R"(print('HELLO WORLD'))";
    NOs::WriteToFile(DasbootFile, input_1);
    NOs::WriteToFile(ScriptFile, input_2);

    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "build", "-n", "meow", "-f", "DasbootFile"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TBuildOptions ProtoBuildOptions;
    ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
    auto protoText = ProtoBuildOptions.DebugString();

    NMessages::TBuildOptions ExpectedProtoBuildOptions;
    nlohmann::json resultJson;
    resultJson["network"] = false;
    resultJson["script_code"] = input_2;
    ExpectedProtoBuildOptions.set_name("meow");
    ExpectedProtoBuildOptions.set_dasboot_file(resultJson.dump());
    auto expectedText = ExpectedProtoBuildOptions.DebugString();

    NOs::RemoveFile(DasbootFile);
    NOs::RemoveFile(ScriptFile);

    ASSERT_EQ(protoText, expectedText);
}

TEST(CliUt, GetProtoCommandBuild1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "build", "--name", "meow"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TBuildOptions ProtoBuildOptions;
    ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
    auto protoText = ProtoBuildOptions.DebugString();

    auto excpected =
R"(name: "meow"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandBuild2) {
    string DasbootFile = "DasbootFile", ScriptFile = "script.py";
    NOs::CreateFile(DasbootFile, false, 0700);
    NOs::CreateFile(ScriptFile, false, 0700);

    string input_1 = R"({
	"network" : true,
	"script_file" : "script.py"
})", 
                input_2 = R"(n = 3
for i in range(3):
	print("HELLO!!!"))";
    NOs::WriteToFile(DasbootFile, input_1);
    NOs::WriteToFile(ScriptFile, input_2);

    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "build", "--file", "DasbootFile"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TBuildOptions ProtoBuildOptions;
    ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
    auto protoText = ProtoBuildOptions.DebugString();

    NMessages::TBuildOptions ExpectedProtoBuildOptions;
    nlohmann::json resultJson;
    resultJson["network"] = true;
    resultJson["script_code"] = input_2;
    ExpectedProtoBuildOptions.set_dasboot_file(resultJson.dump());
    auto expectedText = ExpectedProtoBuildOptions.DebugString();

    NOs::RemoveFile(DasbootFile);
    NOs::RemoveFile(ScriptFile);

    ASSERT_EQ(protoText, expectedText);
}

TEST(CliUt, GetProtoCommandBuild3) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "build"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TBuildOptions ProtoBuildOptions;
    ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
    auto protoText = ProtoBuildOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRun0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "run", "-n", "meow"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRunOptions ProtoRunOptions;
    ProtoRunOptions = NCli::TConverter::ConvertRunOptions(settings.RunOptions, ProtoRunOptions);
    auto protoText = ProtoRunOptions.DebugString();

    auto excpected =
R"(name: "meow"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRun1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "run", "--name", "meow"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRunOptions ProtoRunOptions;
    ProtoRunOptions = NCli::TConverter::ConvertRunOptions(settings.RunOptions, ProtoRunOptions);
    auto protoText = ProtoRunOptions.DebugString();

    auto excpected =
R"(name: "meow"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRun2) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "run"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRunOptions ProtoRunOptions;
    ProtoRunOptions = NCli::TConverter::ConvertRunOptions(settings.RunOptions, ProtoRunOptions);
    auto protoText = ProtoRunOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStart0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "start", "-n", "meow", "--id", "12345"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStartOptions ProtoStartOptions;
    ProtoStartOptions = NCli::TConverter::ConvertStartOptions(settings.StartOptions, ProtoStartOptions);
    auto protoText = ProtoStartOptions.DebugString();

    auto excpected =
R"(name: "meow"
id: "12345"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStart1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "start", "--name", "meow"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStartOptions ProtoStartOptions;
    ProtoStartOptions = NCli::TConverter::ConvertStartOptions(settings.StartOptions, ProtoStartOptions);
    auto protoText = ProtoStartOptions.DebugString();

    auto excpected =
R"(name: "meow"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStart2) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "start"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStartOptions ProtoStartOptions;
    ProtoStartOptions = NCli::TConverter::ConvertStartOptions(settings.StartOptions, ProtoStartOptions);
    auto protoText = ProtoStartOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStop0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "stop", "-n", "meow", "--id", "12345"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStopOptions ProtoStopOptions;
    ProtoStopOptions = NCli::TConverter::ConvertStopOptions(settings.StopOptions, ProtoStopOptions);
    auto protoText = ProtoStopOptions.DebugString();

    auto excpected =
R"(name: "meow"
id: "12345"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStop1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "stop", "-i", "12345"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStopOptions ProtoStopOptions;
    ProtoStopOptions = NCli::TConverter::ConvertStopOptions(settings.StopOptions, ProtoStopOptions);
    auto protoText = ProtoStopOptions.DebugString();

    auto excpected =
R"(id: "12345"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStop2) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "stop", "--name", "meow"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStopOptions ProtoStopOptions;
    ProtoStopOptions = NCli::TConverter::ConvertStopOptions(settings.StopOptions, ProtoStopOptions);
    auto protoText = ProtoStopOptions.DebugString();

    auto excpected =
R"(name: "meow"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandStop3) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "stop"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TStopOptions ProtoStopOptions;
    ProtoStopOptions = NCli::TConverter::ConvertStopOptions(settings.StopOptions, ProtoStopOptions);
    auto protoText = ProtoStopOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandPs0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "ps", "--all", "1"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TPsOptions ProtoPsOptions;
    ProtoPsOptions = NCli::TConverter::ConvertPsOptions(settings.PsOptions, ProtoPsOptions);
    auto protoText = ProtoPsOptions.DebugString();

    auto excpected =
R"(show_all: true
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandPs1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "ps"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TPsOptions ProtoPsOptions;
    ProtoPsOptions = NCli::TConverter::ConvertPsOptions(settings.PsOptions, ProtoPsOptions);
    auto protoText = ProtoPsOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRm0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "rm", "-n", "meow", "-i", "12345"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRmOptions ProtoRmOptions;
    ProtoRmOptions = NCli::TConverter::ConvertRmOptions(settings.RmOptions, ProtoRmOptions);
    auto protoText = ProtoRmOptions.DebugString();

    auto excpected =
R"(name: "meow"
id: "12345"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRm1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "rm", "--name", "MEOW"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRmOptions ProtoRmOptions;
    ProtoRmOptions = NCli::TConverter::ConvertRmOptions(settings.RmOptions, ProtoRmOptions);
    auto protoText = ProtoRmOptions.DebugString();

    auto excpected =
R"(name: "MEOW"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRm2) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "rm", "--id", "1234512345"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRmOptions ProtoRmOptions;
    ProtoRmOptions = NCli::TConverter::ConvertRmOptions(settings.RmOptions, ProtoRmOptions);
    auto protoText = ProtoRmOptions.DebugString();

    auto excpected =
R"(id: "1234512345"
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandRm3) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "rm"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TRmOptions ProtoRmOptions;
    ProtoRmOptions = NCli::TConverter::ConvertRmOptions(settings.RmOptions, ProtoRmOptions);
    auto protoText = ProtoRmOptions.DebugString();

    auto excpected =
R"()";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandExec0) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "exec", "-n", "meow", "-i", "12345", "-d", "1", "--interactive", "1"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TExecOptions ProtoExecOptions;
    ProtoExecOptions = NCli::TConverter::ConvertExecOptions(settings.ExecOptions, ProtoExecOptions);
    auto protoText = ProtoExecOptions.DebugString();

    auto excpected =
R"(name: "meow"
id: "12345"
is_interactive: true
detach: true
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandExec1) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "exec", "--detach", "0", "--interactive", "1"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TExecOptions ProtoExecOptions;
    ProtoExecOptions = NCli::TConverter::ConvertExecOptions(settings.ExecOptions, ProtoExecOptions);
    auto protoText = ProtoExecOptions.DebugString();

    auto excpected =
R"(is_interactive: true
)";

    ASSERT_EQ(protoText, excpected);
}

TEST(CliUt, GetProtoCommandExec2) {
    string ExecFile = "ExecFile", CopyFile = "file.txt", ScriptFile = "script.py";
    NOs::CreateFile(ExecFile, false, 0700);
    NOs::CreateFile(CopyFile, false, 0700);
    NOs::CreateFile(ScriptFile, false, 0700);

    string input_1 = R"({
	"network" : true,
	"copy_file" : "file.txt",
	"script_file" : "script.py"
})", 
                input_2 = R"(HELLO WORLD)",
                input_3 = R"(print("HELLO!!!"))";
    NOs::WriteToFile(ExecFile, input_1);
    NOs::WriteToFile(CopyFile, input_2);
    NOs::WriteToFile(ScriptFile, input_3);


    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "exec", "--file", "ExecFile"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TExecOptions ProtoExecOptions;
    ProtoExecOptions = NCli::TConverter::ConvertExecOptions(settings.ExecOptions, ProtoExecOptions);
    auto protoText = ProtoExecOptions.DebugString();

    NMessages::TExecOptions ExpectedProtoExecOptions;
    nlohmann::json resultJson;
    resultJson["network"] = true;
    resultJson["copy_file"] = input_2;
    resultJson["script_code"] = input_3;
    ExpectedProtoExecOptions.set_exec_file(resultJson.dump());
    auto expectedText = ExpectedProtoExecOptions.DebugString();

    NOs::RemoveFile(ExecFile);
    NOs::RemoveFile(CopyFile);
    NOs::RemoveFile(ScriptFile);

    ASSERT_EQ(protoText, expectedText);
}

TEST(CliUt, GetProtoCommandExec3) {
    string ExecFile = "ExecFile", 
                CopyFile_1 = "file_1.txt", CopyFile_2 = "file_2.txt", 
                ScriptFile_1 = "script_1.py";
    NOs::CreateFile(ExecFile, false, 0700);
    NOs::CreateFile(CopyFile_1, false, 0700);
    NOs::CreateFile(CopyFile_2, false, 0700);
    NOs::CreateFile(ScriptFile_1, false, 0700);

    string input_1 = R"({
	"network" : true,
	"copy_file" : ["file_1.txt", "file_2.txt"],
	"script_file" : "script_1.py"
})", 
                input_2 = R"(HELLO WORLD)",
                input_3 = R"(print("HELLO!!!"))";
    NOs::WriteToFile(ExecFile, input_1);
    NOs::WriteToFile(CopyFile_1, input_2);
    NOs::WriteToFile(CopyFile_2, input_2);
    NOs::WriteToFile(ScriptFile_1, input_3);


    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "exec", "--file", "ExecFile"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TExecOptions ProtoExecOptions;
    ProtoExecOptions = NCli::TConverter::ConvertExecOptions(settings.ExecOptions, ProtoExecOptions);
    auto protoText = ProtoExecOptions.DebugString();

    NMessages::TExecOptions ExpectedProtoExecOptions;
    nlohmann::json resultJson;
    std::vector<string> CodeFile;
    CodeFile = {input_2, input_2};
    resultJson["network"] = true;
    resultJson["copy_file"] = CodeFile;
    resultJson["script_code"] = input_3;
    ExpectedProtoExecOptions.set_exec_file(resultJson.dump());
    auto expectedText = ExpectedProtoExecOptions.DebugString();

    NOs::RemoveFile(ExecFile);
    NOs::RemoveFile(CopyFile_1);
    NOs::RemoveFile(CopyFile_2);
    NOs::RemoveFile(ScriptFile_1);

    ASSERT_EQ(protoText, expectedText);
}


TEST(CliUt, GetProtoCommandAttach) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    const char* argv0[] = {
    "./dasboot", "attach", "-n", "meow", "-i", "12345", "--no-stdin", "1"
    };
    int argc = sizeof(argv0)/sizeof(argv0[0]);
    char** argv = const_cast<char**>(argv0);

    parser->Parse(argc, argv);

    NMessages::TAttachOptions ProtoAttachOptions;
    ProtoAttachOptions = NCli::TConverter::ConvertAttachOptions(settings.AttachOptions, ProtoAttachOptions);
    auto protoText = ProtoAttachOptions.DebugString();

    auto excpected =
R"(name: "meow"
id: "12345"
nostdin: true
)";

    ASSERT_EQ(protoText, excpected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

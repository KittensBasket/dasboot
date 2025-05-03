#include <gtest/gtest.h>

#include <string>

#include <dasboot/cli/cli.hpp>

TEST(CliUt, ConstructHelp) {
    NCli::TParser parser("Some description");

    bool flag = false;

    parser.AddGlobalFlag("-f", "--flag", flag, "This is flag.");

    {
        std::string name = "super-command";
        parser.AddGlobalCommand(name, "Super command.");
        std::optional<std::string> opt;
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
  build                       Creates a container 
  run                         Run container in interactive mode 
  start                       Launches a container by name or ID depending on specified options 
  stop                        Terminates a running container 
  ps                          Displays all available containers. 
  rm                          Deletes a container by name or ID depending on specified options 
  exec                        Runs one or multiple commands inside an already running container 
  attach                      Connects a terminal to a running container by its ID or name for 
                              interactive management and output monitoring 
)";

    ASSERT_EQ(help, excpected);
}

//---------------------- COMMAND BUILD TESTS------------------------------
TEST(CliUt, GetProtoCommandBuild0) {
  NCli::TMainSettings settings;
  auto parser = NCli::MakeDasbootParser(settings);

  const char* argv0[] = {
    "./dasboot", "build", "-n", "meow", "-f", "/home/bibeda"
  };
  int argc = sizeof(argv0)/sizeof(argv0[0]);
  char** argv = const_cast<char**>(argv0);

  parser->Parse(argc, argv);

  NMessages::TBuildOptions ProtoBuildOptions;
  ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
  auto protoText = ProtoBuildOptions.DebugString();

  auto excpected = 
R"(name: "meow"
pathtodasbootfile: "/home/bibeda"
)";

  ASSERT_EQ(protoText, excpected);
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
  NCli::TMainSettings settings;
  auto parser = NCli::MakeDasbootParser(settings);

  const char* argv0[] = {
    "./dasboot", "build", "--file", "/home/bibeda"
  };
  int argc = sizeof(argv0)/sizeof(argv0[0]);
  char** argv = const_cast<char**>(argv0);

  parser->Parse(argc, argv);

  NMessages::TBuildOptions ProtoBuildOptions;
  ProtoBuildOptions = NCli::TConverter::ConvertBuildOptions(settings.BuildOptions, ProtoBuildOptions);
  auto protoText = ProtoBuildOptions.DebugString();

  auto excpected = 
R"(pathtodasbootfile: "/home/bibeda"
)";

  ASSERT_EQ(protoText, excpected);
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

//-------------------------------- COMMAND RUN TESTS --------------------------------

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


//---------------------------- COMMAND START TESTS ----------------------------------


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

//---------------------------- COMMAND STOP TESTS ----------------------------------


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



//---------------------------- COMMAND PS TESTS ----------------------------------


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


//---------------------------- COMMAND RM TESTS ----------------------------------


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


//---------------------------- COMMAND EXEC TESTS ----------------------------------


TEST(CliUt, GetProtoCommandExec0) {
  NCli::TMainSettings settings;
  auto parser = NCli::MakeDasbootParser(settings);

  const char* argv0[] = {
    "./dasboot", "exec", "-n", "meow", "-i", "12345", "-d", "1"
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
detach: true
)";

  ASSERT_EQ(protoText, excpected);
}


TEST(CliUt, GetProtoCommandExec1) {
  NCli::TMainSettings settings;
  auto parser = NCli::MakeDasbootParser(settings);

  const char* argv0[] = {
    "./dasboot", "exec", "--detach", "0"
  };
  int argc = sizeof(argv0)/sizeof(argv0[0]);
  char** argv = const_cast<char**>(argv0);

  parser->Parse(argc, argv);

  NMessages::TExecOptions ProtoExecOptions;
  ProtoExecOptions = NCli::TConverter::ConvertExecOptions(settings.ExecOptions, ProtoExecOptions);
  auto protoText = ProtoExecOptions.DebugString();

  auto excpected = 
R"()";

  ASSERT_EQ(protoText, excpected);
}



//---------------------------- COMMAND ATTACH TESTS ----------------------------------


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

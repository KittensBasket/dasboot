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
        std::string opt;
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


Usage: [OPTIONS]


OPTIONS:
  -h,     --help              Print this help message and exit 
)";

    ASSERT_EQ(help, excpected);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    auto result = RUN_ALL_TESTS();
    return result;
}

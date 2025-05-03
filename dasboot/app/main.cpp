#include <iostream>
#include <vector>
#include <format>

#include <dasboot/cli/cli.hpp>
#include <dasboot/cli/commands.hpp>
#include <dasboot/controller/controller.hpp>

int main(int argc, char* argv[]) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    RegisterCommands(*parser, settings);

    parser->Parse(argc, argv);

    NCli::TConverter::ConvertToProtobuf(settings);
    return 0;
}

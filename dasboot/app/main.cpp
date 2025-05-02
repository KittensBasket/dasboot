#include <iostream>
#include <vector>

#include <dasboot/cli/cli.hpp>
#include <dasboot/cli/commands.hpp>
#include <dasboot/controller/controller.hpp>

int main(int argc, char* argv[]) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);

    RegisterCommands(*parser, settings);

    parser->Parse(argc, argv);

    NController::TProtobufMessage protoMsg;
    protoMsg = NController::ConvertToProtobuf(settings);

    std::cout << *protoMsg.ProtoStartOptions.mutable_name() << std::endl;
    return 0;
}

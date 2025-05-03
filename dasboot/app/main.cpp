#include <iostream>
#include <vector>
#include <format>

#include <dasboot/cli/cli.hpp>
#include <dasboot/controller/controller.hpp>

int main(int argc, char* argv[]) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);
    parser->Parse(argc, argv);

    std::string command = parser->FindCalledCommand();
    
    if (command == "") {
        parser->GetHelp();
        return 0;
    }
    
    NCli::TConverter::SendMainSettings(settings, command);
    
    return 0;
}

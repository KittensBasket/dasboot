#include <iostream>
#include <vector>

#include <dasboot/cli/cli.hpp>
#include <dasboot/controller/controller.hpp>
#include <dasboot/cru/common.hpp>

int main(int argc, char* argv[]) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);
    parser->Parse(argc, argv);

    std::string command = parser->FindCalledCommand();
    
    if (command.empty()) {
        std::cout << parser->GetHelp();
        return 0;
    }
    
    NCli::TSender Sender("ipc://" DASBOOTD_SOCK); 
    Sender.SendMainSettings(settings, command);
    
    return 0;
}

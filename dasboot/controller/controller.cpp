#include <dasboot/controller/controller.hpp>

namespace NController {

    bool TController::Build(const string& dasbootFilePath = DEFAULT_CONFIG, const std::string& containerName = std::to_string(rand())) {
        ifstream ifs(dasbootFilePath, std::ifstream:in);

        if (!TJson config = json::parse(ifs))
            return false;

        request["containerName"] = containerName;
        request["config"] = config;

        if (!connectToDaemon())
            return false;
        
        WriteToDaemon();
    }
}
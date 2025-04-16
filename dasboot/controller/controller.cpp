#include <dasboot/controller/controller.hpp>

namespace NController {

    TReferencingRule::TReferencingRule(const string& reference, const bool isReferencingById) {
        Reference = reference;
        IsReferencingById = isReferencingById;
    }
    TReferencingRule::~TReferencingRule() {};

    TStartSettings::TStartSettings(const TReferencingRule& container) {
        Container = container;
    }

    TStopSettings::TStopSettings(const TReferencingRule&  container) {
        Container = container;
    }

    TRemoveSettings::TRemoveSettings(const TReferencingRule& container) {
        Container = container;
    }

    TExecuteSettings::TExecuteSettings(const TReferencingRule& container, 
    const string& command, const string& args, const bool backgroundFlag) {
        Container = container;
        Command = command;
        Args = args;
        BackgroundFlag = backgroundFlag;
    }

    TController::TController() {
        globalConfig = TGlobalConfig();
    }
    TController::~TController() {};

    TRequest TController::Build(const TBuildSettings& buildSettings) {
        TRequest request;
        request.set_type("build");

        TArg* arg1 = request.add_args();
        arg1->set_name("PathToDasbootFile");
        arg1->set_value(buildSettings.PathToDasbootFile);

        TArg* arg2 = request.add_args();
        arg2->set_name("ContainerName");
        arg2->set_value(buildSettings.ContainerName.value_or(std::to_string(rand())));

        return request;
    }
} // namespace NController

#include <dasboot/controller/controller.hpp>
#include <dasboot/cli/cli.hpp>


namespace NController {

    TProtobufMessage ConvertToProtobuf(const NCli::TMainSettings& mainSettings) {
        TProtobufMessage protoMsg;
        
        if (mainSettings.StartOptions.name) {
            protoMsg.ProtoStartOptions.set_name(*mainSettings.StartOptions.name);
        }

        if (mainSettings.StartOptions.id) {
            protoMsg.ProtoStartOptions.set_name(*mainSettings.StartOptions.id);
        }
        
        //... и так для каждой структуры Options

        return protoMsg;
    }


} // namespace NController

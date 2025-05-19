#include <dasboot/core/container/container.hpp>
#include "imagemanager.hpp"
#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>
#include <messages.pb.h>
#include <nlohmann/json.hpp>

namespace NCoordinator {
    using TStatus = NCommon::TStatus;

    class TCoordinator {
        NImageManager::TImageManager ImageManager;

        struct TBuildOptionsDeserialized {
            std::string Name;
            bool NeedNetwork = false;
            std::string Script = "";
            std::vector<std::string> CopyFiles = {};
            std::vector<std::string> CopyFilesNames = {};
        };

        struct TExecOptionsDeserialized {
            std::string Name;
            bool NeedNetwork = false;
            std::string Script = "";
            std::vector<std::string> CopyFiles = {};
            std::vector<std::string> CopyFilesNames = {};
            bool IsInteractive = false;
        };

        static TStatus Deserialize(TBuildOptionsDeserialized&, const std::string& json);
        static TStatus Deserialize(TExecOptionsDeserialized&, const std::string& json);

        std::map<std::string, std::unique_ptr<NContainer::TContainer>> Containers;

        TStatus LoadList();
    public:
        TCoordinator();

        std::pair<std::string, TStatus> Ps(const NMessages::TPsOptions&);
        TStatus Build(const NMessages::TBuildOptions&);
        TStatus Exec(const NMessages::TExecOptions&);
    };
} // namespace NCoordinator

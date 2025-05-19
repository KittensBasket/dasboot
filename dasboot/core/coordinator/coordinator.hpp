#include <dasboot/core/container/container.hpp>
#include <dasboot/cru/common.hpp>
#include <dasboot/cru/os.hpp>
#include <messages.pb.h>

namespace NCoordinator {
    using TStatus = NCommon::TStatus;

    class TCoordinator {
        // TImageManager ImageManager;

        std::map<std::string, std::unique_ptr<NContainer::TContainer>> Containers;

        TStatus LoadList();
    public:
        TCoordinator();

        std::pair<std::string, TStatus> Ps(const NMessages::TPsOptions&);
        TStatus Build(const NMessages::TBuildOptions&);
        TStatus Exec(const NMessages::TExecOptions&);
    };
} // namespace NCoordinator

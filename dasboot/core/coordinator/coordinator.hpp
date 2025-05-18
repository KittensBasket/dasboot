#include <dasboot/core/container/container.hpp>
#include <messages.pb.h>

namespace NCoordinator {
    class TCoordinator {
        // TImageManager ImageManager;
    public:
        using TStatus = NCommon::TStatus;

        TStatus Ps(const NMessages::TPsOptions&);
        TStatus Build(const NMessages::TBuildOptions&);
        TStatus Exec(const NMessages::TExecOptions&);
    };
} // namespace NCoordinator

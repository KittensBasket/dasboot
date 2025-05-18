#include "coordinator.hpp"

namespace NCoordinator {
    TCoordinator::TStatus TCoordinator::Ps(const NMessages::TPsOptions&) {
        return { TStatus::Failed };
    }

    TCoordinator::TStatus TCoordinator::Build(const NMessages::TBuildOptions&) {
        return { TStatus::Failed };
    }

    TCoordinator::TStatus TCoordinator::Exec(const NMessages::TExecOptions&) {
        return { TStatus::Failed };
    }
} // namespace NCoordinator

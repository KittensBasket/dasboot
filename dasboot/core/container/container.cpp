#include "container.hpp"

namespace NContainer { // TRunTimeCreator implemenation
    TStatus TRunTimeCreator::TSelfPreparator::PrepareProcessDie() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TSelfPreparator::PreparePidNamespace() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TSelfPreparator::PrepareMountNamespace() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TSelfPreparator::PrepareEnvironment() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TSelfPreparator::Prepare() {
        return { TStatus::Failed };
    }



    TStatus TRunTimeCreator::TByPidPreparator::PrepareNetworkNamespace() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TByPidPreparator::PrepareUserNamespace() {
        return { TStatus::Failed };
    }

    TStatus TRunTimeCreator::TByPidPreparator::Prepare() {
        return { TStatus::Failed };
    }



    std::pair<pid_t, TStatus> TRunTimeCreator::CloneProccess() {
        return { -1, { TStatus::Failed } };
    }

    std::pair<pid_t, TStatus> TRunTimeCreator::Create() {
        return { -1, { TStatus::Failed } };
    }
} // namespace NContainer

namespace NContainer { // TContainer implemenation
    TStatus TContainer::Build() {
        return { TStatus::Failed };
    }

    std::pair<pid_t, TStatus> TContainer::Exec() {
        return { -1, { TStatus::Failed } };
    }
} // namespace NContainer

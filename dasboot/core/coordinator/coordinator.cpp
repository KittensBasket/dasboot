#include "coordinator.hpp"

namespace NCoordinator {
namespace {
    const std::string MainDasbootPath =     "/var/lib/dasboot/";
    const std::string ContainersPath =      "/var/lib/dasboot/containers/";
    const std::string ImagesPath =          "/var/lib/dasboot/images/";
} // anonymous namespace

    TStatus TCoordinator::LoadList() {
        // Containers

        auto [list, status] = NOs::GetListOfFiles(ContainersPath);
        if (status.Code != TStatus::Success) {
            return { TStatus::Failed, status.Error };
        }

        for (const auto& entry : list) {
            if (!NOs::IsDirectory(entry)) {
                continue;
            }

            const auto name = entry.substr(ContainersPath.size());

            NContainer::TContainer::TMetaInfo info;
            info.Name = name;
            info.RootFsPath = MakeString() << entry << "/rootfs";

            auto containerPtr = std::make_unique<NContainer::TContainer>(info, true);
            Containers[name] = std::move(containerPtr);
        }

        return { TStatus::Success };
    }

    TCoordinator::TCoordinator() {
        // ImageManager = TImageManager(MainDasbootPath);

        if (!NOs::IsDirectoryExists(MainDasbootPath)) {
            NOs::CreateDirectory(MainDasbootPath);
        }

        if (!NOs::IsDirectoryExists(ImagesPath)) {
            NOs::CreateDirectory(ImagesPath);
        }

        LoadList();
    }

    std::pair<std::string, TStatus> TCoordinator::Ps(const NMessages::TPsOptions&) {
        std::string result = "List of containers:\n";

        for (const auto& [name, container] : Containers) {
            const auto state = container->GetStateString();
            result += MakeString() << "Name: " << name << " | " << "State: " << state << '\n';
        }

        return { result, { TStatus::Success } };
    }

    TStatus TCoordinator::Build(const NMessages::TBuildOptions& buildOptions) {
        {
            std::string alphinePath = ImagesPath + "alpine";
            if (!NOs::IsDirectoryExists(alphinePath)) {
                // ImageManager.Install();
            }
        }

        std::string name = buildOptions.name();
        std::string fullPath = MakeString() << ContainersPath << name;

        if (NOs::IsPathExists(fullPath)) {
            return { TStatus::Failed, MakeString() << "Container with name " << name << " already exists. "};
        }

        // TODO(flown4qqqq)

        return { TStatus::Failed };
    }

    TStatus TCoordinator::Exec(const NMessages::TExecOptions&) {
        return { TStatus::Failed };
    }
} // namespace NCoordinator

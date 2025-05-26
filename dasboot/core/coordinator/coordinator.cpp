#include "coordinator.hpp"
#include <chrono>
#include <thread>

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
        ImageManager = NImageManager::TImageManager();

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

    TStatus TCoordinator::Deserialize(TBuildOptionsDeserialized& opts, const std::string& stringJson) {
        nlohmann::json jsonValue = nlohmann::json::parse(stringJson);

        if (jsonValue.contains("copy_file")) {
            opts.CopyFiles = jsonValue["copy_file"].array();
        }

        if (jsonValue.contains("copy_file_names")) {
            opts.CopyFilesNames = jsonValue["copy_file_names"].array();
        }

        if (jsonValue.contains("script_code")) {
            opts.Script = jsonValue["script_code"];
        }

        if (jsonValue.contains("network")) {
            opts.NeedNetwork = jsonValue["network"];
        }

        return { TStatus::Success };
    }

    TStatus TCoordinator::Build(const NMessages::TBuildOptions& buildOptions) {
        // in the future there will be any type of image
        std::string alphinePath = ImagesPath + "alpine";
        if (!NOs::IsDirectoryExists(alphinePath)) {
            ImageManager.Install(NImageManager::Images::Alpine);
        }

        std::string name = buildOptions.name();

        if (Containers.contains(name)) {
            return { TStatus::Failed, MakeString() << "Container with name " << name << " already exists. "};
        }

        std::string containerPath = MakeString() << ContainersPath << name;
        if (auto status = NOs::CreateDirectory(containerPath); status.Code != TStatus::Success) {
            return status;
        }

        TBuildOptionsDeserialized options;
        std::string serializedJson = buildOptions.dasboot_file();
        Deserialize(options, serializedJson);

        std::string rootfsPath = MakeString() << containerPath << "/rootfs";
        auto copyStatus = NOs::Copy(alphinePath, rootfsPath);

        if (copyStatus.Code != TStatus::Success) {
            return { TStatus::Failed, MakeString() << "Failed to copy image: " << copyStatus.Error };
        }

        for (size_t i = 0; i < options.CopyFilesNames.size(); ++i) {
            const std::string name = options.CopyFilesNames[i];
            const std::string data = options.CopyFiles[i];

            std::string fullPath = MakeString() << rootfsPath << "/" << name;
            NOs::CreateFile(fullPath, true, 0755);
            NOs::WriteToFile(fullPath, data);
        }

        NContainer::TContainer::TMetaInfo metaBuildInfo;
        metaBuildInfo.Name = name;
        metaBuildInfo.RootFsPath = rootfsPath;
        Containers[name] = std::make_unique<NContainer::TContainer>(metaBuildInfo);

        NContainer::TContainer::TBuildInfo buildInfo;
        buildInfo.NeedNetwork = options.NeedNetwork;
        buildInfo.StaticScript = options.Script;
        return Containers[name]->Build(buildInfo);
    }

    TStatus TCoordinator::Deserialize(TExecOptionsDeserialized& opts, const std::string& stringJson) {
        nlohmann::json jsonValue = nlohmann::json::parse(stringJson);

        if (jsonValue.contains("copy_file")) {
            opts.CopyFiles = jsonValue["copy_file"].array();
        }

        if (jsonValue.contains("copy_file_names")) {
            opts.CopyFilesNames = jsonValue["copy_file_names"].array();
        }

        if (jsonValue.contains("script_code")) {
            opts.Script = jsonValue["script_code"];
        }

        return { TStatus::Success };
    }

    TStatus TCoordinator::Exec(const NMessages::TExecOptions& execOptions) {
        std::string name = execOptions.name();
        if (!Containers.contains(name)) {
            return { TStatus::Failed, MakeString() << "Container with name " << name << " does not exists. "};
        }

        TExecOptionsDeserialized options;

        if (execOptions.has_is_interactive()) {
            options.IsInteractive = execOptions.is_interactive();
        }

        if (execOptions.has_exec_file()) {
            std::string serializedJson = execOptions.exec_file();
            Deserialize(options, serializedJson);
        }

        std::string containerPath = MakeString() << ContainersPath << name;
        std::string rootfsPath = MakeString() << containerPath << "/rootfs";
        for (size_t i = 0; i < options.CopyFilesNames.size(); ++i) {
            const std::string name = options.CopyFilesNames[i];
            const std::string data = options.CopyFiles[i];

            std::string fullPath = MakeString() << rootfsPath << "/" << name;
            NOs::CreateFile(fullPath, true, 0755);
            NOs::WriteToFile(fullPath, data);
        }

        while (true) {
            using EState = NContainer::TContainer::EState;
            if (Containers[name]->GetState() == EState::Exited) {
                NContainer::TContainer::TExecInfo execInfo;
                execInfo.NeedNetwork = options.NeedNetwork;
                execInfo.DynamicScript = options.Script;
                execInfo.IsInteractive = options.IsInteractive;
                return Containers[name]->Exec(execInfo);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
} // namespace NCoordinator

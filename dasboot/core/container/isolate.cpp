#include <iostream>
#include <dasboot/core/container/container.hpp>

signed main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    NContainer::TContainer::TMetaInfo metaInfo;
    metaInfo.RootFsPath = "./rootfs"; // If you want to use it, download it.
    metaInfo.id = 1;

    NContainer::TContainer container(metaInfo);

    {
        NContainer::TContainer::TBuildInfo buildInfo;
        buildInfo.NeedNetwork = false;
        buildInfo.StaticScript = "echo 'hello, world!'";

        auto status = container.Build(buildInfo);

        if (status.Code != NCommon::TStatus::ECode::Success) {
            std::cout << "Build failed: " << status.Error << std::endl;
            return 1;
        }

        std::cout << "Build succeeded!" << std::endl;
    }

    {
        NContainer::TContainer::TExecInfo execInfo;
        execInfo.NeedNetwork = true;
        execInfo.IsInteractive = true;
        auto status = container.Exec(execInfo);

        if (status.Code != NCommon::TStatus::ECode::Success) {
            std::cout << "Exec failed: " << status.Error << std::endl;
            return 1;
        }

        std::cout << "Exec succeeded!" << std::endl;
    }

    {
        NContainer::TContainer::TExecInfo execInfo;
        execInfo.NeedNetwork = false;
        execInfo.DynamicScript = R"(
            for i in $(seq 1 10)
            do
                echo "hello, exec world!"
                sleep 1
            done
        )";

        auto status = container.Exec(execInfo);

        if (status.Code != NCommon::TStatus::ECode::Success) {
            std::cout << "Exec failed: " << status.Error << std::endl;
            return 1;
        }

        std::cout << "Exec succeeded!" << std::endl;
    }

    return 0;
}

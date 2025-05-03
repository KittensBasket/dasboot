#pragma once

#include <CLI/CLI.hpp>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <messages.pb.h>

namespace NCli {
    using std::string;

    using TValue = std::optional<string>;

    class TParser final {
    private:
        mutable CLI::App App;
        std::unordered_map<std::string, CLI::App*> Commands;

        static string BuildFullName(const string& shortName, const string& longName);

    public:
        TParser(const string& description);

        void AddGlobalFlag(const string& shortName, const string& longName, bool& flag, const string& description);
        void AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description);
        void AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);

        CLI::App* AddGlobalCommand(const string& commandName, const string& description);
        void AddLocalFlag(const string& commandName, const string& shortName, const string& longName, bool& flag, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);

        int Parse(int argc, char* argv[]) const;
        string GetHelp() const;
    };

    struct TVersion {
        bool printVersion = false;
    };
    struct TInfo {
        CLI::App* isCalled;
    };
    struct TRunOptions {
        CLI::App* isCalled;
        std::optional<string> name;
    };
    struct TBuildOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> pathtodasbootfile;
    };
    struct TStartOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TStopOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TPsOptions {
        CLI::App* isCalled;
        bool showAll = false; 
    };
    struct TRmOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TExecOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> id;
        bool detach = false;
    };
    struct TAttachOptions {
        CLI::App* isCalled;
        std::optional<string> name;
        std::optional<string> id;
        bool no_stdin = false;
    };

    struct TMainSettings {
        TVersion Version;
        TInfo Info;
        TRunOptions RunOptions;
        TBuildOptions BuildOptions;
        TStartOptions StartOptions;
        TStopOptions StopOptions;
        TPsOptions PsOptions;
        TRmOptions RmOptions;
        TExecOptions ExecOptions;
        TAttachOptions AttachOptions;
    };

    class TConverter {
    public:
        static bool CommandCallback(const CLI::App* command);
        static void ConvertToProtobuf(const TMainSettings& mainSettings);
        static bool ConvertRunOptions(const NCli::TRunOptions& options, NMessages::TRunOptions& protoOptions);
        static bool ConvertBuildOptions(const NCli::TBuildOptions& options, NMessages::TBuildOptions& protoOptions);
        static bool ConvertStartOptions(const NCli::TStartOptions& options, NMessages::TStartOptions& protoOptions);
        static bool ConvertStopOptions(const NCli::TStopOptions& options, NMessages::TStopOptions& protoOptions);
        static bool ConvertPsOptions(const NCli::TPsOptions& options, NMessages::TPsOptions& protoOptions);
        static bool ConvertRmOptions(const NCli::TRmOptions& options, NMessages::TRmOptions& protoOptions);
        static bool ConvertExecOptions(const NCli::TExecOptions& options, NMessages::TExecOptions& protoOptions);
        static bool ConvertAttachOptions(const NCli::TAttachOptions& options, NMessages::TAttachOptions& protoOptions);
    };

    std::unique_ptr<TParser> MakeDasbootParser(TMainSettings& settings);
};

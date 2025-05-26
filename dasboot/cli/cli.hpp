#pragma once

#include <CLI/CLI.hpp>
#include <iostream>
#include <optional>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include <dasboot/cru/os.hpp>
#include <dasboot/controller/controller.hpp>
#include <messages.pb.h>

namespace NCli {
    using std::string;

    using TValue = std::optional<string>;

    struct TVersion {
        bool PrintVersion = false;
    };

    struct TRunOptions {
        std::optional<string> Name;
    };

    struct TBuildOptions {
        std::optional<string> Name;
        std::optional<string> PathToDasbootFile;
    };

    struct TStartOptions {
        std::optional<string> Name;
        std::optional<string> Id;
    };

    struct TStopOptions {
        std::optional<string> Name;
        std::optional<string> Id;
    };

    struct TPsOptions {
        bool ShowAll = false; 
    };

    struct TRmOptions {
        std::optional<string> Name;
        std::optional<string> Id;
    };

    struct TExecOptions {
        std::optional<string> Name;
        std::optional<string> Id;
        std::optional<string> ExecFile;
        bool IsInteractive = false;
        bool Detach = false;
    };
    
    struct TAttachOptions {
        std::optional<string> Name;
        std::optional<string> Id;
        bool NoStdin = false;
    };

    struct TMainSettings {
        TVersion Version;
        TRunOptions RunOptions;
        TBuildOptions BuildOptions;
        TStartOptions StartOptions;
        TStopOptions StopOptions;
        TPsOptions PsOptions;
        TRmOptions RmOptions;
        TExecOptions ExecOptions;
        TAttachOptions AttachOptions;
    };

    class TParser final {
    private:
        CLI::App App;
        std::unordered_map<string, CLI::App*> Commands;

        static string BuildFullName(const string& shortName, const string& longName);

    public:
        TParser(const string& description);

        void AddGlobalFlag(const string& shortName, const string& longName, bool& flag, const string& description);
        void AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description);
        void AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);

        void AddGlobalCommand(const string& commandName, const string& description);
        void AddLocalFlag(const string& commandName, const string& shortName, const string& longName, bool& flag, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);
        
        string FindCalledCommand();
        int Parse(int argc, char* argv[]);
        string GetHelp() const;

        void RegisterCommands(NCli::TMainSettings& mainSettings);
    };

    class TConverter {
    public:
        static string ReadDasbootFile(const string& path);
        static string ReadExecFile(const string& path, bool isInteractive);
        static string GetFilename(const string& path);
        static NMessages::TRunOptions ConvertRunOptions(const NCli::TRunOptions& options, NMessages::TRunOptions& protoOptions);
        static NMessages::TBuildOptions ConvertBuildOptions(const NCli::TBuildOptions& options, NMessages::TBuildOptions& protoOptions);
        static NMessages::TStartOptions ConvertStartOptions(const NCli::TStartOptions& options, NMessages::TStartOptions& protoOptions);
        static NMessages::TStopOptions ConvertStopOptions(const NCli::TStopOptions& options, NMessages::TStopOptions& protoOptions);
        static NMessages::TPsOptions ConvertPsOptions(const NCli::TPsOptions& options, NMessages::TPsOptions& protoOptions);
        static NMessages::TRmOptions ConvertRmOptions(const NCli::TRmOptions& options, NMessages::TRmOptions& protoOptions);
        static NMessages::TExecOptions ConvertExecOptions(const NCli::TExecOptions& options, NMessages::TExecOptions& protoOptions);
        static NMessages::TAttachOptions ConvertAttachOptions(const NCli::TAttachOptions& options, NMessages::TAttachOptions& protoOptions);
    };

    class TSender {
    private:
        NController::TController Controller;
    public:
        TSender(const string& address);
        void SendMainSettings(const TMainSettings& mainSettings, const string& command);
    };

    std::unique_ptr<TParser> MakeDasbootParser(TMainSettings& settings);
};

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

        void AddGlobalCommand(const string& commandName, const string& description);
        void AddLocalFlag(const string& commandName, const string& shortName, const string& longName, bool& flag, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);

        int Parse(int argc, char* argv[]) const;
        string GetHelp() const;
    };

    struct TRunOptions {
        std::optional<string> name;
        // std::optional<string> id;
    };
    struct TBuildOptions {
        std::optional<string> name;
        // std::optional<string> id;
    };
    struct TStartOptions {
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TStopOptions {
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TPsOptions {
        bool showAll; 
    };
    struct TRmOptions {
        std::optional<string> name;
        std::optional<string> id;
    };
    struct TExecOptions {
        std::optional<string> name;
        std::optional<string> id;
        bool detach;
    };
    struct TAttachOptions {
        std::optional<string> name;
        std::optional<string> id;
        bool no_stdin;
    };

    struct TMainSettings {
        // instead, there should be a structure from the controller.
        TRunOptions RunOptions;
        TBuildOptions BuildOptions;
        TStartOptions StartOptions;
        TStopOptions StopOptions;
        TPsOptions PsOptions;
        TRmOptions RmOptions;
        TExecOptions ExecOptions;
        TAttachOptions AttachOptions;
    };

    std::unique_ptr<TParser> MakeDasbootParser(TMainSettings& settings);
};

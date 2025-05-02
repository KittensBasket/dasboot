#pragma once

#include <CLI/CLI.hpp>
#include <iostream>
#include <optional>
#include <unordered_map>

namespace NCli {
    using std::string;

    using TValue = std::string;

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
        // void AddGlobalOption(const string& shortName, const string& longName, string value, const string& description);
        // void AddGlobalOption(const string& shortName, const string& longName, string value, const string& description, CLI::Validator&& validator);

        void AddGlobalCommand(const string& commandName, const string& description);
        void AddLocalFlag(const string& commandName, const string& shortName, const string& longName, bool& flag, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description);
        void AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator);
        // void AddLocalOption(const string& commandName, const string& shortName, const string& longName, string value, const string& description);
        // void AddLocalOption(const string& commandName, const string& shortName, const string& longName, string value, const string& description, CLI::Validator&& validator);

        int Parse(int argc, char* argv[]) const;
        string GetHelp() const;
    };

    struct TMainSettings {
        // instead, there should be a structure from the controller.
        // NMessages::TBuildOptions BuildOptions;
        NMessages::TStartOptions StartOptions;
        struct {
            std::optional<string> a;
            std::optional<string> b;
        } Meow;
        // NMessages::TRunOptions RunOptions;
        // NMessages::TStopOptions StopOptions;
        // NMessages::TPsOptions PsOptions;
        // NMessages::TRmOptions RmOptions;
        // NMessages::TExecOptions ExecOptions;
        // NMessages::TAttachOptions AttachOptions;

    };

    std::unique_ptr<TParser> MakeDasbootParser(TMainSettings& settings);
};

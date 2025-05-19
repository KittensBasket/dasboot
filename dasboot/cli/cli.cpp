#include <dasboot/cli/cli.hpp>

namespace NCli {
    using std::string;

    TParser::TParser(const string& description)
    : App(description)
    {}

    string TParser::BuildFullName(const string& shortName, const string& longName) {
        return shortName + "," + longName;
    }

    void TParser::AddGlobalFlag(const string& shortName, const string& longName, bool& flag, const string& description) {
        App.add_flag(BuildFullName(shortName, longName), flag, description);
    }

    void TParser::AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description) {
        App.add_option(BuildFullName(shortName, longName), value, description);
    }

    void TParser::AddGlobalOption(const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator) {
        App.add_option(BuildFullName(shortName, longName), value, description)->transform(std::move(validator));
    }

    void TParser::AddGlobalCommand(const string& commandName, const string& description){
        Commands[commandName] = App.add_subcommand(commandName, description);
    }

    void TParser::AddLocalFlag(const string& commandName, const string& shortName, const string& longName, bool& flag, const string& description) {
        Commands[commandName]->add_option(BuildFullName(shortName, longName), flag, description);
    }

    void TParser::AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description) {
        Commands[commandName]->add_option(BuildFullName(shortName, longName), value, description);
    }

    void TParser::AddLocalOption(const string& commandName, const string& shortName, const string& longName, TValue& value, const string& description, CLI::Validator&& validator) {
        Commands[commandName]->add_option(BuildFullName(shortName, longName), value, description)->transform(validator);
    }

    string TParser::FindCalledCommand() {
        for (const auto& [command, interface] : Commands) {
            if (interface->parsed() && (!interface->get_help_ptr()->as<bool>())) {
                return command;
            }
        }
        return "";
    }
    
    int TParser::Parse(int argc, char* argv[]) {
        CLI11_PARSE(App, argc, argv);
        return 0;
    }

    string TParser::GetHelp() const {
        return App.help();
    }

    namespace {
        // Dasboot description
        static const string DasbootDescription = "A small containerization utility, written in C/C++. Made as team pet project.";
    
        // Common option descriptions
        static const string ContainerNameDescription = "Container name";
        static const string ContainerIdDescription = "Container ID";
        static const string BuildFromFileDescription = "Create a container from a DasbootFile";
        static const string ShowAllContainersDescription = "List all containers, including stopped ones.";
        static const string DetachFlagDescription = "Detached mode: run command in the background";
        static const string NoStdinFlagDescription = "Do not attach STDIN";
        static const string ExecFileDescription = "Path to ExecFile";
        static const string InteractiveContainerDescription = "Run container in interactive mode"; //maybe edit

        // Command descriptions
        static const string VersionDescription = "Print version information and quit";
        static const string InfoDescription = "Display system-wide information";
        static const string BuildDescription = "Create a container";
        static const string RunDescription = "Run container in interactive mode";
        static const string StartDescription = "Launch a container by name or ID depending on specified options";
        static const string StopDescription = "Terminate a running container";
        static const string PsDescription = "Display all available containers.";
        static const string RmDescription = "Delete a container by name or ID depending on specified options";
        static const string ExecDescription = "Run one or multiple commands inside an already running container";
        static const string AttachDescription = "Connect a terminal to a running container by its ID or name for interactive management and output monitoring";
    } // anonymous namespace

    void TParser::RegisterCommands(TMainSettings& mainSettings) {
        AddGlobalFlag("-v", "--version", mainSettings.Version.PrintVersion, VersionDescription);

        AddGlobalCommand("info", InfoDescription);

        AddGlobalCommand("build", BuildDescription);
        AddLocalOption("build", "-n", "--name", mainSettings.BuildOptions.Name, ContainerNameDescription);
        AddLocalOption("build", "-f", "--file", mainSettings.BuildOptions.PathToDasbootFile, BuildFromFileDescription);

        AddGlobalCommand("run", RunDescription);
        AddLocalOption("run", "-n", "--name", mainSettings.RunOptions.Name, ContainerNameDescription);

        AddGlobalCommand("start", StartDescription);
        AddLocalOption("start", "-n", "--name",  mainSettings.StartOptions.Name, ContainerNameDescription);
        AddLocalOption("start", "-i", "--id",  mainSettings.StartOptions.Id, ContainerIdDescription);

        AddGlobalCommand("stop", StopDescription);
        AddLocalOption("stop", "-n", "--name", mainSettings.StopOptions.Name, ContainerNameDescription);
        AddLocalOption("stop", "-i", "--id", mainSettings.StopOptions.Id, ContainerIdDescription);

        AddGlobalCommand("ps", PsDescription);
        AddLocalFlag("ps", "-a", "--all", mainSettings.PsOptions.ShowAll, ShowAllContainersDescription);

        AddGlobalCommand("rm", RmDescription);
        AddLocalOption("rm", "-n", "--name", mainSettings.RmOptions.Name, ContainerNameDescription);
        AddLocalOption("rm", "-i", "--id", mainSettings.RmOptions.Id, ContainerIdDescription);

        AddGlobalCommand("exec", ExecDescription);
        AddLocalOption("exec", "-n", "--name", mainSettings.ExecOptions.Name, ContainerNameDescription);
        AddLocalOption("exec", "-i", "--id", mainSettings.ExecOptions.Id, ContainerIdDescription);
        AddLocalFlag("exec", "", "--interactive", mainSettings.ExecOptions.IsInteractive, InteractiveContainerDescription);
        AddLocalOption("exec", "-f", "--file", mainSettings.ExecOptions.ExecFile, ExecFileDescription);
        AddLocalFlag("exec", "-d", "--detach", mainSettings.ExecOptions.Detach, DetachFlagDescription);

        AddGlobalCommand("attach", AttachDescription);
        AddLocalOption("attach", "-n", "--name", mainSettings.AttachOptions.Name, ContainerNameDescription);
        AddLocalOption("attach", "-i", "--id", mainSettings.AttachOptions.Id, ContainerIdDescription);
        AddLocalFlag("attach", "", "--no-stdin", mainSettings.AttachOptions.NoStdin, NoStdinFlagDescription);
    }

    TSender::TSender(const string adress)
    : Controller(adress) {}

    void TSender::SendMainSettings(const TMainSettings& mainSettings, const string& command) {
        if (mainSettings.Version.PrintVersion) {
            //print version -- function in controller
        }

        else if (command == "info") {
            //controller handle call
        }

        else if (command == "build") {
            NMessages::TBuildOptions ProtoBuildOptions;
            ProtoBuildOptions = TConverter::ConvertBuildOptions(mainSettings.BuildOptions, ProtoBuildOptions);
            Controller.Build(ProtoBuildOptions);
        } 

        else if (command == "run") {
            NMessages::TRunOptions ProtoRunOptions;
            ProtoRunOptions = TConverter::ConvertRunOptions(mainSettings.RunOptions, ProtoRunOptions);
            Controller.Run(ProtoRunOptions);
        } 

        else if (command == "start") {
            NMessages::TStartOptions ProtoStartOptions;
            ProtoStartOptions = TConverter::ConvertStartOptions(mainSettings.StartOptions, ProtoStartOptions);
            Controller.Start(ProtoStartOptions);
        }

        else if (command == "stop") {
            NMessages::TStopOptions ProtoStopOptions;
            ProtoStopOptions = TConverter::ConvertStopOptions(mainSettings.StopOptions, ProtoStopOptions);
            Controller.Stop(ProtoStopOptions);
        }

        else if (command == "ps") {
            NMessages::TPsOptions ProtoPsOptions;
            ProtoPsOptions = TConverter::ConvertPsOptions(mainSettings.PsOptions, ProtoPsOptions);
            Controller.Ps(ProtoPsOptions);
        }

        else if (command == "rm") {
            NMessages::TRmOptions ProtoRmOptions;
            ProtoRmOptions = TConverter::ConvertRmOptions(mainSettings.RmOptions, ProtoRmOptions);
            Controller.Rm(ProtoRmOptions);
        }

        else if (command == "exec") {
            NMessages::TExecOptions ProtoExecOptions;
            ProtoExecOptions = TConverter::ConvertExecOptions(mainSettings.ExecOptions, ProtoExecOptions);
            Controller.Exec(ProtoExecOptions);
        }

        else if (command == "attach") {
            NMessages::TAttachOptions ProtoAttachOptions;
            ProtoAttachOptions = TConverter::ConvertAttachOptions(mainSettings.AttachOptions, ProtoAttachOptions);
            //controller handle call
        }

        else {
            throw std::runtime_error("Unknown command");
        }

        
    }

    string GetReadFileResult(const string& path) {
        auto [result, status] = NOs::ReadFile(path);
        auto [statusCode, errorMsg] = status;
        if (statusCode == NCommon::TStatus::ECode::Failed) {
            throw std::runtime_error("Error: " + errorMsg);
        }
        return result;
    }

    string TConverter::ReadDasbootFile(const string& path) {
        std::ifstream DasbootFile(path);
        nlohmann::json jsonDasbootFile, resultJson;
        std::vector<string> CopyFile;

        try {
            jsonDasbootFile = nlohmann::json::parse(DasbootFile);
        } catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + string(e.what()));
        } catch (const std::exception& e) {
            throw std::runtime_error("Error reading JSON: " + string(e.what()));
        }

        if (jsonDasbootFile.contains("network")) {
            if (!jsonDasbootFile["network"].is_boolean()) {
                throw std::runtime_error("Field 'network' must be boolean (true or false)");
            }
            else {
                resultJson["network"] = jsonDasbootFile["network"];
            }
        }

        if (jsonDasbootFile.contains("script_file") && !jsonDasbootFile["script_file"].is_null()) {
            if (jsonDasbootFile["script_file"].is_string()) {
                string result = GetReadFileResult(jsonDasbootFile["script_file"]);
                resultJson["script_code"] = result;
            } 
        }

        if (jsonDasbootFile.contains("copy_file") && !jsonDasbootFile["copy_file"].is_null()) {
            if (jsonDasbootFile["copy_file"].is_string()) {
                string result = GetReadFileResult(jsonDasbootFile["copy_file"]);
                CopyFile.push_back(result);
            } 
            else if (jsonDasbootFile["copy_file"].is_array()) {
                for (const auto& CodePath : jsonDasbootFile["copy_file"]) {
                    if (CodePath.is_string()) {
                        string result = GetReadFileResult(CodePath);
                        CopyFile.push_back(result);
                    } else {
                        throw std::runtime_error("Error: non-string element in copy_file array");
                    }
                }
            }
            resultJson["copy_file"] = CopyFile;
        }

        return resultJson.dump();
    }

    string TConverter::ReadExecFile(const string& path, const bool& isInteractive) {
        std::ifstream ExecFile(path);
        nlohmann::json jsonExecFile, resultJson;
        string pathToScript, pathToCopyFile;
        std::vector<string> CopyFile;
        
        try {
            jsonExecFile = nlohmann::json::parse(ExecFile);
        } catch (const nlohmann::json::parse_error& e) {
            throw std::runtime_error("JSON parse error: " + string(e.what()));
        } catch (const std::exception& e) {
            throw std::runtime_error("Error reading JSON: " + string(e.what()));
        }

        if (jsonExecFile.contains("network")) {
            if (!jsonExecFile["network"].is_boolean()) {
                throw std::runtime_error("Field 'network' must be boolean (true or false)");
            }
            else {
                resultJson["network"] = jsonExecFile["network"];
            }
        }

        if (jsonExecFile.contains("copy_file")) {
            if (jsonExecFile["copy_file"].is_string()) {
                string result = GetReadFileResult(jsonExecFile["copy_file"]);
                resultJson["copy_file"] = result;
            } 
            else if (jsonExecFile["copy_file"].is_array()) {
                for (const auto& CodePath : jsonExecFile["copy_file"]) {
                    if (CodePath.is_string()) {
                        string result = GetReadFileResult(CodePath);
                        CopyFile.push_back(result);
                    } else {
                        throw std::runtime_error("Error: non-string element in copy_file array");
                    }
                }
                resultJson["copy_file"] = CopyFile;
            }
        }

        if (jsonExecFile.contains("script_file")) {
            if (!isInteractive) {
                string result = GetReadFileResult(jsonExecFile["script_file"]);
                resultJson["script_code"] = result;
            } 
            else {
                throw std::runtime_error("When --interactive flag is used, 'script_file' field must not be present");
            }
        } else if (!jsonExecFile.contains("script_file") && !isInteractive) {
            throw std::runtime_error("There must be 'script_file' field");
        }

        return resultJson.dump();
    }
    
    NMessages::TBuildOptions TConverter::ConvertBuildOptions(const NCli::TBuildOptions& options, NMessages::TBuildOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.PathToDasbootFile.has_value()) {
            string DasbootFile = ReadDasbootFile(options.PathToDasbootFile.value());
            protoOptions.set_dasboot_file(DasbootFile);
        }
        else {
            throw std::runtime_error("Path to DasbootFile must be specified");
        }
        return protoOptions;
    }

    NMessages::TRunOptions TConverter::ConvertRunOptions(const NCli::TRunOptions& options, NMessages::TRunOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        return protoOptions;
    }

    NMessages::TStartOptions TConverter::ConvertStartOptions(const NCli::TStartOptions& options, NMessages::TStartOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.Id.has_value()) {
            protoOptions.set_id(options.Id.value());
        }

        return protoOptions;
    }
    
    NMessages::TStopOptions TConverter::ConvertStopOptions(const NCli::TStopOptions& options, NMessages::TStopOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.Id.has_value()) {
            protoOptions.set_id(options.Id.value());
        }

        return protoOptions;
    }
    
    NMessages::TPsOptions TConverter::ConvertPsOptions(const NCli::TPsOptions& options, NMessages::TPsOptions& protoOptions) {
        if (options.ShowAll) {
            protoOptions.set_show_all(options.ShowAll);
        }

        return protoOptions;
    }
    
    NMessages::TRmOptions TConverter::ConvertRmOptions(const NCli::TRmOptions& options, NMessages::TRmOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.Id.has_value()) {
            protoOptions.set_id(options.Id.value());
        }

        return protoOptions;
    }
    
    NMessages::TExecOptions TConverter::ConvertExecOptions(const NCli::TExecOptions& options, NMessages::TExecOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.Id.has_value()) {
            protoOptions.set_id(options.Id.value());
        }

        if (options.IsInteractive) {
            if (options.ExecFile.has_value()) {
                string result = ReadExecFile(options.ExecFile.value(), true);
                protoOptions.set_exec_file(result);
            }
            protoOptions.set_is_interactive(options.IsInteractive);
        }
        else if (options.ExecFile.has_value()) {
            string result = ReadExecFile(options.ExecFile.value(), false);
            protoOptions.set_exec_file(result);
        }
        else {
            throw std::runtime_error("You forgot flag --interactive or ExecFile");
        }

        if (options.Detach) {
            protoOptions.set_detach(options.Detach);
        }

        return protoOptions;
    }
    
    NMessages::TAttachOptions TConverter::ConvertAttachOptions(const NCli::TAttachOptions& options, NMessages::TAttachOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.Id.has_value()) {
            protoOptions.set_id(options.Id.value());
        }

        if (options.NoStdin) {
            protoOptions.set_nostdin(options.NoStdin);
        }

        return protoOptions;
    }

    std::unique_ptr<TParser> MakeDasbootParser(TMainSettings& settings) {
        std::unique_ptr<TParser> parser(new TParser{DasbootDescription});
        parser->RegisterCommands(settings);
        return parser;
    }
}; // namespace NCli

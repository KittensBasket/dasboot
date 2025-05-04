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
        static const std::string DasbootDescription = "A small containerization utility, written in C/C++. Made as team pet project.";
    
        // Common option descriptions
        static const std::string ContainerNameDescription = "Container name";
        static const std::string ContainerIdDescription = "Container ID";
        static const std::string BuildFromFileDescription = "Create a container from a DasbootFile";
        static const std::string ShowAllContainersDescription = "List all containers, including stopped ones.";
        static const std::string DetachFlagDescription = "Detached mode: run command in the background";
        static const std::string NoStdinFlagDescription = "Do not attach STDIN";
    
        // Command descriptions
        static const std::string VersionDescription = "Print version information and quit";
        static const std::string InfoDescription = "Display system-wide information";
        static const std::string BuildDescription = "Create a container";
        static const std::string RunDescription = "Run container in interactive mode";
        static const std::string StartDescription = "Launch a container by name or ID depending on specified options";
        static const std::string StopDescription = "Terminate a running container";
        static const std::string PsDescription = "Display all available containers.";
        static const std::string RmDescription = "Delete a container by name or ID depending on specified options";
        static const std::string ExecDescription = "Run one or multiple commands inside an already running container";
        static const std::string AttachDescription = "Connect a terminal to a running container by its ID or name for interactive management and output monitoring";
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
        AddLocalFlag("exec", "-d", "--detach", mainSettings.ExecOptions.Detach, DetachFlagDescription);

        AddGlobalCommand("attach", AttachDescription);
        AddLocalOption("attach", "-n", "--name", mainSettings.AttachOptions.Name, ContainerNameDescription);
        AddLocalOption("attach", "-i", "--id", mainSettings.AttachOptions.Id, ContainerIdDescription);
        AddLocalFlag("attach", "", "--no-stdin", mainSettings.AttachOptions.NoStdin, NoStdinFlagDescription);
    }

    void TConverter::SendMainSettings(const TMainSettings& mainSettings, const std::string& command) {
        if (mainSettings.Version.PrintVersion) {
            //print version -- function in controller
        }

        if (command == "info") {
            //controller handle call
        }

        if (command == "build") {
            NMessages::TBuildOptions ProtoBuildOptions;
            ProtoBuildOptions = TConverter::ConvertBuildOptions(mainSettings.BuildOptions, ProtoBuildOptions);
            //controller handle call
        } 

        if (command == "run") {
            NMessages::TRunOptions ProtoRunOptions;
            ProtoRunOptions = TConverter::ConvertRunOptions(mainSettings.RunOptions, ProtoRunOptions);
            //controller handle call
        } 

        if (command == "start") {
            NMessages::TStartOptions ProtoStartOptions;
            ProtoStartOptions = TConverter::ConvertStartOptions(mainSettings.StartOptions, ProtoStartOptions);
            //controller handle call
        }

        if (command == "stop") {
            NMessages::TStopOptions ProtoStopOptions;
            ProtoStopOptions = TConverter::ConvertStopOptions(mainSettings.StopOptions, ProtoStopOptions);
            //controller handle call
        }

        if (command == "ps") {
            NMessages::TPsOptions ProtoPsOptions;
            ProtoPsOptions = TConverter::ConvertPsOptions(mainSettings.PsOptions, ProtoPsOptions);
            //controller handle call
        }

        if (command == "rm") {
            NMessages::TRmOptions ProtoRmOptions;
            ProtoRmOptions = TConverter::ConvertRmOptions(mainSettings.RmOptions, ProtoRmOptions);
            //controller handle call
        }

        if (command == "exec") {
            NMessages::TExecOptions ProtoExecOptions;
            ProtoExecOptions = TConverter::ConvertExecOptions(mainSettings.ExecOptions, ProtoExecOptions);
            //controller handle call
        }

        if (command == "attach") {
            NMessages::TAttachOptions ProtoAttachOptions;
            ProtoAttachOptions = TConverter::ConvertAttachOptions(mainSettings.AttachOptions, ProtoAttachOptions);
            //controller handle call
        }
    }

    NMessages::TBuildOptions TConverter::ConvertBuildOptions(const NCli::TBuildOptions& options, NMessages::TBuildOptions& protoOptions) {
        if (options.Name.has_value()) {
            protoOptions.set_name(options.Name.value());
        }

        if (options.PathToDasbootFile.has_value()) {
            protoOptions.set_pathtodasbootfile(options.PathToDasbootFile.value());
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

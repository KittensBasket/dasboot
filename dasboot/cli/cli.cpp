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

    CLI::App* TParser::AddGlobalCommand(const string& commandName, const string& description){
        Commands[commandName] = App.add_subcommand(commandName, description);
        return Commands[commandName];
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


    int TParser::Parse(int argc, char* argv[]) const{
        CLI11_PARSE(App, argc, argv);
        return 0;
    }

    
    string TParser::GetHelp() const {
        return App.help();
    }


    // Converter for converting parsed command-line strings to protobuf messages, which are then sent to controller handlers.
    
    void TConverter::ConvertToProtobuf(const TMainSettings& mainSettings) {
        NMessages::TBuildOptions ProtoBuildOptions;
        NMessages::TRunOptions ProtoRunOptions;
        NMessages::TStartOptions ProtoStartOptions;
        NMessages::TStopOptions ProtoStopOptions;
        NMessages::TPsOptions ProtoPsOptions;
        NMessages::TRmOptions ProtoRmOptions;
        NMessages::TExecOptions ProtoExecOptions;
        NMessages::TAttachOptions ProtoAttachOptions;

        if (mainSettings.Version.printVersion) {
            //print version -- function in controller
        }
        if (CommandCallback(mainSettings.Info.isCalled)) {
            //controller handle call
        }
        if (ConvertRunOptions(mainSettings.RunOptions, ProtoRunOptions) || CommandCallback(mainSettings.RunOptions.isCalled)) {
            //controller handle call
        } 
        if (ConvertBuildOptions(mainSettings.BuildOptions, ProtoBuildOptions) || CommandCallback(mainSettings.BuildOptions.isCalled)) {
            //controller handle call
        } 
        if (ConvertStartOptions(mainSettings.StartOptions, ProtoStartOptions) || CommandCallback(mainSettings.StartOptions.isCalled)) {
            //controller handle call
        }
        if (ConvertStopOptions(mainSettings.StopOptions, ProtoStopOptions) || CommandCallback(mainSettings.StopOptions.isCalled)) {
            //controller handle call
        }
        if (ConvertPsOptions(mainSettings.PsOptions, ProtoPsOptions) || CommandCallback(mainSettings.PsOptions.isCalled)) {
            //controller handle call
        }
        if (ConvertRmOptions(mainSettings.RmOptions, ProtoRmOptions) || CommandCallback(mainSettings.RmOptions.isCalled)) {
            //controller handle call
        }
        if (ConvertExecOptions(mainSettings.ExecOptions, ProtoExecOptions) || CommandCallback(mainSettings.ExecOptions.isCalled)) {
            //controller handle call
        }
        if (ConvertAttachOptions(mainSettings.AttachOptions, ProtoAttachOptions) || CommandCallback(mainSettings.AttachOptions.isCalled)) {
            //controller handle call
        }
    }


    bool TConverter::CommandCallback(const CLI::App* command) {
        return (command->parsed() && !command->get_help_ptr()->as<bool>());
    }

    bool TConverter::ConvertBuildOptions(const NCli::TBuildOptions& options, NMessages::TBuildOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.pathtodasbootfile.has_value()) {
            protoOptions.set_pathtodasbootfile(options.pathtodasbootfile.value());
        }
        return (protoOptions.has_name() || protoOptions.has_pathtodasbootfile());
    }

    bool TConverter::ConvertRunOptions(const NCli::TRunOptions& options, NMessages::TRunOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        return protoOptions.has_name();
    }

    bool TConverter::ConvertStartOptions(const NCli::TStartOptions& options, NMessages::TStartOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.id.has_value()) {
            protoOptions.set_id(options.id.value());
        }
        return (protoOptions.has_name() || protoOptions.has_id());
    }
    
    bool TConverter::ConvertStopOptions(const NCli::TStopOptions& options, NMessages::TStopOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.id.has_value()) {
            protoOptions.set_id(options.id.value());
        }
        return (protoOptions.has_name() || protoOptions.has_id());
    }
    
    bool TConverter::ConvertPsOptions(const NCli::TPsOptions& options, NMessages::TPsOptions& protoOptions) {
        if (options.showAll) {
            protoOptions.set_show_all(options.showAll);
        }
        return protoOptions.has_show_all();
    }
    
    bool TConverter::ConvertRmOptions(const NCli::TRmOptions& options, NMessages::TRmOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.id.has_value()) {
            protoOptions.set_id(options.id.value());
        }
        return (protoOptions.has_name() || protoOptions.has_id());
    }
    
    bool TConverter::ConvertExecOptions(const NCli::TExecOptions& options, NMessages::TExecOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.id.has_value()) {
            protoOptions.set_id(options.id.value());
        }
        if (options.detach) {
            protoOptions.set_detach(options.detach);
        }
        return (protoOptions.has_name() || protoOptions.has_id() || protoOptions.has_detach());
    }
    
    bool TConverter::ConvertAttachOptions(const NCli::TAttachOptions& options, NMessages::TAttachOptions& protoOptions) {
        if (options.name.has_value()) {
            protoOptions.set_name(options.name.value());
        }
        if (options.id.has_value()) {
            protoOptions.set_id(options.id.value());
        }
        if (options.no_stdin) {
            protoOptions.set_nostdin(options.no_stdin);
        }
        return (protoOptions.has_name() || protoOptions.has_id() || protoOptions.has_nostdin());
    }




    // There will be constant values with dasboot's descriptions
    namespace {
        static const std::string DasbootDescription = "A small containerization utility, written in C/C++. Made as team pet project.";
    } // anonymous namespace


    std::unique_ptr<TParser> MakeDasbootParser([[maybe_unused]] TMainSettings& settings) {
        std::unique_ptr<TParser> parser(new TParser{DasbootDescription});
        return parser;
    }
}; // namespace NCli

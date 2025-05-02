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



    // void TParser::AddGlobalOption(const string& shortName, const string& longName, string value, const string& description) {
    //     App.add_option(BuildFullName(shortName, longName), value, description);
    // }

    // void TParser::AddGlobalOption(const string& shortName, const string& longName, string value, const string& description, CLI::Validator&& validator) {
    //     App.add_option(BuildFullName(shortName, longName), value, description)->transform(std::move(validator));
    // }



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

    // void TParser::AddLocalOption(const string& commandName, const string& shortName, const string& longName, string value, const string& description) {
    //     Commands[commandName]->add_option(BuildFullName(shortName, longName), value, description);
    // }

    // void TParser::AddLocalOption(const string& commandName, const string& shortName, const string& longName, string value, const string& description, CLI::Validator&& validator) {
    //     Commands[commandName]->add_option(BuildFullName(shortName, longName), value, description)->transform(validator);
    // }


    int TParser::Parse(int argc, char* argv[]) const{
        CLI11_PARSE(App, argc, argv);
        // std::cout << App.config_to_str(true, true) << std::endl;
        return 0;
    }

    string TParser::GetHelp() const {
        return App.help();
    }


    // There will be constant values with dasboot's descriptions
    namespace {
        static const std::string DasbootDescription = "A small containerization utility, written in C/C++. Made as team pet project.";
    } // anonymous namespace


    std::unique_ptr<const TParser> MakeDasbootParser([[maybe_unused]] TMainSettings& settings) {
        std::unique_ptr<TParser> parser(new TParser{DasbootDescription});
        return parser;
    }
}; // namespace NCli

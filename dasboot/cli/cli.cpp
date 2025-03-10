#include <dasboot/cli/cli.hpp>

namespace NCli {
    using std::string;

    TApp::TApp()
    : App()
    {}

    TApp::TApp(const string& description)
    : App(description)
    {}

    void TApp::AddOption(const string& shortName, const string& longName, string& value, const string& description) {
        string name = "-" + shortName + ",--" + longName;
        App.add_option(name, value, description);
    }

    int TApp::Parse(int argc, char* argv[]) {
        CLI11_PARSE(App, argc, argv);
        return 0;
    }
};

#pragma once

#include <CLI/CLI.hpp>
#include <iostream>

namespace NCli {
    using std::string;

    class TApp {
    private:
        CLI::App App;
    public:
        TApp();
        TApp(const string& description);

        void AddOption(const string& shortName, const string& longName, string& value, const string& description);
        int Parse(int argc, char* argv[]);
    };
};

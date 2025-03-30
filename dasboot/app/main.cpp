#include <iostream>
#include <vector>

#include <dasboot/cli/cli.hpp>

int main(int argc, char* argv[]) {
    NCli::TMainSettings settings;
    auto parser = NCli::MakeDasbootParser(settings);
    parser->Parse(argc, argv);
    return 0;
}

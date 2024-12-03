#include <iostream>
#include <vector>

#include <cli/cli.hpp>

int main(int argc, char** argv) {
    std::vector<std::string> args(argc - 1);

    for (size_t i = 1; i < argc; i++) {
        args[i] = std::move(argv[i]);
    }

	return 0;
}

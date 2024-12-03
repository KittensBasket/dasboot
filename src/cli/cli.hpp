#pragma once

#include <baseinterfaces.hpp>

#include <vector>
#include <iostream>
#include <functional>
#include <optional>

namespace NCli {

using TArgs = std::vector<std::string>;
using TError = std::optional<std::string>;

struct TFlag {
    char shortName;
    std::string fullName;
};

struct TOption {
    char shortName;
    std::string fullName;
    std::string value;  
};

struct TMainSettings {
    // some fields here
};

class TCommand {
public:
    TCommand(char shortName, std::string fullName);

    std::string GetName();

    void AddFlag(TFlag&&);
    void AddOption(TOption&&);
    void Handle(TArgs&&, TMainSettings&);
};

class TCliHandler {
private:
    TMainSettings settings;
public:
    void AddFlag(TFlag&&);
    void AddComand(TCommand&&);
    TError Handle(TArgs&&);
};

} // namespace NCli

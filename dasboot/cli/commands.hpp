#pragma once

#include <iostream>
#include <optional>
#include <unordered_map>

#include <dasboot/cli/cli.hpp>

void RegisterCommands(NCli::TParser& parser, NCli::TMainSettings& mainSettings);

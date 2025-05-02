#include <dasboot/cli/commands.hpp>

// void RegisterCommands(NCli::TParser& parser, TMainSettings& mainSettings) {
//     parser.AddGlobalCommand("run", "Run container in interactive mode");
//     parser.AddLocalOption("run", "-n", "--name", mainSettings.RunSettings.get_mutable_name(), "Container name");
// }

void RegisterCommands(NCli::TParser& parser, NCli::TMainSettings& mainSettings) {
// void RegisterCommands(NCli::TParser& parser) {
    
    
    // // dasboot --version
    // parser.AddGlobalFlag("-v", "--version", flag, "Print version information and quit");

    // // Command 'info'
    // parser.AddGlobalCommand("info", "Display system-wide information"); // What if we don't use -f/--file flag?

    // // Command 'build'
    // parser.AddGlobalCommand("build", "Creates a container"); // What if we don't use -f/--file flag?
    // parser.AddLocalOption("build", "-f", "--file", string, "Creates a container from a DasbootFile");

    // // Command 'run' done
    // parser.AddGlobalCommand("run", "Run container in interactive mode");
    // parser.AddLocalOption("run", "-n", "--name", string, "Container name");

    // Command 'start' done
    // std::optional<std::string> name_opt = *mainSettings.StartOptions.mutable_name();
    // std::optional<std::string> id_opt = *mainSettings.StartOptions.mutable_id();
    parser.AddGlobalCommand("start", "Launches a container by name or ID depending on specified options");
    parser.AddLocalOption("start", "-n", "--name",  *mainSettings.StartOptions.mutable_name() , "Container name");
    parser.AddLocalOption("start", "-i", "--id",  *mainSettings.StartOptions.mutable_id() , "Container ID");
    // parser.AddLocalOption("start", "-n", "--name",  name_opt, "Container name");
    // parser.AddLocalOption("start", "-i", "--id",  id_opt, "Container ID");

    // // Command 'stop' done 
    // parser.AddGlobalCommand("stop", "Terminates a running container");
    // parser.AddLocalOption("stop", "-n", "--name", string, "Container name");
    // parser.AddLocalOption("stop", "-i", "--id", string, "Container ID");

    // // Command 'ps' done
    // parser.AddGlobalCommand("ps", "Displays all available containers.");
    // parser.AddLocalFlag("ps", "-a", "--all", flag, "Lists all containers, including stopped ones."); // add --all in cli rfs

    // // Command 'rm' done
    // parser.AddGlobalCommand("rm", "Deletes a container by name or ID depending on specified options");
    // parser.AddLocalOption("rm", "-n", "--name", string, "Container name");
    // parser.AddLocalOption("rm", "-i", "--id", string, "Container ID");

    // // Command 'exec' done
    // parser.AddGlobalCommand("exec", "Runs one or multiple commands inside an already running container");
    // parser.AddLocalOption("exec", "-n", "--name", string, "Container name");
    // parser.AddLocalOption("exec", "-i", "--id", string, "Container ID");
    // parser.AddLocalFlag("exec", "-d", "--detach", flag, "Detached mode: run command in the background"); // add --detach in cli rfs

    // // Command 'attach' done
    // parser.AddGlobalCommand("attach", "Connects a terminal to a running container by its ID or name for interactive management and output monitoring");
    // parser.AddLocalOption("attach", "-n", "--name", string, "Container name");
    // parser.AddLocalOption("attach", "-i", "--id", string, "Container ID");
    // parser.AddLocalFlag("attach", "", "--no-stdin", flag, "Do not attach STDIN");

}
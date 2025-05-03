#include <dasboot/cli/commands.hpp>

void RegisterCommands(NCli::TParser& parser, NCli::TMainSettings& mainSettings) {
    // // dasboot --version
    parser.AddGlobalFlag("-v", "--version", mainSettings.Version.printVersion, "Print version information and quit");

    // Command 'info'
    mainSettings.Info.isCalled = parser.AddGlobalCommand("info", "Display system-wide information");

    // Command 'build'
    mainSettings.BuildOptions.isCalled = parser.AddGlobalCommand("build", "Creates a container"); // What if we don't use -f/--file flag?
    parser.AddLocalOption("build", "-n", "--name", mainSettings.BuildOptions.name, "Container name");
    parser.AddLocalOption("build", "-f", "--file", mainSettings.BuildOptions.name, "Creates a container from a DasbootFile");

    // Command 'run'
    mainSettings.RunOptions.isCalled = parser.AddGlobalCommand("run", "Run container in interactive mode");
    parser.AddLocalOption("run", "-n", "--name", mainSettings.RunOptions.name, "Container name");

    // Command 'start'
    mainSettings.StartOptions.isCalled = parser.AddGlobalCommand("start", "Launches a container by name or ID depending on specified options");
    parser.AddLocalOption("start", "-n", "--name",  mainSettings.StartOptions.name, "Container name");
    parser.AddLocalOption("start", "-i", "--id",  mainSettings.StartOptions.id, "Container ID");

    // Command 'stop'
    mainSettings.StopOptions.isCalled = parser.AddGlobalCommand("stop", "Terminates a running container");
    parser.AddLocalOption("stop", "-n", "--name", mainSettings.StopOptions.name, "Container name");
    parser.AddLocalOption("stop", "-i", "--id", mainSettings.StopOptions.id, "Container ID");

    // Command 'ps'
    mainSettings.PsOptions.isCalled = parser.AddGlobalCommand("ps", "Displays all available containers.");
    parser.AddLocalFlag("ps", "-a", "--all", mainSettings.PsOptions.showAll, "Lists all containers, including stopped ones."); // add --all in cli rfs

    // Command 'rm'
    mainSettings.RmOptions.isCalled = parser.AddGlobalCommand("rm", "Deletes a container by name or ID depending on specified options");
    parser.AddLocalOption("rm", "-n", "--name", mainSettings.RmOptions.name, "Container name");
    parser.AddLocalOption("rm", "-i", "--id", mainSettings.RmOptions.id, "Container ID");

    // Command 'exec'
    mainSettings.ExecOptions.isCalled = parser.AddGlobalCommand("exec", "Runs one or multiple commands inside an already running container");
    parser.AddLocalOption("exec", "-n", "--name", mainSettings.ExecOptions.name, "Container name");
    parser.AddLocalOption("exec", "-i", "--id", mainSettings.ExecOptions.id, "Container ID");
    parser.AddLocalFlag("exec", "-d", "--detach", mainSettings.ExecOptions.detach, "Detached mode: run command in the background"); // add --detach in cli rfs

    // Command 'attach'
    mainSettings.AttachOptions.isCalled = parser.AddGlobalCommand("attach", "Connects a terminal to a running container by its ID or name for interactive management and output monitoring");
    parser.AddLocalOption("attach", "-n", "--name", mainSettings.AttachOptions.name, "Container name");
    parser.AddLocalOption("attach", "-i", "--id", mainSettings.AttachOptions.id, "Container ID");
    parser.AddLocalFlag("attach", "", "--no-stdin", mainSettings.AttachOptions.no_stdin, "Do not attach STDIN");

}
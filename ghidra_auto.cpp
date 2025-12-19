/*
 * Add analyzeHeadless to PATH
 * Its in /opt/ghidra/support/analyzeHeadless
 * ~ ❯  export PATH="/opt/ghidra/support:$PATH"
 * This is how it should look like
 * ~ ❯  which analyzeHeadless
 *      /opt/ghidra/support/analyzeHeadless
 * also u need mkdir -p $HOME/<User>/GhidraProjects
 */


#include <string>
#include <filesystem>
#include <cstdio>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace chrono;

void print_help(const char *prog) {
    std::printf(
        "Usage: %s <binary>\n"
        "\n"
        "Imports <binary> into a new Ghidra project under $HOME/GhidraProjects/<name>_<timestamp>\n"
        "and then opens the project in the Ghidra GUI.\n"
        "\n"
        "Requirements:\n"
        "  - analyzeHeadless and ghidra available in PATH (see header for PATH example)\n"
        "  - $HOME set (falls back to ./GhidraProjects if not)\n",
        prog
    );
}

int main(int argc, char **argv) {
    if (argc < 2) {
        print_help(argv[0]);
        return 1;
    }

    string inputArg = argv[1];
    if (inputArg == "-h" || inputArg == "--help") {
        print_help(argv[0]);
        return 0;
    }

    // Get the current time point from the system clock
    auto now = system_clock::now();
    auto ts  = duration_cast<seconds>(now.time_since_epoch()).count();

    const char *home  = std::getenv("HOME");
    filesystem::path baseDir = home && *home
        ? filesystem::path(home) / "GhidraProjects"
        : filesystem::path("GhidraProjects");

    string projectName= filesystem::path(argv[1]).stem().string();
    string projectDir = (baseDir / (projectName + "_" + to_string(ts))).string();
    string gprPath    = (filesystem::path(projectDir) / (projectName + ".gpr")).string();

    // Make sure the target folder exists before kicking off Ghidra
    filesystem::create_directories(projectDir);

    char cmd[1024];

    snprintf(
        cmd, sizeof(cmd),
        "analyzeHeadless \"%s\" \"%s\" -import \"%s\" && ghidra \"%s\"",
        projectDir.c_str(),   // folder in ~/GhidraProjects
        projectName.c_str(),  // project name
        argv[1],              // binary to import
        gprPath.c_str()       // open the created project in GUI
    );


    printf("Project saved as: %s\n", gprPath.c_str());

    return std::system(cmd);
}

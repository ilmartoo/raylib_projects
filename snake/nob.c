#define NOB_IMPLEMENTATION
#include "nob.h"

#define EXECUTABLE_NAME "navecitas"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define LIB_FOLDER   "lib/"

void nob_base(Nob_Cmd* cmd) {
    nob_cc(cmd);        // cc
    nob_cc_flags(cmd);  // -Wall -Wextra
    nob_cmd_append(cmd, "-I" SRC_FOLDER);
    nob_cc_inputs(cmd,
                  SRC_FOLDER "main.c",                // entrypoint
                  SRC_FOLDER "input/input-handler.c"  // input handler
    );
    nob_cmd_append(cmd, "-L" LIB_FOLDER, "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lm");
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

    nob_base(&cmd);
    nob_cmd_append(&cmd, "-O3");
    nob_cc_output(&cmd, BUILD_FOLDER EXECUTABLE_NAME "_release");

    if (!nob_cmd_run(&cmd)) return 1;

    nob_base(&cmd);
    nob_cmd_append(&cmd, "-g", "-DDEBUG");
    nob_cc_output(&cmd, BUILD_FOLDER EXECUTABLE_NAME "_debug");

    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

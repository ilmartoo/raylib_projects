#define NOB_IMPLEMENTATION
#include "nob.h"

#define EXECUTABLE_NAME "input-handler"

#define BUILD_FOLDER "build/"
#define SRC_FOLDER   "src/"
#define LIB_FOLDER   "lib/"

void nob_build_common(Nob_Cmd* cmd) {
    nob_cc(cmd);        // cc
    nob_cc_flags(cmd);  // -Wall -Wextra
    nob_cmd_append(cmd, "-I" SRC_FOLDER);
    nob_cc_inputs(cmd,
                  SRC_FOLDER "device-tester.c",       // entrypoint
                  SRC_FOLDER "types/float16.c",       // float16 implementation
                  SRC_FOLDER "input/input-handler.c"  // input handler
    );
    nob_cmd_append(cmd, "-L" LIB_FOLDER, "-lraylib", "-lopengl32", "-lgdi32", "-lwinmm", "-lm");
}

void nob_build_release(Nob_Cmd* cmd) {
    nob_build_common(cmd);
    nob_cmd_append(cmd, "-O3");
    nob_cc_output(cmd, BUILD_FOLDER EXECUTABLE_NAME "_release");
    if (!nob_cmd_run(cmd)) exit(1);
}

void nob_build_debug(Nob_Cmd* cmd) {
    nob_build_common(cmd);
    nob_cmd_append(cmd, "-g", "-DDEBUG");
    nob_cc_output(cmd, BUILD_FOLDER EXECUTABLE_NAME "_debug");
    if (!nob_cmd_run(cmd)) exit(1);
}

int main(int argc, char** argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};

    nob_build_release(&cmd);
    nob_build_debug(&cmd);

    return 0;
}

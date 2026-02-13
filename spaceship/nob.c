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
                  SRC_FOLDER "main.c",                  // entrypoint
                  SRC_FOLDER "types/arena.c"            // arena
                  SRC_FOLDER "types/float16.c"          // float16
                  SRC_FOLDER "types/object_pool.c"      // object pool
                  SRC_FOLDER "types/arena.c"            // arena
                  SRC_FOLDER "utils/extra_math.c"       // extra raymath
                  SRC_FOLDER "utils/memory_utils.c"     // memory utilities
                  SRC_FOLDER "input/input-handler.c"    // input handler
                  SRC_FOLDER "entities/collisions.c"    // collisions
                  SRC_FOLDER "entities/entities.c"      // entities
                  SRC_FOLDER "abilities/abilites.c"     // abilities
                  SRC_FOLDER "lifecycles/game_clear.c"  // game clearing
                  SRC_FOLDER "lifecycles/game_draw.c"   // game drawing
                  SRC_FOLDER "lifecycles/game_frame.c"  // game frame logic
                  SRC_FOLDER "lifecycles/game_init.c"   // game initialization
                  SRC_FOLDER "lifecycles/game_loop.c"   // game loop
                  SRC_FOLDER "lifecycles/game_state.c"  // game state
                  SRC_FOLDER "debug/debug_panel.c"      // debug panel
                  SRC_FOLDER "debug/game_debug.c"       // game debug utils
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

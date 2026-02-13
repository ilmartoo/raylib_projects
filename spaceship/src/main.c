#include <stdlib.h>

#include "lifecycles/game_lifecycle.h"
#include "types/types.h"

i32 main(void) {
    GameInitialize();  // Game initialization
    GameLoop();        // Game loop
    GameClear();       // Game cleaning

    return EXIT_SUCCESS;
}

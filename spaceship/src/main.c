#include <stdlib.h>

#include "game_lifecycle.h"
#include "types.h"

i32 main(void)
{
    GameInitialize(); // Game initialization
    GameLoop();       // Game loop
    GameClear();      // Game cleaning

    return EXIT_SUCCESS;
}

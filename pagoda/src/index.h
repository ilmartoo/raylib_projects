#pragma once
#ifndef __ILMARTO_INDEX_H_
#define __ILMARTO_INDEX_H_

#include "types.h"

/* Assets and images utils */
#pragma region :assets

#define ASSETS_DIR "assets/"

#define IMAGES_DIR ASSETS_DIR "images/"
#define GetImage(image_name) IMAGES_DIR image_name

#pragma endregion

/* Display utils */
#pragma region :display :window

#define WINDOW_WIDTH 1980
#define WINDOW_HEIGHT 1080

#define Draw(x)         \
    do {                \
        BeginDrawing(); \
        x               \
        EndDrawing();   \
    } while (0)

#pragma endregion

/* Pagoda & resources */
#pragma region :pagoda :resources

typedef struct Pagoda {
    Resources resources[4];
    u8 offsets[4];
} Pagoda;

typedef enum Resources {
    RESOURCE_NONE = 0, // No resource
    RESOURCE_PLANTS,
    RESOURCE_WATER,
    RESOURCE_STONE,
    RESOURCE_WOOD,
} Resources;

#pragma endregion

/* Panoramas */
#pragma region :panoramas

typedef struct Panorama {
    PanoramaColors color;
    u8 size;
} Panorama;

typedef enum PanoramaColors {
    PANORAMA_COLOR_MAGENTA = 1,
    PANORAMA_COLOR_ORANGE,
    PANORAMA_COLOR_GREEN,
    PANORAMA_COLOR_BLUE
} PanoramaColors;

Panorama PANORAMAS[] = {
    { .color = PANORAMA_COLOR_MAGENTA, .size = 2 },
    { .color = PANORAMA_COLOR_MAGENTA, .size = 3 },
    { .color = PANORAMA_COLOR_MAGENTA, .size = 4 },
    { .color = PANORAMA_COLOR_MAGENTA, .size = 5 },
};

#pragma endregion

/* Cards */
#pragma region :cards

typedef enum CardSides {
    CARD_SIDE_GROUNDWORK = 1,
    CARD_SIDE_LANDSCAPE
} CardSides;

typedef struct Card {
    CardSides side;
    GroundworkData groundwork;
    LandscapeData landscape;
} Card;

typedef struct GroundworkData {
    u8 rotation_floor;
    RetrievalDirection direction;
    Resources resources[4];
} GroundworkData;

typedef enum RetrievalDirection {
    RETRIEVAL_DIRECTION_NONE = 0, // "Take a wild" action
    RETRIEVAL_DIRECTION_BOTTOM_TO_TOP,
    RETRIEVAL_DIRECTION_TOP_TO_BOTTOM,
} RetrievalDirection;

typedef struct LandscapeData {
    PanoramaColors color;
    u32 position;
} LandscapeData;

#pragma endregion

#endif // __ILMARTO_INDEX_H_
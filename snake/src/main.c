#include <math.h>
#include "raylib.h"
#include "types.h"
#include "helpers.h"
#include "debug.h"
#include "arena.h"

#define ASSETS_DIR "assets/"
#define IMAGES_DIR ASSETS_DIR"images/"
#define GetImagePath(image_name) IMAGES_DIR image_name
#define Draw(x) do { BeginDrawing(); { x } EndDrawing(); } while (0)

i32 windowWidth = 1200;
i32 windowHeight = 600;


#pragma region SLAB
    #define SLAB_SIZE 20

    typedef struct Slab {
        Color color;
    } Slab;

    Slab SlabCreate(bool variant) {
        return (Slab){ variant ? ORANGE : YELLOW };
    }

    /**
     * @brief Draws a slab.
     * @param slab Slab to use.
     */
    void SlabDraw(i32 x, i32 y, Slab slab) {
        DrawRectangle(x, y, SLAB_SIZE, SLAB_SIZE, slab.color);
    }
#pragma endregion 

#pragma region MAP 
    typedef struct Map {
        usize width;
        usize height;
        Optional *matrix;
        Slab *slabs;
    } Map;

    /**
     * @brief Creates a map from a slab matrix.
     * @param width Width of the matrix.
     * @param height Height of the matrix.
     * @param slabMatrix Matrix of where to place the slabs.
     * @return New map.
     */
    Map MapCreateFromMatrix(usize width, usize height, bool matrix[]) {
        usize total = width * height;
        usize slabsCount;
        usize idx;
        for (idx = 0, slabsCount = 0; idx < total; ++idx) { if (matrix[idx]) { ++slabsCount; } }
        
        Map map = { width, height, malloc(sizeof(Optional) * total), malloc(sizeof(Slab) * slabsCount)};
        
        for (idx = 0, slabsCount = 0; idx < total; ++idx) {
            if (matrix[idx]) {
                Slab *slab = &map.slabs[slabsCount];
                *slab = SlabCreate((idx + idx / width) & 1 );
                ++slabsCount;
                map.matrix[idx] = (Optional){ true, slab };
            } else {
                map.matrix[idx] = (Optional){ false, NULL };
            }
        }

        return map;
    }

    /**
     * @brief Creates a map.
     * @param width Width of the map.
     * @param height Height of the map.
     * @return New map.
     */
    Map MapCreate(usize width, usize height) {
        usize total = width * height;        
        Map map = { width, height, malloc(sizeof(Optional) * total), malloc(sizeof(Slab) * total)};
        
        for (usize idx = 0; idx < total; ++idx) {
            Slab *slab = &map.slabs[idx];
            *slab = SlabCreate((idx % width + idx / height) & 1 );
            map.matrix[idx] = (Optional){ true, slab };
        }

        return map;
    }

    /**
     * @brief Destroys a map.
     * @param map Map to use.
     */
    void MapDestroy(Map *map) {
        free(map->slabs);
        free(map->matrix);
    }

    /**
     * @brief Draws a map.
     * @param map Map to use.
     */
    void MapDraw(Map map, Vector2 center) {
        i32 xo = center.x - (map.width * SLAB_SIZE) / 2;
        i32 yo = center.y - (map.height * SLAB_SIZE) / 2;
        usize total = map.width * map.height;       
        
        for (usize idx = 0; idx < total; ++idx) {
            Optional opt = map.matrix[idx];
            if (opt.ok) {
                i32 x = SLAB_SIZE * (idx % map.height) + xo;
                i32 y = SLAB_SIZE * (idx / map.width) + yo;
                SlabDraw(x, y, *(Slab *)opt.data);
            }
        }
    }
#pragma endregion 

void DrawFigures(void)
{
    u32 screenWidth = GetScreenWidth();
    u32 halfScreenWidth = screenWidth / 2;
    u32 padding = 25;
    u32 spacing = 50;
    u32 offset = padding;

    // Line
    Vector2 lStart = { padding, offset };
    Vector2 lEnd = { screenWidth - padding, padding };
    u32 thick = 10;
    DrawLineEx(lStart, lEnd, thick, SKYBLUE);

    offset += thick + spacing;

    // Circle
    u32 radius = min(100, halfScreenWidth - padding);
    offset += radius;
    Vector2 center = { halfScreenWidth, offset };
    DrawCircleV(center, radius, PURPLE);

    offset += radius + spacing;

    // Triangle
    u32 tSize = min(200, screenWidth - padding * 2);
    u32 tHalfSize = tSize / 2;
    u32 tHeight = tSize * sqrt(3) / 2;
    Vector2 tp1 = { halfScreenWidth, offset };
    offset += tHeight;
    Vector2 tp2 = { halfScreenWidth - tHalfSize, offset };
    Vector2 tp3 = { halfScreenWidth + tHalfSize, offset };
    DrawTriangle(tp1, tp2, tp3, LIME);
}

int main()
{
#ifdef DEBUG
    string gameTitle = "Snake game - Debug";
#else
    string gameTitle = "Snake game";
#endif

    // Initialization
    InitWindow(windowWidth, windowHeight, gameTitle);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(LoadImage(GetImagePath("snake-icon.png")));

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    Map map = MapCreate(14, 14);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        Vector2 center = { GetScreenWidth() / 2, GetScreenHeight() / 2 };

        Draw(
            ClearBackground(DARKGRAY); // Set background color

            // DrawFigures();
            MapDraw(map, center);
        #ifdef DEBUG
            DrawFPS(10, 10);
        #endif
        );
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}

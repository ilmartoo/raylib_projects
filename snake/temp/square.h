#pragma once
#ifndef _SQUARE_H_
#define _SQUARE_H_

#include "raylib.h"
#include "tuples.h"
#include "types.h"

typedef struct Square {
    TupleU32 pos;
    TupleU32 size;
    Color color;
} Square;

Square* newSquare(TupleU32 pos, TupleU32 size, Color color);

void drawSquare(Square* square);

#endif //_SQUARE_H_

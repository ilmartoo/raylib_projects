#pragma once
#ifndef _PENTAGRAM_H_
#define _PENTAGRAM_H_

#include "raylib.h"
#include "types.h"

typedef struct Pentagram {
    Vector2 pos;
    i32 innerSize;
    i32 outerSize;
} Pentagram;

Pentagram* newPentagram(Vector2 pos, i32 innerSize, i32 outerSize);

void drawPentagram(Pentagram*);

#endif //_PENTAGRAM_H_

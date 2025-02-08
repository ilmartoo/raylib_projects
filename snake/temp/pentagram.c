#include "pentagram.h"

Pentagram* newPentagram(Vector2 pos, i32 innerSize, i32 outerSize)
{
    Pentagram* pentagram = new (Pentagram);
    pentagram->pos = pos;
    pentagram->innerSize = innerSize;
    pentagram->outerSize = outerSize;
    return pentagram;
}

void drawPentagram(Pentagram* pentagram)
{
    i32 halfInnerSize = pentagram->innerSize / 2;
    Vector2 center = {
        pentagram->pos.x - halfInnerSize,
        pentagram->pos.y - halfInnerSize
    };
    DrawSplineLinear()
}
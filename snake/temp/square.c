#include "square.h"

Square* newSquare(TupleU32 pos, TupleU32 size, Color color)
{
    Square* square = new (Square);
    square->pos = pos;
    square->size = size;
    square->color = color;
    return square;
}

void drawSquare(Square* square)
{
}
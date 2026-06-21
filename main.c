#include <stdio.h>
#include <stdbool.h>

enum {T_WALL = 0,
      T_GRASS = 1,
      T_TREE = 2,
      T_WATER = 3,
      T_MTN = 4,
      T_DOOR = 5,
    };

struct TileDef {
    char glyph;
    bool solid;
};

TileDef array[6] = {{'#', true},
                    {'.', false},
                    {'T', true},
                    {'~', true},
                    {'^', true},
                    {'+', false}
                }

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
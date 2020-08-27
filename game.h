#ifndef GAME_H
#define GAME_H

#include "gba.h"

                    /* TODO: */

            // Create any necessary structs //


/*
* For example, for a Snake game, one could be:
*
* typedef struct snake {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*
*
*
*
* Example of a struct to hold state machine data:
*
* typedef struct state {
*   int currentState;
*   int nextState;
* } State
*
*/

struct Player {
    volatile u16 playerX;
    volatile u16 playerY;
};

struct Enemy {
    volatile u16 enemyX;
    volatile u16 enemyY;
};

struct SpecialEnemy {
    volatile u16 specialX;
    volatile u16 specialY;
};

#endif

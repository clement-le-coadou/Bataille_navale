#ifndef PRINT_H
    #define PRINT_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <unistd.h>

    #include "player.h"


    /**
     * @brief Prints the player's own board with ships.
     * @param p Pointer to the player.
     */
    void printAllyBoard(Player* p);

    /**
     * @brief Prints the player's enemy board with shots and wrecks.
     * @param p Pointer to the player.
     */
    void printEnemyBoard(Player* p);

#endif
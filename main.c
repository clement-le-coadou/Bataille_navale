/**
 * \author Clement Le Coadou GI2
 * \date 23/12/2023
 *
 * Naval Battle
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "game.h"

// game loop
int main() {
    srand(time(NULL));

    // Initialize the game
    int win = 0;
    Game* game = create_Game();

    // Main game loop
    while (win == 0) {
        // Player's turn
        puts("----- SIR, IT IS OUR TURN TO ATTACK ------");
        puts("Our ships: ");
        printAllyBoard(game->player1);
        puts("\nEnemy ships: ");
        printEnemyBoard(game->computer);
        playerTurn(game);
        sleep(2);

        // Check if the Computer is defeated
        if (is_player_dead(game->computer)) {
            puts("----- SIR, WE JUST WON THIS BATTLE, CONGRATULATIONS ------");
            break;
        }

        // Computer's turn
        puts("Our ships: ");
        printAllyBoard(game->player1);
        sleep(1);
        puts("\nEnemy ships: ");
        printEnemyBoard(game->computer);

        puts("----- SIR, THE ENEMY IS ATTACKING, TAKE COVER! ------");
        sleep(4);
        computerTurn(game);

        // Check if the Player is defeated
        if (is_player_dead(game->player1)) {
            puts("----- YOU DIED ------");
            break;
        }

        // Display the current state of the game
        puts("Our ships: ");
        printAllyBoard(game->player1);
        puts("\nEnemy ships: ");
        printEnemyBoard(game->computer);
        sleep(4);
    }

    // Free memory allocated for the game
    freeGame(game);

    return 0;
}
#include "print.h"  
 


// Function to print the player's own game board
void printAllyBoard(Player* p) {
    if (p == NULL) {
        exit(50); // Exit if player is NULL
    }

    // Print the column numbers
    puts("");
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    puts("");

    // Print the game board with symbols representing different states
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            int cell = p->board.board[i][j];
            switch (cell) {
                case WATER:
                    printf("~ ");
                    break;
                case BOAT:
                    printf("B ");
                    break;
                case WATER_SHOT:
                    printf(". ");
                    break;
                case WRECK:
                    printf("X ");
                    break;
                default:
                    break;
            }
        }
        puts("");
    }

    puts("");
}

// Function to print the enemy player's game board
void printEnemyBoard(Player* p) {
    if (p == NULL) {
        exit(50); // Exit if player is NULL
    }

    puts("");
    printf("  ");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
    }
    puts("");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            int cell = p->board.board[i][j];
            switch (cell) {
                case WATER:
                    printf("~ ");
                    break;
                case WATER_SHOT:
                    printf(". ");
                    break;
                case WRECK:
                    printf("X ");
                    break;
                default:
                    printf("~ ");
                    break;
            }
        }
        puts("");
    }

    puts("");
}
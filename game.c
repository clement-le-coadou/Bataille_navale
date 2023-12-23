#include "game.h"  

// Function to create a Game structure with two players
Game* create_Game() {
    // Allocate memory for a Game structure
    Game* game = malloc(sizeof(Game));
    if (game == NULL) {
        exit(22);
    }

    // Create two players and assign them to the game structure
    game->player1 = create_Player();
    game->computer = create_Player();
    return game;
}

// Function to flush the input buffer
void flush() {
    char c = getchar();
    while (c != '\n') {
        c = getchar();
    }
}

// Function for a player's turn
void playerTurn(Game* game) {
    if (game == NULL) {
        exit(40);
    }
    int choice_X = 0;
    int choice_Y = 0;
    int verif = 0;
    int ok = 1;

    // Input validation for X coordinate
    puts("Captain, please enter X coordinate");
    do {
        verif = scanf("%d", &choice_X);
        if (verif != 1 || choice_X < 0 || choice_X > SIZE - 1) {
            puts("Please enter a valid X coordinate");
            ok = 0;
            flush();
        } else {
            ok = 1;
        }
    } while (ok == 0);

    // Input validation for Y coordinate
    ok = 1;
    puts("Captain, please enter Y coordinate");
    do {
        verif = scanf("%d", &choice_Y);
        if (verif != 1 || choice_Y < 0 || choice_Y > SIZE - 1) {
            puts("Please enter a valid Y coordinate");
            ok = 0;
            flush();
        } else {
            ok = 1;
        }
    } while (ok == 0);

    // Check if the player's shot hits a boat
    if (searchBoat(game->computer, choice_X, choice_Y)) {
        game->computer->board.board[choice_X][choice_Y] = WRECK;
        puts("----------------- HIT! WELL DONE SIR -----------------------");
        printEnemyBoard(game->computer);
    } else {
        // Update the player's board based on the result of the shot
        if (game->computer->board.board[choice_X][choice_Y] == WRECK) {
            game->computer->board.board[choice_X][choice_Y] = WRECK;
        } else {
            game->computer->board.board[choice_X][choice_Y] = WATER_SHOT;
        }
        puts("--------- SIR, WE MISSED THE TARGET, PUT ON SOME GLASSES! ---------");
    }
}

// Function for the computer's turn
void computerTurn(Game* game) {
    if (game == NULL) {
        exit(40);
    }
    int choice_X = 0;
    int choice_Y = 0;

    // Generate random coordinates for the computer's shot
    do {
        choice_X = rand() % SIZE;
        choice_Y = rand() % SIZE;
    } while (game->player1->board.board[choice_X][choice_Y] == WRECK || game->player1->board.board[choice_X][choice_Y] == WATER_SHOT);

    // Check if the computer's shot hits a boat
    if (searchBoat(game->player1, choice_X, choice_Y)) {
        game->player1->board.board[choice_X][choice_Y] = WRECK;
        puts("----------------------- SIR, WE HAVE BEEN HIT -----------------------");
    } else {
        // Update the player's board based on the result of the shot
        if (game->player1->board.board[choice_X][choice_Y] == WRECK) {
            game->player1->board.board[choice_X][choice_Y] = WRECK;
        } else {
            game->player1->board.board[choice_X][choice_Y] = WATER_SHOT;
        }
        puts("---------------------- GOOD NEWS, THEY MISSED ----------------------");
    }
}

// Deallocates memory for the entire game, including both Players.
void freeGame(Game* game) {
    // Free memory for the Computer
    freePlayer(game->computer);

    // Free memory for the Player
    freePlayer(game->player1);

    // Free memory for the game structure
    free(game);
}

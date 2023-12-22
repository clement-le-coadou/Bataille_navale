#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define SIZE 3
#define NB 3

/**
 * @brief Enumeration representing different types of elements.
 */
enum {
    WATER,         /**< Water type */
    WATER_SHOT,    /**< Water shot type */
    BOAT,          /**< Boat type */
    WRECK          /**< Wreck type */
} type;

/**
 * @brief Structure representing the game board.
 */
typedef struct {
    int** board;    /**< 2D array representing the board */
    int size;       /**< Size of the board */
} Board;

/**
 * @brief Structure representing a ship.
 */
typedef struct {
    int size;       /**< Size of the ship */
    int pos[2];     /**< Position of the ship on the board [x, y] */
    int or;         /**< Orientation of the ship: 1 for horizontal and 0 for vertical */
    int* life;      /**< Array representing the life status of ship segments */
} Ship;

/**
 * @brief Structure representing a player.
 */
typedef struct {
    Board board;    /**< Game board for the player */
    Ship** ships;   /**< Array of ships belonging to the player */
    int nb_ships;   /**< Number of ships the player has */
} Player;

/**
 * @brief Structure representing the game.
 */
typedef struct {
    Player* player1; /**< Player 1 */
    Player* computer; /**< Computer */
} Game;

/**
 * @brief Function to create a new ship.
 * @param size Size of the ship.
 * @param pos1 X-coordinate of the ship's position.
 * @param pos2 Y-coordinate of the ship's position.
 * @param or Orientation of the ship: 1 for horizontal and 0 for vertical.
 * @return Pointer to the created ship.
 */
Ship* createShip(int size, int pos1, int pos2, int or);

/**
 * @brief Function to check if a ship is alive.
 * @param ship Pointer to the ship.
 * @return 1 if the ship is alive, 0 otherwise.
 */
int is_alive(Ship* ship);

/**
 * @brief Function to check if a player is dead.
 * @param p Pointer to the player.
 * @return 1 if the player is dead, 0 otherwise.
 */
int is_player_dead(Player* p);

/**
 * @brief Initializes the game board for a player and places ships randomly.
 * @param p Pointer to the player.
 */
void board_init(Player* p);

/**
 * @brief Creates a new player with initialized board and ships.
 * @return Pointer to the created player.
 */
Player* create_Player();

/**
 * @brief Creates a new game with two players.
 * @return Pointer to the created game.
 */
Game* create_Game();

/**
 * @brief Searches for a boat at the specified coordinates and updates the board.
 * @param p Pointer to the player.
 * @param choice_X X-coordinate of the target.
 * @param choice_Y Y-coordinate of the target.
 * @return 1 if a boat is hit, 0 otherwise.
 */
int searchBoat(Player* p, int choice_X, int choice_Y);

/**
 * @brief Clears the input buffer.
 */
void flush();

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

/**
 * @brief Manages the player's turn in the game.
 * @param game Pointer to the game.
 */
void playerTurn(Game* game);

/**
 * @brief Manages the computer's turn in the game.
 * @param game Pointer to the game.
 */
void computerTurn(Game* game);

/**
 * @brief Deallocates memory for a ship, including its life status array.
 * @param ship Pointer to the ship.
 */
void unalloc_ship(Ship* ship);

/**
 * @brief Deallocates memory for the game board.
 * @param p Pointer to the player.
 */
void freeBoard(Player* p);

/**
 * @brief Deallocates memory for a player, including its ships and the game board.
 * @param player Pointer to the player.
 */
void freePlayer(Player* player);

/**
 * @brief Deallocates memory for the game, including both players.
 * @param game Pointer to the game.
 */
void freeGame(Game* game);




// Creates a new ship with the specified size, position, and orientation.
Ship* createShip(int size, int pos1,int pos2, int or){
    puts("test");
    Ship* ship = malloc(sizeof(Ship)); // Allocate memory for the ship
    if(ship==NULL){
        exit(1); // Exit if memory allocation fails
    }
    if(size<0 || pos1<0 || pos2<0 || pos1>SIZE-1 || pos2>SIZE-1 || (or!=1 && or!=0)){
        exit(2);
    }
    ship->size = size;
    ship->pos[0] = pos1;
    ship->pos[1] = pos2;
    ship->or = or;

    int* life = malloc(sizeof(int)*size); // Allocate memory for ship life status
    if(life == NULL){
        exit(3); // Exit if memory allocation fails
    }

    for(int i=0;i<size;i++){
        life[i] = BOAT; // Initialize ship life status
    }
    ship->life = life;
    return ship;
}

// Checks if the given ship is alive.
int is_alive(Ship* ship){
    if(ship == NULL){
        exit(4); // Exit if ship is NULL
    }
    for(int i=0;i<ship->size;i++){
        if(ship->life[i] == BOAT){
            return 1; // Ship is alive
        }
    }
    return 0; // Ship is not alive
}

// Checks if the given player is dead (all ships destroyed).
int is_player_dead(Player* p){
    if(p==NULL){
        exit(5); // Exit if player is NULL
    }
    for(int i=0;i<p->nb_ships;i++){
        if(is_alive(p->ships[i])){
            return 0; // Player is not dead
        }
    }
    return 1; // Player is dead
}


// Initializes the game board for a player.
void board_init(Player* p) {
    if (p == NULL) {
        exit(10); // Exit if player is NULL
    }

    // Allocate memory for the 2D array representing the game board
    p->board.board = malloc(sizeof(int*) * SIZE);
    if (p->board.board == NULL) {
        exit(11);
    }

    // Initialize each cell of the board to WATER
    for (int i = 0; i < SIZE; i++) {
        p->board.board[i] = malloc(SIZE * sizeof(int));
        if (p->board.board[i] == NULL) {
            exit(15);
        }
        for (int j = 0; j < SIZE; j++) {
            p->board.board[i][j] = WATER;
        }
    }

    // Generate and place boats on the board
    int collision = 0;
    int x, y, or, size = 0;

    for (int i = 0; i < NB; i++) {
        size = i + 1; // there will be one boat of each size

        while (collision == 0) {
            collision = 1;
            x = rand() % SIZE;
            y = rand() % SIZE;
            or = rand() % 2;

            // Check for collisions with existing boats
            for (int j = 0; j < size; j++) {
                if (or == 1) {
                    if (y + j >= SIZE || p->board.board[x][y + j] == BOAT) {
                        collision = 0;
                        break;
                    }
                } else {
                    if (x + j >= SIZE || p->board.board[x + j][y] == BOAT) {
                        collision = 0;
                        break;
                    }
                }
            }
        }

        // Place the boat on the board
        collision = 0;
        if (or == 1) {
            for (int k = 0; k < size; k++) {
                p->board.board[x][y + k] = BOAT;
            }
        } else {
            for (int h = 0; h < size; h++) {
                p->board.board[x + h][y] = BOAT;
            }
        }

        // Create a Ship object and store it in the player's ships array
        p->ships[i] = createShip(size, x, y, or);
    }
}

// Function to create a player
Player* create_Player() {
    // Allocate memory for a Player structure
    Player* p = malloc(sizeof(Player));
    if (p == NULL) {
        exit(20);
    }

    // Initialize player attributes and call board_init to set up the game board
    p->nb_ships = NB;
    p->ships = malloc(sizeof(Ship) * NB);
    if (p->ships == NULL) {
        free(p);
        exit(21);
    }
    p->board.size = SIZE;
    board_init(p);
    return p;
}

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

// Function to search for a boat at a specific location and update the game state
int searchBoat(Player* p, int choice_X, int choice_Y) {
    if (p == NULL || choice_X < 0 || choice_X > SIZE - 1 || choice_Y < 0 || choice_Y > SIZE - 1) {
        exit(23);
    }

    // Check if there is a boat at the chosen location
    if (p->board.board[choice_X][choice_Y] == BOAT) {
        // Update the ship's state if a boat is hit
        for (int i = 0; i < p->nb_ships; i++) {
            if (p->ships[i]->pos[0] == choice_X || p->ships[i]->pos[1] == choice_Y) {
                for (int j = 0; j < p->ships[i]->size; j++) {
                    if ((p->ships[i]->or == 1 && p->ships[i]->pos[0] == choice_X && p->ships[i]->pos[1] + j == choice_Y) ||
                        (p->ships[i]->or == 0 && p->ships[i]->pos[0] + j == choice_X && p->ships[i]->pos[1] == choice_Y)) {
                        p->ships[i]->life[j] = WRECK;
                        break;
                    }
                }
            }
        }
        return 1; // Boat found
    } else {
        return 0; // No boat found
    }
}

// Function to flush the input buffer
void flush() {
    char c = getchar();
    while (c != '\n') {
        c = getchar();
    }
}

// Function to print the player's own game board
void printAllyBoard(Player* p) {
    if (p == NULL) {
        exit(50); // Exit if player is NULL
    }

    // Print the column numbers
    puts("");
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
    puts("Hello captain, please enter X coordinate");
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
    puts("Hello captain, please enter Y coordinate");
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






// Deallocates memory for a ship, including its life status array.
void unalloc_ship(Ship* ship) {
    if (ship == NULL) {
        exit(3); // Exit if ship is NULL
    }
    free(ship->life);
    free(ship);
}

// Deallocates memory for the game board.
void freeBoard(Player* p) {
    if (p == NULL) {
        exit(10); // Exit if Player is NULL
    }

    int** mat = p->board.board;
    if (mat != NULL) {
        // Free memory for each row of the 2D array
        for (int i = 0; i < SIZE; i++) {
            free(mat[i]);
        }
        // Free memory for the array of pointers
        free(mat);
        p->board.board = NULL;
    }
}

// Deallocates memory for a Player, including its ships and the game board.
void freePlayer(Player* player) {
    // Free memory for the game board
    freeBoard(player);

    // Free memory for each ship
    for (int i = 0; i < player->nb_ships; i++) {
        unalloc_ship(player->ships[i]);
    }

    // Free memory for the array of ships
    free(player->ships);

    // Free memory for the Player structure
    free(player);
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

// Main function implementing the game loop
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





#include "player.h"  

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

// Creates a new ship with the specified size, position, and orientation.
Ship* createShip(int size, int pos1,int pos2, int or){
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



// Deallocates memory for a ship, including its life status array.
void unalloc_ship(Ship* ship) {
    if (ship == NULL) {
        exit(3); // Exit if ship is NULL
    }
    free(ship->life);
    free(ship);
}
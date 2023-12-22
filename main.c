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
    int or;      /**< Orientation of the ship 0 for horizontal and 1 for vertical */
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
 * @param pos Position of the ship on the board [x, y].
 * @param or Orientation of the ship : 1 -> horizontal and 0 -> vertical
 * @return Pointer to the created ship.
 */
Ship* createShip(int size, int pos1,int pos2, int or);

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
void board_init(Player* p){
    if(p==NULL){
        exit(10); // Exit if player is NULL
    }
    p->board.board = malloc(sizeof(int*)*SIZE);
    if(p->board.board==NULL){
        exit(11);
    }
    for(int i=0;i<SIZE;i++){
        p->board.board[i] = malloc(SIZE*sizeof(int));
        if(p->board.board[i]==NULL){
            exit(15);
        }
        for(int j=0;j<SIZE;j++){
            p->board.board[i][j] = WATER;
        }
    }

    int collision = 0;
    int x ;
    int y ;
    int or;
    int size = 0;

    for(int i=0;i<NB;i++){
        size = i+1; // there will be one boat of each size

        while(collision==0){
            collision = 1;
            x = rand()%SIZE;
            y = rand()%SIZE;
            or = rand()%2;
            printf("%d %d %d taille : %d \n",x,y,or,size);
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
        collision = 0;
        if(or==1){
            puts("test1");
            for(int k=0;k<size;k++){
                p->board.board[x][y+k] = BOAT ;
            }
        }
        else{
            puts("test2");
            for(int h=0;h<size;h++){
                p->board.board[x+h][y] = BOAT;
            }
        }
        puts("test3");
        p->ships[i] = createShip(size,x,y,or);         
    }

}

Player* create_Player(){
    Player* p = malloc(sizeof(Player));
    if(p==NULL){
        exit(20);
    }
    p->nb_ships = NB;
    p->ships = malloc(sizeof(Ship)*NB);
    if(p->ships==NULL){
        free(p);
        exit(21);
    }
    p->board.size = SIZE;
    board_init(p);
    return p;
}

Game* create_Game(){
    Game* game = malloc(sizeof(Game));
    if(game == NULL){
        exit(22);
    }
    game->player1 = create_Player();
    game->computer = create_Player();
    return game;
}


int searchBoat(Player* p,int choice_X,int choice_Y){
    if(p == NULL || choice_X < 0 || choice_X > SIZE-1 || choice_Y < 0 || choice_Y > SIZE-1){
        exit(23);
    }
    if(p->board.board[choice_X][choice_Y] == BOAT){
        for(int i=0;i<p->nb_ships;i++){
            if(p->ships[i]->pos[0] == choice_X || p->ships[i]->pos[1] == choice_Y){
                for(int j=0;j<p->ships[i]->size;j++){
                    if ((p->ships[i]->or == 1 && p->ships[i]->pos[0] == choice_X && p->ships[i]->pos[1] + j == choice_Y) || (p->ships[i]->or == 0 && p->ships[i]->pos[0] + j == choice_X && p->ships[i]->pos[1] == choice_Y)) {
                        p->ships[i]->life[j] = WRECK;
                        break;
                    }
                }

            }
        }
        return 1;  
    }
    else{
        return 0;
    }
}



void flush(){
    char c = getchar();
    while(c!='\n'){
        c = getchar();
    }
}

void printAllyBoard(Player* p) {
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


void playerTurn(Game* game){
    if(game==NULL){
        exit(40);
    }
    int choice_X = 0;
    int choice_Y = 0;
    int verif = 0;
    int ok = 1;
    puts("Hello captain, please enter X coordonate");
    do{
        verif = scanf("%d", &choice_X);
        if(verif != 1 || choice_X < 0 || choice_X > SIZE-1){
            puts("please enter something correct");
            ok = 0;
            flush();
        }
        else{
            ok=1;
        }
    }while(ok == 0);
    ok = 1;
    puts("Hello captain, please enter Y coordonate");
    do{
        verif = scanf("%d", &choice_Y);
        if(verif != 1 || choice_Y < 0 || choice_Y > SIZE-1){
            puts("please enter something correct");
            ok = 0;
            flush();
        }
        else{
            ok=1;
        }
    }while(ok == 0);

    if(searchBoat(game->computer,choice_X,choice_Y)){
        game->computer->board.board[choice_X][choice_Y] = WRECK;
        puts("----------------- HIT !!!!! WELL DONE SIR -----------------------");
        printEnemyBoard(game->computer);
    }
    else{
        if(game->computer->board.board[choice_X][choice_Y] == WRECK){
            game->computer->board.board[choice_X][choice_Y] = WRECK;
        }
        else{
            game->computer->board.board[choice_X][choice_Y] = WATER_SHOT;
        }
        puts("--------- SIR WE MISSED THE TARGET PUT SOME GLASSES !!! ---------");
    }

    

}

void computerTurn(Game* game){
    if(game==NULL){
        exit(40);
    }
    int choice_X = 0;
    int choice_Y = 0;
    do{
        choice_X = rand()%SIZE;
        choice_Y = rand()%SIZE;
    }while(game->player1->board.board[choice_X][choice_Y] == WRECK || game->player1->board.board[choice_X][choice_Y] == WATER_SHOT);
    if(searchBoat(game->player1,choice_X,choice_Y)){
        game->player1->board.board[choice_X][choice_Y] = WRECK;
        puts("----------------------- SIR WE HAVE BEEN HIT -----------------------");
    }
    else{
        if(game->player1->board.board[choice_X][choice_Y] == WRECK){
            game->player1->board.board[choice_X][choice_Y] = WRECK;
        }
        else{
            game->player1->board.board[choice_X][choice_Y] = WATER_SHOT;
        }
        puts("---------------------- GOOD NEWS, THEY MISSED ----------------------");
    }

    

}






// Deallocates memory for a ship, including its life status array.
void unalloc_ship(Ship* ship){
    if(ship == NULL){
        exit(3); // Exit if ship is NULL
    }
    free(ship->life);
    free(ship);
}

// Deallocates memory for the game board.
void freeBoard(Player* p) {
    if (p == NULL) {
        exit(10); // Exit if player is NULL
    }

    int** mat = p->board.board;
    if (mat != NULL) {
        for (int i = 0; i < SIZE; i++) {
            free(mat[i]);
        }
        free(mat);
        p->board.board = NULL;
    }
}



// Deallocates memory for a player, including its ships and the game board.
void freePlayer(Player* player) {
    freeBoard(player);
    for (int i = 0; i < player->nb_ships; i++) {
        unalloc_ship(player->ships[i]);
    }
    free(player->ships);
    free(player);
}

void freeGame(Game* game){
    freePlayer(game->computer);
    freePlayer(game->player1);
    free(game);
}


int main(){
    srand(time(NULL));
    int win = 0;
    Game* game = create_Game();
    while(win == 0){
        puts("----- SIR IT IS OUR TURN TO ATTACK ------");
        puts("our ships : ");
        printAllyBoard(game->player1);
        puts("\nenemi ships : ");
        printEnemyBoard(game->computer);
        playerTurn(game);
        sleep(2);
        if(is_player_dead(game->computer)){
            puts("----- SIR, WE JUST WON THIS BATTLE, CONGRATULATION ------");
            break;
        }
        puts("our ships : ");
        printAllyBoard(game->player1);
        sleep(1);
        puts("\nenemi ships : ");
        printEnemyBoard(game->computer);

        puts("----- SIR THE ENEMY IS ATTACKING TAKE COVER ! ------");
        sleep(4);
        computerTurn(game);
        if(is_player_dead(game->player1)){
            puts("----- YOU DIED ------");
            break;
        }
        puts("our ships : ");
        printAllyBoard(game->player1);
        puts("\nEnemy ships : ");
        printEnemyBoard(game->computer);
        sleep(4);        
    }

    freeGame(game);
    return 0;
}



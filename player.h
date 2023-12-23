#ifndef PLAYER_H
    #define PLAYER_H

    #include <stdio.h>
    #include <stdlib.h>
    #include <time.h>
    #include <unistd.h>

    

    #define SIZE 3
    #define NB 3
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
     * @brief Enumeration representing different types of elements.
     */
    static enum {
        WATER,         /**< Water type */
        WATER_SHOT,    /**< Water shot type */
        BOAT,          /**< Boat type */
        WRECK          /**< Wreck type */
    } type;

    /**
     * @brief Initializes the game board for a player and places ships randomly.
     * @param p Pointer to the player.
     */
    void board_init(Player* p);

    /**
     * @brief Searches for a boat at the specified coordinates and updates the board.
     * @param p Pointer to the player.
     * @param choice_X X-coordinate of the target.
     * @param choice_Y Y-coordinate of the target.
     * @return 1 if a boat is hit, 0 otherwise.
     */
    int searchBoat(Player* p, int choice_X, int choice_Y);

    /**
     * @brief Deallocates memory for the game board.
     * @param p Pointer to the player.
     */
    void freeBoard(Player* p);


    /**
     * @brief Function to check if a player is dead.
     * @param p Pointer to the player.
     * @return 1 if the player is dead, 0 otherwise.
     */
    int is_player_dead(Player* p);

    /**
     * @brief Creates a new player with initialized board and ships.
     * @return Pointer to the created player.
     */
    Player* create_Player();

    /**
     * @brief Deallocates memory for a player, including its ships and the game board.
     * @param player Pointer to the player.
     */
    void freePlayer(Player* player);

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
     * @brief Deallocates memory for a ship, including its life status array.
     * @param ship Pointer to the ship.
     */
    void unalloc_ship(Ship* ship);


#endif
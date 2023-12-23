#ifndef GAME_H
    #define GAME_H

    #include "player.h"
    #include "print.h"  
    
    /**
     * @brief Structure representing the game.
     */
    typedef struct {
        Player* player1; /**< Player 1 */
        Player* computer; /**< Computer */
    } Game;

    /**
     * @brief Creates a new game with two players.
     * @return Pointer to the created game.
     */
    Game* create_Game();

    /**
     * @brief Clears the input buffer.
     */
    void flush();

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
     * @brief Deallocates memory for the game, including both players.
     * @param game Pointer to the game.
     */
    void freeGame(Game* game);
#endif
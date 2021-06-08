#include <iostream>
#include "Game.h"

int main()
{
        Game game = Game::init();

        Game::start(game, 3);

        if (game.getState() == GameState::WHITE_WIN)
            std::cout << "\n**********Checkmate!! WHITE WON **********\n";
        else if (game.getState() == GameState::BLACK_WIN)
            std::cout << "\n**********Checkmate!! BLACK WON **********\n";
        else if (game.getState() == GameState::INSUFFICIENT_MATERIAL)
            std::cout << "\n**********INSUFFICIENT_MATERIAL**********\n";
        else if (game.getState() == GameState::THREEFOLD_REPETITION)
            std::cout << "\n**********THREEFOLD_REPETITION**********\n";
        else if (game.getState() == GameState::STALEMATE)
            std::cout << "\n**********STALEMATE**********\n";
        else if (game.getState() == GameState::FIFTY_REPETITION)
            std::cout << "\n**********FIFTY_REPETITION**********\n";

        std::cout << "\nnumber of move : " << game.getNumberOfMove();

    return 0;
}


#include <iostream>
#include "Game.h"

//TODO: FEN notation converter
//TODO: improvements
//TODO: when game is stalemate, score should be decreased //it is done i think
//TODO: GUI
//TODO: refactor
//TODO: github
//TODO: .h and .cpp files should be split

std::vector<int> nodePerMove;

int Perft(Board b, int depth) {
    MoveGenerator mg;
    mg.generateLegalMoveSet(b);

    int nodes = 0;

    if (depth == 0)
        return 1;

    for (const Move &move : mg.legalMoveSet) {
        Board tempBoard = b;
        tempBoard.makeMove(move);
        nodes += Perft(tempBoard, depth - 1);
        if (depth == 4) {
            std::cout << "Move: " << (char) ('A' + move.getFrom() % 8) << (char) ('1' + move.getFrom() / 8)
                      << "-" << (char) ('A' + move.getTo() % 8) << (char) ('1' + move.getTo() / 8) << ", nodes: "
                      << nodes << "\n";
            nodePerMove.push_back(nodes);
            nodes = 0;
        }
    }
    return nodes;
}

void PerftAllDepth(int depth) {
    for (int i = depth - 1; i < depth; i++) {
        auto start = std::chrono::system_clock::now();
        std::cout << "node per move: \n";
        Board testBoard;
        Perft(testBoard, i);
        int totalNodes = 0;
        for (int nodeNum : nodePerMove)
            totalNodes += nodeNum;
        std::cout << "Perft: Total number of nodes : " << totalNodes << " in depth " << i;
        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start) / 1000.f;

        std::cout << "\n\nÖlçüm " << elapsed.count() << " saniye sürdü.. \n";
        std::cout << "*****************************************************\n\n";
    }
}


int main() {
    LookupTables::init();

//    PerftAllDepth(5);

    Game game = Game::init();
    auto start = std::chrono::system_clock::now();


    //Game::start(game, 2);
    Game::startWithPlayer(game, 3, Color::WHITE);

    auto end = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start) / 1000.f;
    std::cout << "\n\nÖlçüm " << elapsed.count() << " saniye sürdü.. \n";
    std::cout << "*****************************************************\n\n";
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


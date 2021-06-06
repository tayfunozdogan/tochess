#include <iostream>
#include <strings.h>
#include <algorithm>
#include <random>
#include <optional>
#include "MoveGenerator.h"
#include "LookupTables.h"
#include "common.h"
#include "Move.h"

void displayBitboard(const Bitboard &bitboard)
{
//    for (size_t i = bitboard.size(); i > 0; --i) {
//        if (!(i % 8))
//            std::cout << "\n";
//        std::cout << bitboard[i - 1] << " ";
//    }
//    std::cout << "\n";
    for (size_t i = 0; i < bitboard.size(); ++i) {
        if (!(i % 8))
            std::cout << "\n";
        std::cout << bitboard[i] << " ";
    }
    std::cout << "\n";
}

void displayMoveSet(const MoveSet &ms)
{
    std::cout << "size = " << ms.size() << "\n";
    std::for_each(ms.cbegin(), ms.cend(),
                  [](const Move& move)
                  {
                      static int cnt = 0;
                      if (cnt % 10 == 0)
                          std::cout << "\n";
                      std::cout << "Move: " << (char)('A' + move.getFrom() % 8) << (char)('1' + move.getFrom() / 8)
                                << "-" << (char)('A' + move.getTo() % 8) << (char)('1' + move.getTo() / 8) << ",  ";
                      ++cnt;
                  });
    std::cout << "\n\n";
}

std::optional<Move> findMove(const Board &board, const MoveSet &moveSet)
{
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0, moveSet.size() - 1);

    return moveSet.empty() ? std::nullopt : std::optional<Move>(moveSet[dis(gen)]);
}


enum class GameState {
    WHITE_WIN, BLACK_WIN, DRAW, STALEMATE, ONGOING
};

class Game {
    Board board;
    GameState state;

    Game()
    {
        state = GameState::ONGOING;
    }
public:
    static Game init()
    {
        LookupTables::init();
        return {};
    }

    static void start(Game &game)
    {
        game.board.displayBoard();

        while (game.getState() == GameState::ONGOING) {
            MoveGenerator moveGen;
            moveGen.generateLegalMoveSet(game.board);
            displayMoveSet(moveGen.legalMoveSet);

            auto foundMove = findMove(game.board, moveGen.legalMoveSet);

            //detectState
            if (!foundMove.has_value()) {
                MoveGenerator opponentMoveSet;
                opponentMoveSet.generatePseudoMoveSet(game.board, game.board.inactivePlayer);
                if (!MoveGenerator::isKingInCheckByAnyMove(game.board, opponentMoveSet.pseudoMoveSet, game.board.activePlayer)) {
                    game.state = GameState::STALEMATE;
                } else {
                    game.state = game.board.inactivePlayer == Color::WHITE ? GameState::WHITE_WIN : GameState::BLACK_WIN;
                }
                return;
            } else if (game.board.getAllWhitePieces().count() == 1 && game.board.getAllBlackPieces().count() == 1) {
                game.state = GameState::DRAW;
                return;
            }
            std::cout << "chosen Move: " << (char)('A' + foundMove.value().getFrom() % 8) << (char)('1' + foundMove.value().getFrom() / 8)
                      << "-" << (char)('A' + foundMove.value().getTo() % 8) << (char)('1' + foundMove.value().getTo() / 8) << "\n\n";
            std::cout << "-------------------------------------------------------------------------------------------\n\n";
            //active player has changed
            game.board.makeMove(foundMove.value());
            game.board.displayBoard();
        }
    }


    GameState getState() { return state; }
    void setState(GameState t_state) { state = t_state; }
};

int main()
{
    Game game = Game::init();

    Game::start(game);


    if (game.getState() == GameState::WHITE_WIN)
        std::cout << "\n**********Checkmate!! WHITE WON **********\n";
    else if (game.getState() == GameState::BLACK_WIN)
        std::cout << "\n**********Checkmate!! BLACK WON **********\n";
    else if (game.getState() == GameState::DRAW)
        std::cout << "\n**********DRAW**********\n";
    else if (game.getState() == GameState::STALEMATE)
        std::cout << "\n**********STALEMATE**********\n";
//    LookupTables::init();
//    Board board;
//    board.displayBoard();
//    MoveGenerator mg;
////    board.changeActivePlayer(); //TEST
//    mg.generateLegalMoveSet(board, board.activePlayer);
//    displayMoveSet(mg.legalMoveSet);
//
//    displayBitboard(MoveGenerator::generateKingMoves(board.whiteKing, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateKnightMoves(board.whiteKnights, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateWhitePawnMoves(board.whitePawns, board.getAllBlackPieces(), board.getAllPieces()));
//    displayBitboard(MoveGenerator::generateBlackPawnMoves(board.blackPawns, board.getAllWhitePieces(), board.getAllPieces()));

    return 0;
}


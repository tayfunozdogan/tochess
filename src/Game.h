//
// Created by Tayfun Özdoğan on 7.06.2021.
//

#ifndef TOCHESS_GAME_H
#define TOCHESS_GAME_H

#include <iostream>
#include <algorithm>
#include "Board.h"
#include "LookupTables.h"
#include "MoveGenerator.h"
#include "Search.h"

enum class GameState {
    WHITE_WIN, BLACK_WIN, INSUFFICIENT_MATERIAL, STALEMATE, ONGOING, FIFTY_REPETITION, THREEFOLD_REPETITION
};

typedef std::vector<Board> BoardHistory;

class Game {
    Board board;
    GameState state;
    BoardHistory boardHistory;

    Game()
    {
        state = GameState::ONGOING;
    }

    bool isThreefoldRepetition()
    {
        const Board &curBoard = board;
        int threefoldCount = 1;

        for (const auto &prevBoard : boardHistory) {
            if ((curBoard.getAllPieces() == prevBoard.getAllPieces()) &&
                (curBoard.getAllBlackPieces() == prevBoard.getAllBlackPieces() && curBoard.getAllWhitePieces() == prevBoard.getAllWhitePieces()) &&
                (curBoard.whitePawns == prevBoard.whitePawns && curBoard.blackPawns == prevBoard.blackPawns) &&
                (curBoard.whiteRooks == prevBoard.whiteRooks && curBoard.blackRooks == prevBoard.blackRooks) &&
                (curBoard.whiteKnights == prevBoard.whiteKnights && curBoard.blackKnights == prevBoard.blackKnights) &&
                (curBoard.whiteBishops == prevBoard.whiteBishops && curBoard.blackBishops == prevBoard.blackBishops) &&
                (curBoard.whiteQueens == prevBoard.whiteQueens && curBoard.blackQueens == prevBoard.blackQueens) &&
                (curBoard.whiteKing == prevBoard.whiteKing && curBoard.blackKing == prevBoard.blackKing) &&
                (curBoard.castlingRights[0] == prevBoard.castlingRights[0]) &&
                (curBoard.castlingRights[1] == prevBoard.castlingRights[1]) &&
                (curBoard.castlingRights[2] == prevBoard.castlingRights[2]) &&
                (curBoard.castlingRights[3] == prevBoard.castlingRights[3]) &&
                (curBoard.attackableWhitePawnsEnPassant == prevBoard.attackableWhitePawnsEnPassant) &&
                (curBoard.attackableBlackPawnsEnPassant == prevBoard.attackableBlackPawnsEnPassant)) {
                threefoldCount++;
            }

            if (threefoldCount == 3)
                return true;
        }

        return false;
    }

public:
    static Game init()
    {
        LookupTables::init();
        return {};
    }

    static void start(Game &game, int AILevel)
    {
        game.boardHistory.push_back(game.board);
        game.board.displayBoard();
        //todo: AI should consider game states on the next moves
        while (game.getState() == GameState::ONGOING) {
            if (game.board.halfMoveClock == 100) {
                game.state = GameState::FIFTY_REPETITION;
                return;
            }
            if (game.isThreefoldRepetition()) {
                game.state = GameState::THREEFOLD_REPETITION;
                return;
            }
            game.boardHistory.push_back(game.board);


            MoveGenerator moveGen;
            moveGen.generateLegalMoveSet(game.board);
            displayMoveSet(moveGen.legalMoveSet);

            auto foundMove = Search::findMove(game.board, AILevel);

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
                game.state = GameState::INSUFFICIENT_MATERIAL;
                return;
            }

            displayMove(foundMove.value());
            std::cout << "\n-------------------------------------------------------------------------------------------\n";

            game.board.makeMove(foundMove.value()); //active player has changed
            game.board.displayBoard();
        }
    }

    float getNumberOfMove() { return (boardHistory.size() - 2.f) / 2.f; }
    GameState getState() { return state; }
    void setState(GameState t_state) { state = t_state; }
};

#endif //TOCHESS_GAME_H

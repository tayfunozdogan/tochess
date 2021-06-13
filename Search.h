//
// Created by Tayfun Özdoğan on 7.06.2021.
//

#ifndef POORMANSENGINE_SEARCH_H
#define POORMANSENGINE_SEARCH_H


#include <random>
#include <optional>
#include "Move.h"
#include "Board.h"
#include "common.h"
#include "MoveGenerator.h"

constexpr int initScore = -100000;

std::optional<Move> findMove(const Board &board, const MoveSet &moveSet)
{
    std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
    std::uniform_int_distribution<int> dis(0, moveSet.size() - 1);

    return moveSet.empty() ? std::nullopt : std::optional<Move>(moveSet[dis(gen)]);
}

class Search {

    static int evaluateBoard(Board board)
    {
        int score = 0;

        if (board.activePlayer == Color::WHITE) {
            while (board.whitePawns.any()) {
                score += EvaluationConstants::pawnValue;
                size_t sq = BitOps::findFirstBit(board.whitePawns);
                score += EvaluationConstants::whitePawn[sq];
                board.whitePawns.reset(sq);
            }
            while (board.whiteKnights.any()) {
                score += EvaluationConstants::knightValue;
                size_t sq = BitOps::findFirstBit(board.whiteKnights);
                score += EvaluationConstants::whiteKnight[sq];
                board.whiteKnights.reset(sq);
            }
            while (board.whiteBishops.any()) {
                score += EvaluationConstants::bishopValue;
                size_t sq = BitOps::findFirstBit(board.whiteBishops);
                score += EvaluationConstants::whiteBishop[sq];
                board.whiteBishops.reset(sq);
            }
            while (board.whiteRooks.any()) {
                score += EvaluationConstants::rookValue;
                size_t sq = BitOps::findFirstBit(board.whiteRooks);
                score += EvaluationConstants::whiteRook[sq];
                board.whiteRooks.reset(sq);
            }
            while (board.whiteQueens.any()) {
                score += EvaluationConstants::queenValue;
                size_t sq = BitOps::findFirstBit(board.whiteQueens);
                score += EvaluationConstants::whiteQueen[sq];
                board.whiteQueens.reset(sq);
            }
            while (board.whiteKing.any()) {
                score += EvaluationConstants::kingValue;
                size_t sq = BitOps::findFirstBit(board.whiteKing);
                score += EvaluationConstants::whiteKing[sq];
                board.whiteKing.reset(sq);
            }
        }
        else {
            while (board.blackPawns.any()) {
                score += EvaluationConstants::pawnValue;
                size_t sq = BitOps::findFirstBit(board.blackPawns);
                score += EvaluationConstants::blackPawn[sq];
                board.blackPawns.reset(sq);
            }
            while (board.blackKnights.any()) {
                score += EvaluationConstants::knightValue;
                size_t sq = BitOps::findFirstBit(board.blackKnights);
                score += EvaluationConstants::blackKnight[sq];
                board.blackKnights.reset(sq);
            }
            while (board.blackBishops.any()) {
                score += EvaluationConstants::bishopValue;
                size_t sq = BitOps::findFirstBit(board.blackBishops);
                score += EvaluationConstants::blackBishop[sq];
                board.blackBishops.reset(sq);
            }
            while (board.blackRooks.any()) {
                score += EvaluationConstants::rookValue;
                size_t sq = BitOps::findFirstBit(board.blackRooks);
                score += EvaluationConstants::blackRook[sq];
                board.blackRooks.reset(sq);
            }
            while (board.blackQueens.any()) {
                score += EvaluationConstants::queenValue;
                size_t sq = BitOps::findFirstBit(board.blackQueens);
                score += EvaluationConstants::blackQueen[sq];
                board.blackQueens.reset(sq);
            }
            while (board.blackKing.any()) {
                score += EvaluationConstants::kingValue;
                size_t sq = BitOps::findFirstBit(board.blackKing);
                score += EvaluationConstants::blackKing[sq];
                board.blackKing.reset(sq);
            }
        }
        return score;

    }

    static int searchTree(const Board &t_board, int alpha, int beta, int level)
    {
        if (level == 0)
            return evaluateBoard(t_board);

        MoveGenerator moveGen;
        moveGen.generateLegalMoveSet(t_board);

        int bestScore = initScore;

        for (const Move &move : moveGen.legalMoveSet) {
            Board board = t_board;

            if (bestScore >= beta)
                break;

            if (bestScore > alpha)
                alpha = bestScore;


            if ((move.getMoveType() == MoveType::CAPTURE || move.getMoveType() == MoveType::CAPTURE_PROMOTION) &&
                !(board.activePlayer == Color::WHITE ? board.whiteKing.any() : board.blackKing.any())) {
                return 90000 + level;
            }

            board.makeMove(move);
            int newScore = -1 * searchTree(board, -beta, -alpha, level - 1);

            if (newScore > bestScore)
                bestScore = newScore;
        }

        return bestScore;
    }

public:
    static std::optional<Move> findMove(const Board &t_board, int depth)
    {
        // Make a list of all legal moves
        MoveGenerator moveGen;
        moveGen.generateLegalMoveSet(t_board);

        MoveSet bestMoveSet;

        int bestScore = initScore;

        for (auto move : moveGen.legalMoveSet) {
            Board board = t_board;

            board.makeMove(move);

            //stalemate check
            MoveGenerator opponentMoveSet;
            opponentMoveSet.generatePseudoMoveSet(board, board.activePlayer);
            if (opponentMoveSet.pseudoMoveSet.empty() && (board.activePlayer == Color::WHITE ? board.whiteKing : board.blackKing).any()) {
                bestMoveSet.clear();
                bestMoveSet.push_back(move);
                continue;
            }

            int newScore = -1 * searchTree(board, initScore, -initScore, depth);

            if (newScore > bestScore) {
                bestScore = newScore;
                bestMoveSet.clear();
                bestMoveSet.push_back(move);
//                std::cout << "Score : " << bestScore << "\n";
//                displayMoveSet(bestMoveSet);
            } else if (newScore == bestScore) {
                bestMoveSet.push_back(move);
//                std::cout << "Score : " << bestScore << "\n";
//                displayMoveSet(bestMoveSet);
            }
        }

        std::mt19937 gen(std::chrono::system_clock::now().time_since_epoch().count());
        std::uniform_int_distribution<int> dis(0, bestMoveSet.size() - 1);

        return bestMoveSet.empty() ? std::nullopt : std::optional<Move>(bestMoveSet[dis(gen)]);
    }
};

#endif //POORMANSENGINE_SEARCH_H

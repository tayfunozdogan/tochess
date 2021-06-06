//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef POORMANSENGINE_MOVEGENERATOR_H
#define POORMANSENGINE_MOVEGENERATOR_H

#include "common.h"
#include "Move.h"
#include "Board.h"
#include "LookupTables.h"

typedef std::vector<Move> MoveSet;

class MoveGenerator {
private:     //TODO: whole code must be refactored
    Bitboard getMagicRookMoves(const size_t &from, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        auto blockers = (LookupTables::rookMasks[from] & allPieces).to_ullong();
        uint64_t key = (blockers * rookMagics[from]) >> (g_boardSize - rookBitsCnt[from]);
        return LookupTables::SlidingAttacks::rookMagicTable[from][key] & ~ownPieces;
    }
    Bitboard getMagicBishopMoves(const size_t &from, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        auto blockers = (LookupTables::bishopMasks[from] & allPieces).to_ullong();
        uint64_t key = (blockers * bishopMagics[from]) >> (g_boardSize - bishopBitsCnt[from]);
        return LookupTables::SlidingAttacks::bishopMagicTable[from][key] & ~ownPieces;
    }
    void generateKingMoves(MoveSet &moveSet, Bitboard king, const Bitboard &ownPieces)
    {
        const PieceType pieceType = PieceType::KING;
        while (king.any()) {
            size_t from = BitOps::findFirstBit(king);
            Bitboard generated = LookupTables::NonSlidingAttacks::kingAttacks[from] & ~ownPieces;
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to, pieceType);
                generated.reset(to);
            }
            king.reset(from);
        }
    }
    void generateKnightMoves(MoveSet &moveSet, Bitboard knights, const Bitboard &ownPieces)
    {
        const PieceType pieceType = PieceType::KNIGHT;
        while (knights.any()) {
            size_t from = BitOps::findFirstBit(knights);
            Bitboard generated = LookupTables::NonSlidingAttacks::knightAttacks[from] & ~ownPieces;
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to, pieceType);
                generated.reset(to);
            }
            knights.reset(from);
        }
    }
    void generateRookMoves(MoveSet &moveSet, Bitboard rooks, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        const PieceType pieceType = PieceType::ROOK;
        while (rooks.any()) {
            size_t from = BitOps::findFirstBit(rooks);
            Bitboard generated = getMagicRookMoves(from, ownPieces, allPieces);
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to, pieceType);
                generated.reset(to);
            }
            rooks.reset(from);
        }
    }
    void generateBishopMoves(MoveSet &moveSet, Bitboard bishops, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        const PieceType pieceType = PieceType::BISHOP;
        while (bishops.any()) {
            size_t from = BitOps::findFirstBit(bishops);
            Bitboard generated = getMagicBishopMoves(from, ownPieces, allPieces);
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to, pieceType);
                generated.reset(to);
            }
            bishops.reset(from);
        }
    }
    void generateQueenMoves(MoveSet &moveSet, Bitboard queens, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        const PieceType pieceType = PieceType::QUEEN;
        while (queens.any()) {
            size_t from = BitOps::findFirstBit(queens);
            Bitboard generatedRook = getMagicRookMoves(from, ownPieces, allPieces);
            Bitboard generatedBishop = getMagicBishopMoves(from, ownPieces, allPieces);
            while (generatedRook.any()) {
                size_t to = BitOps::findFirstBit(generatedRook);
                moveSet.emplace_back(from, to, pieceType);
                generatedRook.reset(to);
            }
            while (generatedBishop.any()) {
                size_t to = BitOps::findFirstBit(generatedBishop);
                moveSet.emplace_back(from, to, pieceType);
                generatedBishop.reset(to);
            }
            queens.reset(from);
        }
    }
    void generateWhitePawnMoves(MoveSet &moveSet, Bitboard pawns, const Bitboard &blackPieces, const Bitboard &allPieces, const Bitboard &attackableBlackPawnsEnPassant)
    {
        const PieceType pieceType = PieceType::PAWN;
        while (pawns.any()) {
            const size_t from = BitOps::findFirstBit(pawns);
            const Bitboard pawn = BitOps::ONE << from;
            const Bitboard oneSquareMoveWithPromotions = (pawn << 8) & ~allPieces;

            //one square prom
            Bitboard oneSquarePromotions = oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_8];
            while (oneSquarePromotions.any()) {
                const size_t oneSqPromTo = BitOps::findFirstBit(oneSquarePromotions);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::KNIGHT_PROM);
                oneSquarePromotions.reset(oneSqPromTo);
            }

            //one square
            Bitboard oneSquareMove = oneSquareMoveWithPromotions & ~LookupTables::maskRank[RANK_8];
            while (oneSquareMove.any()) {
                const size_t oneSqTo = BitOps::findFirstBit(oneSquareMove);
                moveSet.emplace_back(from, oneSqTo, pieceType);
                oneSquareMove.reset(oneSqTo);
            }

            //double square
            Bitboard doubleSquaresMove = ((oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_3]) << 8) & ~allPieces;
            while (doubleSquaresMove.any()) {
                const size_t dblSqTo = BitOps::findFirstBit(doubleSquaresMove);
                moveSet.emplace_back(from, dblSqTo, pieceType, MoveType::DOUBLE_PAWN);
                doubleSquaresMove.reset(dblSqTo);
            }

            Bitboard attackMovesWithPromotions = LookupTables::NonSlidingAttacks::whitePawnAttacks[from] & blackPieces;

            //attack promotions
            Bitboard attackPromotions = attackMovesWithPromotions & LookupTables::maskRank[RANK_8];
            while (attackPromotions.any()) {
                const size_t attackPromTo = BitOps::findFirstBit(attackPromotions);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::KNIGHT_PROM);
                attackPromotions.reset(attackPromTo);
            }

            //attack moves
            Bitboard attackMoves = attackMovesWithPromotions & ~LookupTables::maskRank[RANK_8];
            while (attackMoves.any()) {
                const size_t attackTo = BitOps::findFirstBit(attackMoves);
                moveSet.emplace_back(from, attackTo, pieceType);
                attackMoves.reset(attackTo);
            }

            //enPassant
            Bitboard possibleEnPassant = LookupTables::NonSlidingAttacks::whitePawnAttacks[from] & ~allPieces;
            while (possibleEnPassant.any()) {
                const size_t enPassantTo = BitOps::findFirstBit(possibleEnPassant);
                if (attackableBlackPawnsEnPassant[enPassantTo + 8]) {
                    moveSet.emplace_back(from, enPassantTo, pieceType, MoveType::EN_PASSANT);
                }
                possibleEnPassant.reset(enPassantTo);
            }

            pawns.reset(from);
        }
    }
    void generateBlackPawnMoves(MoveSet &moveSet, Bitboard pawns, const Bitboard &whitePieces, const Bitboard &allPieces, const Bitboard &attackableWhitePawnsEnPassant)
    {
        const PieceType pieceType = PieceType::PAWN;
        while (pawns.any()) {
            const size_t from = BitOps::findFirstBit(pawns);
            const Bitboard pawn = BitOps::ONE << from;
            const Bitboard oneSquareMoveWithPromotions = (pawn >> 8) & ~allPieces;

            //one square prom
            Bitboard oneSquarePromotions = oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_1];
            while (oneSquarePromotions.any()) {
                const size_t oneSqPromTo = BitOps::findFirstBit(oneSquarePromotions);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, oneSqPromTo, pieceType, MoveType::PROMOTION, PromotionType::KNIGHT_PROM);
                oneSquarePromotions.reset(oneSqPromTo);
            }

            //one square
            Bitboard oneSquareMove = oneSquareMoveWithPromotions & ~LookupTables::maskRank[RANK_1];
            while (oneSquareMove.any()) {
                const size_t oneSqTo = BitOps::findFirstBit(oneSquareMove);
                moveSet.emplace_back(from, oneSqTo, pieceType);
                oneSquareMove.reset(oneSqTo);
            }

            //double square
            Bitboard doubleSquaresMove = ((oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_6]) >> 8) & ~allPieces;
            while (doubleSquaresMove.any()) {
                const size_t dblSqTo = BitOps::findFirstBit(doubleSquaresMove);
                moveSet.emplace_back(from, dblSqTo, pieceType, MoveType::DOUBLE_PAWN);
                doubleSquaresMove.reset(dblSqTo);
            }

            Bitboard attackMovesWithPromotions = LookupTables::NonSlidingAttacks::blackPawnAttacks[from] & whitePieces;

            //attack promotions
            Bitboard attackPromotions = attackMovesWithPromotions & LookupTables::maskRank[RANK_1];
            while (attackPromotions.any()) {
                const size_t attackPromTo = BitOps::findFirstBit(attackPromotions);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, attackPromTo, pieceType, MoveType::CAPTURE_PROMOTION, PromotionType::KNIGHT_PROM);
                attackPromotions.reset(attackPromTo);
            }

            //attack moves
            Bitboard attackMoves = attackMovesWithPromotions & ~LookupTables::maskRank[RANK_1];
            while (attackMoves.any()) {
                const size_t attackTo = BitOps::findFirstBit(attackMoves);
                moveSet.emplace_back(from, attackTo, pieceType);
                attackMoves.reset(attackTo);
            }

            //enPassant
            Bitboard possibleEnPassant = LookupTables::NonSlidingAttacks::blackPawnAttacks[from] & ~allPieces;
            while (possibleEnPassant.any()) {
                const size_t enPassantTo = BitOps::findFirstBit(possibleEnPassant);
                if (attackableWhitePawnsEnPassant[enPassantTo - 8]) {
                    moveSet.emplace_back(from, enPassantTo, pieceType, MoveType::EN_PASSANT);
                }
                possibleEnPassant.reset(enPassantTo);
            }

            pawns.reset(from);
        }
    }

    void genWhitePseudoMoveSetWithoutCastling(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.whiteKing, board.getAllWhitePieces());
        generateKnightMoves(moveSet, board.whiteKnights, board.getAllWhitePieces());
        generateRookMoves(moveSet, board.whiteRooks, board.getAllWhitePieces(), board.getAllPieces());
        generateBishopMoves(moveSet, board.whiteBishops, board.getAllWhitePieces(), board.getAllPieces());
        generateQueenMoves(moveSet, board.whiteQueens, board.getAllWhitePieces(), board.getAllPieces());
        generateWhitePawnMoves(moveSet, board.whitePawns, board.getAllBlackPieces(), board.getAllPieces(), board.attackableBlackPawnsEnPassant);

        seperateRegularMovesByMoveType(board, moveSet);
    }
    void genBlackPseudoMoveSetWithoutCastling(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.blackKing, board.getAllBlackPieces());
        generateKnightMoves(moveSet, board.blackKnights, board.getAllBlackPieces());
        generateRookMoves(moveSet, board.blackRooks, board.getAllBlackPieces(), board.getAllPieces());
        generateBishopMoves(moveSet, board.blackBishops, board.getAllBlackPieces(), board.getAllPieces());
        generateQueenMoves(moveSet, board.blackQueens, board.getAllBlackPieces(), board.getAllPieces());
        generateBlackPawnMoves(moveSet, board.blackPawns, board.getAllWhitePieces(), board.getAllPieces(), board.attackableWhitePawnsEnPassant);

        seperateRegularMovesByMoveType(board, moveSet);
    }

    void genWhitePseudoMoveSet(MoveSet &moveSet, const Board &board)
    {
        genWhitePseudoMoveSetWithoutCastling(moveSet, board);
        generateCastling(moveSet, board);
    }
    void genBlackPseudoMoveSet(MoveSet &moveSet, const Board &board)
    {
        genBlackPseudoMoveSetWithoutCastling(moveSet, board);
        generateCastling(moveSet, board);
    }

    void filterPseudoMoveSetByLegality(const Board &board, const Color &color) {
        if (color == Color::WHITE) {
            for (auto &candidateMove : pseudoMoveSet) {
                bool isCandidateMoveLegal = true;
                MoveSet inactivePlayerMoveSet;
                Board tempBoard = board;
                tempBoard.makeMove(candidateMove);
                genBlackPseudoMoveSet(inactivePlayerMoveSet, tempBoard);

                isCandidateMoveLegal = !isKingInCheckByAnyMove(tempBoard, inactivePlayerMoveSet, Color::WHITE);

                if (isCandidateMoveLegal)
                    legalMoveSet.push_back(candidateMove);
            }
        }
        else {
            for (auto &candidateMove : pseudoMoveSet) {
                bool isCandidateMoveLegal = true;
                MoveSet inactivePlayerMoveSet;
                Board tempBoard = board;
                tempBoard.makeMove(candidateMove);
                genWhitePseudoMoveSet(inactivePlayerMoveSet, tempBoard);

                isCandidateMoveLegal = !isKingInCheckByAnyMove(tempBoard, inactivePlayerMoveSet, Color::BLACK);

                if (isCandidateMoveLegal)
                    legalMoveSet.push_back(candidateMove);
            }
        }
    }

    void seperateRegularMovesByMoveType(const Board &board, MoveSet &moveSet)
    {
        auto currentBoard = board.getAllPieces();
        for (auto &move : moveSet) {
            if (move.getMoveType() == MoveType::REGULAR) {
                if (currentBoard.test(move.getTo()))
                    move.setMoveType(MoveType::CAPTURE);
                else
                    move.setMoveType(MoveType::QUIET);
            }
        }
    }
    void generateCastling(MoveSet &t_moveSet, const Board &board)
    {
        const PieceType pieceType = PieceType::KING;
        MoveSet inactivePlayerMoveSet;
        bool qSideCastling = true, kSideCastling = true;
        if (board.inactivePlayer == Color::WHITE) { // active BLACK
            genWhitePseudoMoveSetWithoutCastling(inactivePlayerMoveSet, board);

            // is squares between rook and king in check? is king in check?
            for (auto &move : inactivePlayerMoveSet) {
                if (move.getTo() == 58 || move.getTo() == 59)
                    qSideCastling = false;
                if (move.getTo() == 61 || move.getTo() == 62)
                    kSideCastling = false;
                if (move.getTo() == 60) {
                    kSideCastling = false;
                    qSideCastling = false;
                    break;
                }
            }
            //is there any piece in squares between rook and king? and is castling rights valid?
            if (kSideCastling && board.isBlackKSideCastlingPossible() && board.isBlackKSideCastlingRightsValid())
                t_moveSet.emplace_back(60, 62, pieceType, MoveType::CASTLING, CastlingType::BLACK_KSIDE); //add kSideCastling for black
            if (qSideCastling && board.isBlackQSideCastlingPossible() && board.isBlackQSideCastlingRightsValid())
                t_moveSet.emplace_back(60, 58, pieceType, MoveType::CASTLING, CastlingType::BLACK_QSIDE); //add qSideCastling for black
        }
        else { // active WHITE
            genBlackPseudoMoveSetWithoutCastling(inactivePlayerMoveSet, board);

            // is squares between rook and king in check? is king in check?
            for (auto &move : inactivePlayerMoveSet) {
                if (move.getTo() == 2 || move.getTo() == 3)
                    qSideCastling = false;
                if (move.getTo() == 5 || move.getTo() == 6)
                    kSideCastling = false;
                if (move.getTo() == 4) {
                    kSideCastling = false;
                    qSideCastling = false;
                }
            }
            //is there any piece in squares between rook and king? and is castling rights valid?
            if (kSideCastling && board.isWhiteKSideCastlingPossible() && board.isWhiteKSideCastlingRightsValid())
                t_moveSet.emplace_back(4, 6, pieceType, MoveType::CASTLING, CastlingType::WHITE_KSIDE); //add kSideCastling for white
            if (qSideCastling && board.isWhiteQSideCastlingPossible() && board.isWhiteQSideCastlingRightsValid())
                t_moveSet.emplace_back(4, 2, pieceType, MoveType::CASTLING, CastlingType::WHITE_QSIDE); //add qSideCastling for white
        }
    }

public:
    MoveSet pseudoMoveSet;
    MoveSet legalMoveSet;

    // todo: color can be got from board active player
    void generateLegalMoveSet(const Board &board) {
        generatePseudoMoveSet(board, board.activePlayer);
        filterPseudoMoveSetByLegality(board, board.activePlayer);
    }

    void generatePseudoMoveSet(const Board &board, const Color &color)
    {
        color == Color::WHITE ? genWhitePseudoMoveSet(pseudoMoveSet, board) : genBlackPseudoMoveSet(pseudoMoveSet, board);
    }

    static bool isKingInCheckByAnyMove(const Board &board, const MoveSet &moveSet, const Color &color)
    {
        auto &king = color == Color::WHITE ? board.whiteKing : board.blackKing;

        for (auto &move : moveSet) {
            if (BitOps::findFirstBit(king) == move.getTo()) {
                return true;
            }
        }

        return false;
    }
};
#endif //POORMANSENGINE_MOVEGENERATOR_H

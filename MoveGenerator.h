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
private:
    static Bitboard getMagicRookMoves(const size_t &from, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        auto blockers = (LookupTables::rookMasks[from] & allPieces).to_ullong();
        uint64_t key = (blockers * rookMagics[from]) >> (g_boardSize - rookBitsCnt[from]);
        return LookupTables::SlidingAttacks::rookMagicTable[from][key] & ~ownPieces;
    }
    static Bitboard getMagicBishopMoves(const size_t &from, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        auto blockers = (LookupTables::bishopMasks[from] & allPieces).to_ullong();
        uint64_t key = (blockers * bishopMagics[from]) >> (g_boardSize - bishopBitsCnt[from]);
        return LookupTables::SlidingAttacks::bishopMagicTable[from][key] & ~ownPieces;
    }
    //TODO:generating moves of pieces can be common code
    static void generateKingMoves(MoveSet &moveSet, Bitboard king, const Bitboard &ownPieces)
    {
        while (king.any()) {
            size_t from = BitOps::findFirstBit(king);
            Bitboard generated = LookupTables::NonSlidingAttacks::kingAttacks[from] & ~ownPieces;
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to);
                generated.reset(to);
            }
            king.reset(from);
        }
    }
    static void generateKnightMoves(MoveSet &moveSet, Bitboard knights, const Bitboard &ownPieces)
    {
        while (knights.any()) {
            size_t from = BitOps::findFirstBit(knights);
            Bitboard generated = LookupTables::NonSlidingAttacks::knightAttacks[from] & ~ownPieces;
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to);
                generated.reset(to);
            }
            knights.reset(from);
        }
    }
    static void generateRookMoves(MoveSet &moveSet, Bitboard rooks, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        while (rooks.any()) {
            size_t from = BitOps::findFirstBit(rooks);
            Bitboard generated = getMagicRookMoves(from, ownPieces, allPieces);
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to);
                generated.reset(to);
            }
            rooks.reset(from);
        }
    }
    static void generateBishopMoves(MoveSet &moveSet, Bitboard bishops, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        while (bishops.any()) {
            size_t from = BitOps::findFirstBit(bishops);
            Bitboard generated = getMagicBishopMoves(from, ownPieces, allPieces);
            while (generated.any()) {
                size_t to = BitOps::findFirstBit(generated);
                moveSet.emplace_back(from, to);
                generated.reset(to);
            }
            bishops.reset(from);
        }
    }
    static void generateQueenMoves(MoveSet &moveSet, Bitboard queens, const Bitboard &ownPieces, const Bitboard &allPieces)
    {
        while (queens.any()) {
            size_t from = BitOps::findFirstBit(queens);
            Bitboard generatedRook = getMagicRookMoves(from, ownPieces, allPieces);
            Bitboard generatedBishop = getMagicBishopMoves(from, ownPieces, allPieces);
            while (generatedRook.any()) {
                size_t to = BitOps::findFirstBit(generatedRook);
                moveSet.emplace_back(from, to);
                generatedRook.reset(to);
            }
            while (generatedBishop.any()) {
                size_t to = BitOps::findFirstBit(generatedBishop);
                moveSet.emplace_back(from, to);
                generatedBishop.reset(to);
            }
            queens.reset(from);
        }
    }
    static void generateWhitePawnMoves(MoveSet &moveSet, Bitboard pawns, const Bitboard &blackPieces, const Bitboard &allPieces, const Bitboard &attackableBlackPawnsEnPassant)
    {
        while (pawns.any()) {
            const size_t from = BitOps::findFirstBit(pawns);
            const Bitboard pawn = BitOps::ONE << from;
            const Bitboard oneSquareMoveWithPromotions = (pawn << 8) & ~allPieces;

            //one square prom
            Bitboard oneSquarePromotions = oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_8];
            while (oneSquarePromotions.any()) {
                const size_t oneSqPromTo = BitOps::findFirstBit(oneSquarePromotions);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::KNIGHT_PROM);
                oneSquarePromotions.reset(oneSqPromTo);
            }

            //one square
            Bitboard oneSquareMove = oneSquareMoveWithPromotions & ~LookupTables::maskRank[RANK_8];
            while (oneSquareMove.any()) {
                const size_t oneSqTo = BitOps::findFirstBit(oneSquareMove);
                moveSet.emplace_back(from, oneSqTo);
                oneSquareMove.reset(oneSqTo);
            }

            //double square
            Bitboard doubleSquaresMove = ((oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_3]) << 8) & ~allPieces;
            while (doubleSquaresMove.any()) {
                const size_t dblSqTo = BitOps::findFirstBit(doubleSquaresMove);
                moveSet.emplace_back(from, dblSqTo, MoveType::DOUBLE_PAWN);
                doubleSquaresMove.reset(dblSqTo);
            }

            Bitboard attackMovesWithPromotions = LookupTables::NonSlidingAttacks::whitePawnAttacks[from] & blackPieces;

            //attack promotions
            Bitboard attackPromotions = attackMovesWithPromotions & LookupTables::maskRank[RANK_8];
            while (attackPromotions.any()) {
                const size_t attackPromTo = BitOps::findFirstBit(attackPromotions);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::KNIGHT_PROM);
                attackPromotions.reset(attackPromTo);
            }

            //attack moves
            Bitboard attackMoves = attackMovesWithPromotions & ~LookupTables::maskRank[RANK_8];
            while (attackMoves.any()) {
                const size_t attackTo = BitOps::findFirstBit(attackMoves);
                moveSet.emplace_back(from, attackTo);
                attackMoves.reset(attackTo);
            }

            //enPassant
            Bitboard possibleEnPassant = LookupTables::NonSlidingAttacks::whitePawnAttacks[from] & ~allPieces;
            while (possibleEnPassant.any()) {
                const size_t enPassantTo = BitOps::findFirstBit(possibleEnPassant);
                if (attackableBlackPawnsEnPassant[enPassantTo + 8]) {
                    moveSet.emplace_back(from, enPassantTo, MoveType::EN_PASSANT);
                }
                possibleEnPassant.reset(enPassantTo);
            }

            pawns.reset(from);
        }
    }
    static void generateBlackPawnMoves(MoveSet &moveSet, Bitboard pawns, const Bitboard &whitePieces, const Bitboard &allPieces, const Bitboard &attackableWhitePawnsEnPassant)
    {
        while (pawns.any()) {
            const size_t from = BitOps::findFirstBit(pawns);
            const Bitboard pawn = BitOps::ONE << from;
            const Bitboard oneSquareMoveWithPromotions = (pawn >> 8) & ~allPieces;

            //one square prom
            Bitboard oneSquarePromotions = oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_1];
            while (oneSquarePromotions.any()) {
                const size_t oneSqPromTo = BitOps::findFirstBit(oneSquarePromotions);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, oneSqPromTo, MoveType::PROMOTION, PromotionType::KNIGHT_PROM);
                oneSquarePromotions.reset(oneSqPromTo);
            }

            //one square
            Bitboard oneSquareMove = oneSquareMoveWithPromotions & ~LookupTables::maskRank[RANK_1];
            while (oneSquareMove.any()) {
                const size_t oneSqTo = BitOps::findFirstBit(oneSquareMove);
                moveSet.emplace_back(from, oneSqTo);
                oneSquareMove.reset(oneSqTo);
            }

            //double square
            Bitboard doubleSquaresMove = ((oneSquareMoveWithPromotions & LookupTables::maskRank[RANK_6]) >> 8) & ~allPieces;
            while (doubleSquaresMove.any()) {
                const size_t dblSqTo = BitOps::findFirstBit(doubleSquaresMove);
                moveSet.emplace_back(from, dblSqTo, MoveType::DOUBLE_PAWN);
                doubleSquaresMove.reset(dblSqTo);
            }

            Bitboard attackMovesWithPromotions = LookupTables::NonSlidingAttacks::blackPawnAttacks[from] & whitePieces;

            //attack promotions
            Bitboard attackPromotions = attackMovesWithPromotions & LookupTables::maskRank[RANK_1];
            while (attackPromotions.any()) {
                const size_t attackPromTo = BitOps::findFirstBit(attackPromotions);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::QUEEN_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::ROOK_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::BISHOP_PROM);
                moveSet.emplace_back(from, attackPromTo, MoveType::CAPTURE_PROMOTION, PromotionType::KNIGHT_PROM);
                attackPromotions.reset(attackPromTo);
            }

            //attack moves
            Bitboard attackMoves = attackMovesWithPromotions & ~LookupTables::maskRank[RANK_1];
            while (attackMoves.any()) {
                const size_t attackTo = BitOps::findFirstBit(attackMoves);
                moveSet.emplace_back(from, attackTo);
                attackMoves.reset(attackTo);
            }

            //enPassant
            Bitboard possibleEnPassant = LookupTables::NonSlidingAttacks::blackPawnAttacks[from] & ~allPieces;
            while (possibleEnPassant.any()) {
                const size_t enPassantTo = BitOps::findFirstBit(possibleEnPassant);
                if (attackableWhitePawnsEnPassant[enPassantTo - 8]) {
                    moveSet.emplace_back(from, enPassantTo, MoveType::EN_PASSANT);
                }
                possibleEnPassant.reset(enPassantTo);
            }

            pawns.reset(from);
        }
    }

    static MoveSet &genWhitePseudoMoveSetWithoutCastling(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.whiteKing, board.allWhitePieces);
        generateKnightMoves(moveSet, board.whiteKnights, board.allWhitePieces);
        generateRookMoves(moveSet, board.whiteRooks, board.allWhitePieces, board.allPieces);
        generateBishopMoves(moveSet, board.whiteBishops, board.allWhitePieces, board.allPieces);
        generateQueenMoves(moveSet, board.whiteQueens, board.allWhitePieces, board.allPieces);
        generateWhitePawnMoves(moveSet, board.whitePawns, board.allBlackPieces, board.allPieces, board.attackableBlackPawnsEnPassant);

        return moveSet;
    }
    static MoveSet &genBlackPseudoMoveSetWithoutCastling(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.blackKing, board.allBlackPieces);
        generateKnightMoves(moveSet, board.blackKnights, board.allBlackPieces);
        generateRookMoves(moveSet, board.blackRooks, board.allBlackPieces, board.allPieces);
        generateBishopMoves(moveSet, board.blackBishops, board.allBlackPieces, board.allPieces);
        generateQueenMoves(moveSet, board.blackQueens, board.allBlackPieces, board.allPieces);
        generateBlackPawnMoves(moveSet, board.blackPawns, board.allWhitePieces, board.allPieces, board.attackableBlackPawnsEnPassant);

        return moveSet;
    }

    static MoveSet &genWhitePseudoMoveSet(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.whiteKing, board.allWhitePieces);
        generateKnightMoves(moveSet, board.whiteKnights, board.allWhitePieces);
        generateRookMoves(moveSet, board.whiteRooks, board.allWhitePieces, board.allPieces);
        generateBishopMoves(moveSet, board.whiteBishops, board.allWhitePieces, board.allPieces);
        generateQueenMoves(moveSet, board.whiteQueens, board.allWhitePieces, board.allPieces);
        generateWhitePawnMoves(moveSet, board.whitePawns, board.allBlackPieces, board.allPieces, board.attackableBlackPawnsEnPassant);

        generateCastling(moveSet, board);

        return moveSet;
    }
    static MoveSet &genBlackPseudoMoveSet(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.blackKing, board.allBlackPieces);
        generateKnightMoves(moveSet, board.blackKnights, board.allBlackPieces);
        generateRookMoves(moveSet, board.blackRooks, board.allBlackPieces, board.allPieces);
        generateBishopMoves(moveSet, board.blackBishops, board.allBlackPieces, board.allPieces);
        generateQueenMoves(moveSet, board.blackQueens, board.allBlackPieces, board.allPieces);
        generateBlackPawnMoves(moveSet, board.blackPawns, board.allWhitePieces, board.allPieces, board.attackableBlackPawnsEnPassant);

        generateCastling(moveSet, board);

        return moveSet;
    }

    static void generateCastling(MoveSet &t_moveSet, const Board &board)
    {
        MoveSet inactivePlayerMoveSet;
        bool qSideCastling = true, kSideCastling = true;
        if (board.inactivePlayer == Color::WHITE) { // active BLACK
            genWhitePseudoMoveSetWithoutCastling(inactivePlayerMoveSet, board);

            // is squares between rook and king in check? is king in check?
            for (auto move : inactivePlayerMoveSet) {
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
                t_moveSet.emplace_back(60, 62, MoveType::CASTLING, CastlingType::BLACK_KSIDE); //add kSideCastling for black
            if (qSideCastling && board.isBlackQSideCastlingPossible() && board.isBlackQSideCastlingRightsValid())
                t_moveSet.emplace_back(60, 58, MoveType::CASTLING, CastlingType::BLACK_QSIDE); //add qSideCastling for black
        }
        else { // active WHITE
            genBlackPseudoMoveSetWithoutCastling(inactivePlayerMoveSet, board);

            // is squares between rook and king in check? is king in check?
            for (auto move : inactivePlayerMoveSet) {
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
                t_moveSet.emplace_back(4, 6, MoveType::CASTLING, CastlingType::WHITE_KSIDE); //add kSideCastling for white
            if (qSideCastling && board.isWhiteQSideCastlingPossible() && board.isWhiteQSideCastlingRightsValid())
                t_moveSet.emplace_back(4, 2, MoveType::CASTLING, CastlingType::WHITE_QSIDE); //add qSideCastling for white
        }
    }

public:
    static MoveSet generateMoveSet(const Board &board, const Color &color)
    {
        MoveSet moveSet;
        return color == Color::WHITE ? genWhitePseudoMoveSet(moveSet, board) : genBlackPseudoMoveSet(moveSet, board);
    }
};
#endif //POORMANSENGINE_MOVEGENERATOR_H

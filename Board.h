//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef POORMANSENGINE_BOARD_H
#define POORMANSENGINE_BOARD_H

#include "common.h"
#include "Move.h"
#include <array>

class Board {
    Bitboard allWhitePieces;
    Bitboard allBlackPieces;
    Bitboard allPieces;

    Bitboard &getPieceBitboard(const Color &color, const PieceType &pieceType) {
        if (color == Color::WHITE) {
            switch (pieceType) {
                case PieceType::PAWN: return whitePawns;
                case PieceType::ROOK: return whiteRooks;
                case PieceType::KNIGHT: return whiteKnights;
                case PieceType::BISHOP: return whiteBishops;
                case PieceType::QUEEN: return whiteQueens;
                case PieceType::KING: return whiteKing;
            }
        } else {
            switch (pieceType) {
                case PieceType::PAWN: return blackPawns;
                case PieceType::ROOK: return blackRooks;
                case PieceType::KNIGHT: return blackKnights;
                case PieceType::BISHOP: return blackBishops;
                case PieceType::QUEEN: return blackQueens;
                case PieceType::KING: return blackKing;
            }
        }
    }

public:
    Bitboard whitePawns     {"0000000000000000000000000000000000000000000000001111111100000000"};
    Bitboard whiteRooks     {"0000000000000000000000000000000000000000000000000000000010000001"};
    Bitboard whiteKnights   {"0000000000000000000000000000000000000000000000000000000001000010"};
    Bitboard whiteBishops   {"0000000000000000000000000000000000000000000000000000000000100100"};
    Bitboard whiteQueens    {"0000000000000000000000000000000000000000000000000000000000001000"};
    Bitboard whiteKing      {"0000000000000000000000000000000000000000000000000000000000010000"};
    Bitboard blackPawns     {"0000000011111111000000000000000000000000000000000000000000000000"};
    Bitboard blackRooks     {"1000000100000000000000000000000000000000000000000000000000000000"};
    Bitboard blackKnights   {"0100001000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackBishops   {"0010010000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackQueens    {"0000100000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackKing      {"0001000000000000000000000000000000000000000000000000000000000000"};

    // it will be updated after every move
    Color activePlayer, inactivePlayer;

    // todo: it is reset before updated when a pawn make double move
    Bitboard attackableWhitePawnsEnPassant;
    Bitboard attackableBlackPawnsEnPassant;

    //todo: it is updated after every move, moves are made using here
    std::array<CastlingType, 4> castlingRights;

    int halfMoveClock = 0; //50 repetition rule

    bool isWhiteKSideCastlingRightsValid() const {
        return castlingRights[0] != CastlingType::NONE;
    }
    bool isWhiteQSideCastlingRightsValid() const {
        return castlingRights[1] != CastlingType::NONE;
    }
    bool isBlackKSideCastlingRightsValid() const {
        return castlingRights[2] != CastlingType::NONE;
    }
    bool isBlackQSideCastlingRightsValid() const {
        return castlingRights[3] != CastlingType::NONE;
    }

    bool isWhiteKSideCastlingPossible() const
    {
        if (getAllPieces().test(5) || getAllPieces().test(6))
            return false;
        return true;
    }
    bool isWhiteQSideCastlingPossible() const
    {
        if (getAllPieces().test(3) || getAllPieces().test(2) || getAllPieces().test(1))
            return false;
        return true;
    }
    bool isBlackKSideCastlingPossible() const
    {
        if (getAllPieces().test(61) || getAllPieces().test(62))
            return false;
        return true;
    }
    bool isBlackQSideCastlingPossible() const
    {
        if (getAllPieces().test(59) || getAllPieces().test(58) || getAllPieces().test(57))
            return false;
        return true;
    }

    Board()
    {
        allWhitePieces = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
        allBlackPieces = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
        allPieces = allWhitePieces | allBlackPieces;
        activePlayer = Color::WHITE;
        inactivePlayer = Color::BLACK;
        castlingRights[0] = whiteKing.test(4) && whiteRooks.test(7) ? CastlingType::WHITE_KSIDE : CastlingType::NONE;
        castlingRights[1] = whiteKing.test(4) && whiteRooks.test(0) ? CastlingType::WHITE_QSIDE : CastlingType::NONE;
        castlingRights[2] = blackKing.test(60) && whiteRooks.test(63) ? CastlingType::BLACK_KSIDE : CastlingType::NONE;
        castlingRights[3] = blackKing.test(60) && whiteRooks.test(56) ? CastlingType::BLACK_QSIDE : CastlingType::NONE;
    }

    Bitboard getAllWhitePieces() const { return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing; }
    Bitboard getAllBlackPieces() const { return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing; }
    Bitboard getAllPieces() const { return getAllWhitePieces() | getAllBlackPieces(); }

    void makeMove(Move move)
    {
        size_t from = move.getFrom(), to = move.getTo();
        PieceType capturedPieceType = PieceType::NONE;

        //it must be reset every move because en passant must be made right after opposite's double pawn move.
        attackableBlackPawnsEnPassant.reset();
        attackableWhitePawnsEnPassant.reset();

        //detect captured type for update captured type bitboard
        if (move.getMoveType() == MoveType::CAPTURE || move.getMoveType() == MoveType::CAPTURE_PROMOTION) {
            if (whitePawns[to] || blackPawns[to]) capturedPieceType = PieceType::PAWN;
            else if (whiteRooks[to] || blackRooks[to]) capturedPieceType = PieceType::ROOK;
            else if (whiteKnights[to] || blackKnights[to]) capturedPieceType = PieceType::KNIGHT;
            else if (whiteBishops[to] || blackBishops[to]) capturedPieceType = PieceType::BISHOP;
            else if (whiteQueens[to] || blackQueens[to]) capturedPieceType = PieceType::QUEEN;
            else if (whiteKing[to] || blackKing[to]) capturedPieceType = PieceType::KING; //???
        } else if (move.getMoveType() == MoveType::EN_PASSANT) {
            capturedPieceType = PieceType::PAWN;
        }

        if (move.getMoveType() == MoveType::CAPTURE || move.getMoveType() == MoveType::CAPTURE_PROMOTION) halfMoveClock = 0; //50 repetition rule

        //update bitboards by piece type made move
        if (activePlayer == Color::WHITE) {
            switch (move.getPieceType()) {
                case PieceType::PAWN: {
                    if (move.getMoveType() == MoveType::QUIET) {
                        whitePawns.reset(from);
                        whitePawns.set(to);
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE) {
                        whitePawns.reset(from);
                        whitePawns.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    }
                    else if (move.getMoveType() == MoveType::DOUBLE_PAWN) {
                        whitePawns.reset(from);
                        whitePawns.set(to);
                        attackableWhitePawnsEnPassant.set(to);
                    }
                    else if (move.getMoveType() == MoveType::PROMOTION) {
                        if (move.getPromotionType() == PromotionType::QUEEN_PROM) {
                            whitePawns.reset(from);
                            whiteQueens.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::ROOK_PROM) {
                            whitePawns.reset(from);
                            whiteRooks.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::BISHOP_PROM) {
                            whitePawns.reset(from);
                            whiteBishops.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::KNIGHT_PROM) {
                            whitePawns.reset(from);
                            whiteKnights.set(to);
                        }
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE_PROMOTION) {
                        if (move.getPromotionType() == PromotionType::QUEEN_PROM) {
                            whitePawns.reset(from);
                            whiteQueens.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::ROOK_PROM) {
                            whitePawns.reset(from);
                            whiteRooks.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::BISHOP_PROM) {
                            whitePawns.reset(from);
                            whiteBishops.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::KNIGHT_PROM) {
                            whitePawns.reset(from);
                            whiteKnights.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                    }
                    else if (move.getMoveType() == MoveType::EN_PASSANT) {
                        whitePawns.reset(from);
                        whitePawns.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to + 8);
                    }
                    halfMoveClock = 0;
                    break;
                }
                case PieceType::KNIGHT: {
                    whiteKnights.reset(from);
                    whiteKnights.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::ROOK: {
                    //update castling rights
                    if (from == 0) castlingRights[1] = CastlingType::NONE;
                    else if (from == 7) castlingRights[0] = CastlingType::NONE;

                    whiteRooks.reset(from);
                    whiteRooks.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::BISHOP: {
                    whiteBishops.reset(from);
                    whiteBishops.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::QUEEN: {
                    whiteQueens.reset(from);
                    whiteQueens.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::KING: {
                    castlingRights[0] = castlingRights[1] = CastlingType::NONE; //update castling rights

                    if (move.getMoveType() == MoveType::QUIET) {
                        whiteKing.reset(from);
                        whiteKing.set(to);
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE) {
                        whiteKing.reset(from);
                        whiteKing.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    }
                    else if (move.getMoveType() == MoveType::CASTLING) {
                        if (move.getCastlingType() == CastlingType::WHITE_KSIDE) {
                            whiteRooks.set(5);
                            whiteRooks.reset(7);
                            whiteKing.set(6);
                            whiteKing.reset(4);
                        }
                        else if (move.getCastlingType() == CastlingType::WHITE_QSIDE) {
                            whiteRooks.set(3);
                            whiteRooks.reset(0);
                            whiteKing.set(2);
                            whiteKing.reset(4);
                        }
                    }
                    break;
                }
                default: std::cout << "MAKE MOVE ERROR!!"; // it will be tested before
            }
        }
        else {
            switch (move.getPieceType()) {
                case PieceType::PAWN: {
                    if (move.getMoveType() == MoveType::QUIET) {
                        blackPawns.reset(from);
                        blackPawns.set(to);
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE) {
                        blackPawns.reset(from);
                        blackPawns.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    }
                    else if (move.getMoveType() == MoveType::DOUBLE_PAWN) {
                        blackPawns.reset(from);
                        blackPawns.set(to);
                        attackableBlackPawnsEnPassant.set(to);
                    }
                    else if (move.getMoveType() == MoveType::PROMOTION) {
                        if (move.getPromotionType() == PromotionType::QUEEN_PROM) {
                            blackPawns.reset(from);
                            blackQueens.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::ROOK_PROM) {
                            blackPawns.reset(from);
                            blackRooks.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::BISHOP_PROM) {
                            blackPawns.reset(from);
                            blackBishops.set(to);
                        }
                        else if (move.getPromotionType() == PromotionType::KNIGHT_PROM) {
                            blackPawns.reset(from);
                            blackKnights.set(to);
                        }
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE_PROMOTION) {
                        if (move.getPromotionType() == PromotionType::QUEEN_PROM) {
                            blackPawns.reset(from);
                            blackQueens.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::ROOK_PROM) {
                            blackPawns.reset(from);
                            blackRooks.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::BISHOP_PROM) {
                            blackPawns.reset(from);
                            blackBishops.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                        else if (move.getPromotionType() == PromotionType::KNIGHT_PROM) {
                            blackPawns.reset(from);
                            blackKnights.set(to);
                            getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                        }
                    }
                    else if (move.getMoveType() == MoveType::EN_PASSANT) {
                        blackPawns.reset(from);
                        blackPawns.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to - 8);
                    }
                    break;
                }
                case PieceType::KNIGHT: {
                    blackKnights.reset(from);
                    blackKnights.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::ROOK: {
                    //update castling rights
                    if (from == 56) castlingRights[3] = CastlingType::NONE;
                    else if (from == 63) castlingRights[2] = CastlingType::NONE;

                    blackRooks.reset(from);
                    blackRooks.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::BISHOP: {
                    blackBishops.reset(from);
                    blackBishops.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::QUEEN: {
                    blackQueens.reset(from);
                    blackQueens.set(to);
                    if (move.getMoveType() == MoveType::CAPTURE) getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    break;
                }
                case PieceType::KING: {
                    castlingRights[2] = castlingRights[3] = CastlingType::NONE; //update castling rights

                    if (move.getMoveType() == MoveType::QUIET) {
                        blackKing.reset(from);
                        blackKing.set(to);
                    }
                    else if (move.getMoveType() == MoveType::CAPTURE) {
                        blackKing.reset(from);
                        blackKing.set(to);
                        getPieceBitboard(inactivePlayer, capturedPieceType).reset(to);
                    }
                    else if (move.getMoveType() == MoveType::CASTLING) {
                        if (move.getCastlingType() == CastlingType::BLACK_KSIDE) {
                            blackRooks.set(61);
                            blackRooks.reset(63);
                            blackKing.set(62);
                            blackKing.reset(60);
                        }
                        else if (move.getCastlingType() == CastlingType::BLACK_QSIDE) {
                            blackRooks.set(59);
                            blackRooks.reset(56);
                            blackKing.set(58);
                            blackKing.reset(60);
                        }
                    }
                    break;
                }
                default: std::cout << "MAKE MOVE ERROR!!"; // it will be tested before
            }
        }

        halfMoveClock++;
        changeActivePlayer();
    }

    void changeActivePlayer()
    {
        if (activePlayer == Color::WHITE) {
            activePlayer = Color::BLACK;
            inactivePlayer = Color::WHITE;
        }
        else {
            activePlayer = Color::WHITE;
            inactivePlayer = Color::BLACK;
        }
    }

    void displayBoard() const
    {
        std::array<PieceType, g_boardSize> board{};
        for (size_t i = 0; i < g_boardSize; ++i) {
            board[i] = PieceType::NONE;
            if (whitePawns.test(i))     board[i] = PieceType::WHITE_PAWN;
            if (blackPawns.test(i))     board[i] = PieceType::BLACK_PAWN;
            if (whiteKnights.test(i))   board[i] = PieceType::WHITE_KNIGHT;
            if (blackKnights.test(i))   board[i] = PieceType::BLACK_KNIGHT;
            if (whiteBishops.test(i))   board[i] = PieceType::WHITE_BISHOP;
            if (blackBishops.test(i))   board[i] = PieceType::BLACK_BISHOP;
            if (whiteRooks.test(i))     board[i] = PieceType::WHITE_ROOK;
            if (blackRooks.test(i))     board[i] = PieceType::BLACK_ROOK;
            if (whiteQueens.test(i))    board[i] = PieceType::WHITE_QUEEN;
            if (blackQueens.test(i))    board[i] = PieceType::BLACK_QUEEN;
            if (whiteKing.test(i))      board[i] = PieceType::WHITE_KING;
            if (blackKing.test(i))      board[i] = PieceType::BLACK_KING;
        }

        for (size_t i = g_rankSize; i > 0; --i) {
            std::cout << "\n" << i << "  ";
            for (size_t k = 0; k < g_fileSize; ++k) {
                std::cout << std::left << std::setw(5) << board[(i - 1) * g_fileSize + k] << " ";
            }
        }
        std::cout << "\n";
        for (char ch = 'a'; ch <= 'h'; ++ch)
            std::cout << "   " << std::left << std::setw(2) << ch << " ";
        std::string activePlayerStr =  activePlayer == Color::WHITE ? "WHITE" : "BLACK";
        std::cout << "\n\nTURN: " + activePlayerStr + "\n\n";
    }
};

#endif //POORMANSENGINE_BOARD_H

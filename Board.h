//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef POORMANSENGINE_BOARD_H
#define POORMANSENGINE_BOARD_H

#include "common.h"
#include "Move.h"
#include <array>

class Board {
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

    Bitboard allWhitePieces;
    Bitboard allBlackPieces;
    Bitboard allPieces;

    // todo: it will be updated after every move
    Color activePlayer, inactivePlayer;

    // todo: it will be reset before updated when a pawn make double move
    Bitboard attackableWhitePawnsEnPassant;
    Bitboard attackableBlackPawnsEnPassant;

    //todo: it will be updated after every move, moves are made using here
    std::array<CastlingType, 4> castlingRights = { CastlingType::WHITE_KSIDE, CastlingType::WHITE_QSIDE,
                                                   CastlingType::BLACK_KSIDE, CastlingType::BLACK_QSIDE };

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
    }

    Bitboard getAllWhitePieces() const { return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing; }
    Bitboard getAllBlackPieces() const { return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing; }
    Bitboard getAllPieces() const { return getAllWhitePieces() | getAllBlackPieces(); }

    void makeMove()
    {

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
        std::cout << "\n\n";
    }
};

#endif //POORMANSENGINE_BOARD_H

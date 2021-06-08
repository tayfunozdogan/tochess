//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef POORMANSENGINE_COMMON_H
#define POORMANSENGINE_COMMON_H

#include <bitset>
#include <iomanip>

typedef std::bitset<64> Bitboard;
typedef size_t BIndex;
constexpr size_t g_rankSize = 8, g_fileSize = 8;
constexpr size_t g_boardSize = g_rankSize * g_fileSize;

enum Square {
    A8, B8, C8, D8, E8, F8, G8, H8,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A1, B1, C1, D1, E1, F1, G1, H1
};
enum class PieceType {
    WHITE_PAWN,     BLACK_PAWN,
    WHITE_ROOK,     BLACK_ROOK,
    WHITE_KNIGHT,   BLACK_KNIGHT,
    WHITE_BISHOP,   BLACK_BISHOP,
    WHITE_QUEEN,    BLACK_QUEEN,
    WHITE_KING,     BLACK_KING,

    PAWN, ROOK, KNIGHT, BISHOP, QUEEN, KING, // todo: must be refactored

    NONE
};
std::ostream& operator<< (std::ostream &os, const PieceType &pieceType)
{
    switch (pieceType) {
        case PieceType::WHITE_PAWN: os << "wp"; break;
        case PieceType::WHITE_KNIGHT: os << "WN"; break;
        case PieceType::WHITE_BISHOP: os << "WB"; break;
        case PieceType::WHITE_ROOK: os << "WR"; break;
        case PieceType::WHITE_QUEEN: os << "WQ"; break;
        case PieceType::WHITE_KING: os << "WK"; break;
        case PieceType::BLACK_PAWN: os << "bp"; break;
        case PieceType::BLACK_KNIGHT: os << "BN"; break;
        case PieceType::BLACK_BISHOP: os << "BB"; break;
        case PieceType::BLACK_ROOK: os << "BR"; break;
        case PieceType::BLACK_QUEEN: os << "BQ"; break;
        case PieceType::BLACK_KING: os << "BK"; break;
        case PieceType::NONE: os << "O "; break;
    }

    return os;
}

enum FileRank {
    FILE_A = 0, RANK_1 = 0,
    FILE_B = 1, RANK_2 = 1,
    FILE_C = 2, RANK_3 = 2,
    FILE_D = 3, RANK_4 = 3,
    FILE_E = 4, RANK_5 = 4,
    FILE_F = 5, RANK_6 = 5,
    FILE_G = 6, RANK_7 = 6,
    FILE_H = 7, RANK_8 = 7
};

enum class Color {
    WHITE, BLACK
};

class BitOps {
private:
    constexpr static const int index64[64] = {
            0,  1, 48,  2, 57, 49, 28,  3,
            61, 58, 50, 42, 38, 29, 17,  4,
            62, 55, 59, 36, 53, 51, 43, 22,
            45, 39, 33, 30, 24, 18, 12,  5,
            63, 47, 56, 27, 60, 41, 37, 16,
            54, 35, 52, 21, 44, 32, 23, 11,
            46, 26, 40, 15, 34, 20, 31, 10,
            25, 14, 19,  9, 13,  8,  7,  6
    };
    constexpr static const uint64_t debruijn64 = 0x03f79d71b4cb0a89;
public:
    static constexpr uint64_t ZERO = 0ULL;
    static constexpr uint64_t ONE = 1ULL;

    /**
     * @param bb bitboard to scan
     * @precondition bb != 0
     * @return index (0..63) of least significant one bit
     */
    static size_t findFirstBit(const Bitboard &bb)
    {
        auto ull = bb.to_ullong();
        assert (ull != 0);
        return index64[((ull & -ull) * debruijn64) >> 58U];
    }
};

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


#endif //POORMANSENGINE_COMMON_H

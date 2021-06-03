#include <iostream>
#include <bitset>
#include <array>
#include <iomanip>
#include <vector>
#include <strings.h>
#include <algorithm>

typedef std::bitset<64> Bitboard;
typedef std::bitset<6> BIndex;
constexpr size_t g_rankSize = 8, g_fileSize = 8;
constexpr size_t g_boardSize = g_rankSize * g_fileSize;
template <size_t I> using MagicTable = std::array<std::array<Bitboard, I>, g_boardSize>;

enum class Color {
    WHITE, BLACK
};

enum class MoveType {
    REGULAR, QUIET, CAPTURE, PROMOTION, CAPTURE_PROMOTION, DOUBLE_PAWN, EN_PASSANT, K_CASTLING, Q_CASTLING
};

enum class PromotionType {
    NONE, QUEEN_PROM, ROOK_PROM, KNIGHT_PROM, BISHOP_PROM
};

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

    NONE
};
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

const uint64_t rookMagics[g_boardSize] = {
        0xa8002c000108020, 0x6c00049b0002001, 0x100200010090040, 0x2480041000800801, 0x280028004000800,
        0x900410008040022, 0x280020001001080, 0x2880002041000080, 0xa000800080400034, 0x4808020004000,
        0x2290802004801000, 0x411000d00100020, 0x402800800040080, 0xb000401004208, 0x2409000100040200,
        0x1002100004082, 0x22878001e24000, 0x1090810021004010, 0x801030040200012, 0x500808008001000,
        0xa08018014000880, 0x8000808004000200, 0x201008080010200, 0x801020000441091, 0x800080204005,
        0x1040200040100048, 0x120200402082, 0xd14880480100080, 0x12040280080080, 0x100040080020080,
        0x9020010080800200, 0x813241200148449, 0x491604001800080, 0x100401000402001, 0x4820010021001040,
        0x400402202000812, 0x209009005000802, 0x810800601800400, 0x4301083214000150, 0x204026458e001401,
        0x40204000808000, 0x8001008040010020, 0x8410820820420010, 0x1003001000090020, 0x804040008008080,
        0x12000810020004, 0x1000100200040208, 0x430000a044020001, 0x280009023410300, 0xe0100040002240,
        0x200100401700, 0x2244100408008080, 0x8000400801980, 0x2000810040200, 0x8010100228810400,
        0x2000009044210200, 0x4080008040102101, 0x40002080411d01, 0x2005524060000901, 0x502001008400422,
        0x489a000810200402, 0x1004400080a13, 0x4000011008020084, 0x26002114058042
};

const uint64_t bishopMagics[g_boardSize] = {
        0x89a1121896040240, 0x2004844802002010, 0x2068080051921000, 0x62880a0220200808, 0x4042004000000,
        0x100822020200011, 0xc00444222012000a, 0x28808801216001, 0x400492088408100, 0x201c401040c0084,
        0x840800910a0010, 0x82080240060, 0x2000840504006000, 0x30010c4108405004, 0x1008005410080802,
        0x8144042209100900, 0x208081020014400, 0x4800201208ca00, 0xf18140408012008, 0x1004002802102001,
        0x841000820080811, 0x40200200a42008, 0x800054042000, 0x88010400410c9000, 0x520040470104290,
        0x1004040051500081, 0x2002081833080021, 0x400c00c010142, 0x941408200c002000, 0x658810000806011,
        0x188071040440a00, 0x4800404002011c00, 0x104442040404200, 0x511080202091021, 0x4022401120400,
        0x80c0040400080120, 0x8040010040820802, 0x480810700020090, 0x102008e00040242, 0x809005202050100,
        0x8002024220104080, 0x431008804142000, 0x19001802081400, 0x200014208040080, 0x3308082008200100,
        0x41010500040c020, 0x4012020c04210308, 0x208220a202004080, 0x111040120082000, 0x6803040141280a00,
        0x2101004202410000, 0x8200000041108022, 0x21082088000, 0x2410204010040, 0x40100400809000,
        0x822088220820214, 0x40808090012004, 0x910224040218c9, 0x402814422015008, 0x90014004842410,
        0x1000042304105, 0x10008830412a00, 0x2520081090008908, 0x40102000a0a60140,
};
constexpr size_t rookBitsCnt[g_boardSize] = {
        12, 11, 11, 11, 11, 11, 11, 12,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        11, 10, 10, 10, 10, 10, 10, 11,
        12, 11, 11, 11, 11, 11, 11, 12
};

constexpr size_t bishopBitsCnt[g_boardSize] = {
        6, 5, 5, 5, 5, 5, 5, 6,
        5, 5, 5, 5, 5, 5, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 9, 9, 7, 5, 5,
        5, 5, 7, 7, 7, 7, 5, 5,
        5, 5, 5, 5, 5, 5, 5, 5,
        6, 5, 5, 5, 5, 5, 5, 6
};
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

    Bitboard attackableWhitePawnsEnPassant; // todo: it will be reset before updated when a pawn make double move
    Bitboard attackableBlackPawnsEnPassant; // todo: it will be reset before updated when a pawn make double move

    Board()
    {
        allWhitePieces = whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing;
        allBlackPieces = blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing;
        allPieces = allWhitePieces | allBlackPieces;
    }

    Bitboard getAllWhitePieces() const { return whitePawns | whiteRooks | whiteKnights | whiteBishops | whiteQueens | whiteKing; }
    Bitboard getAllBlackPieces() const { return blackPawns | blackRooks | blackKnights | blackBishops | blackQueens | blackKing; }
    Bitboard getAllPieces() const { return allWhitePieces | allBlackPieces; }

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

namespace LookupTables {
    std::array<Bitboard, g_rankSize> clearRank, clearFile, maskRank, maskFile;
    std::array<Bitboard, g_boardSize> north, south, west, east, northWest, northEast, southWest, southEast;
    std::array<Bitboard, g_boardSize> rookMasks, bishopMasks;

    namespace NonSlidingAttacks {
        std::array<Bitboard, g_boardSize> knightAttacks, kingAttacks;
        std::array<Bitboard, g_boardSize> whitePawnAttacks, blackPawnAttacks;

        void initKnightAttacks()
        {
            Bitboard clippedSquare1 = LookupTables::clearFile[FILE_A] & LookupTables::clearFile[FILE_B];
            Bitboard clippedSquare2 = LookupTables::clearFile[FILE_A];
            Bitboard clippedSquare3 = LookupTables::clearFile[FILE_H];
            Bitboard clippedSquare4 = LookupTables::clearFile[FILE_H] & LookupTables::clearFile[FILE_G];

            Bitboard clippedSquare5 = LookupTables::clearFile[FILE_H] & LookupTables::clearFile[FILE_G];
            Bitboard clippedSquare6 = LookupTables::clearFile[FILE_H];
            Bitboard clippedSquare7 = LookupTables::clearFile[FILE_A];
            Bitboard clippedSquare8 = LookupTables::clearFile[FILE_A] & LookupTables::clearFile[FILE_B];

            for (size_t i = 0; i < g_boardSize; ++i) {
                Bitboard loc; loc.set(i);
                Bitboard square1 = (loc & clippedSquare1) << 6;
                Bitboard square2 = (loc & clippedSquare2) << 15;
                Bitboard square3 = (loc & clippedSquare3) << 17;
                Bitboard square4 = (loc & clippedSquare4) << 10;
                Bitboard square5 = (loc & clippedSquare5) >> 6;
                Bitboard square6 = (loc & clippedSquare6) >> 15;
                Bitboard square7 = (loc & clippedSquare7) >> 17;
                Bitboard square8 = (loc & clippedSquare8) >> 10;

                knightAttacks[i] = square1 | square2 | square3 | square4 | square5 | square6 | square7 | square8;
            }
        }
        void initKingAttacks()
        {
            for (size_t i = 0; i < g_boardSize; ++i) {
                Bitboard loc; loc.set(i);
                Bitboard kingClippedFileH = loc & LookupTables::clearFile[FILE_H];
                Bitboard kingClippedFileA = loc & LookupTables::clearFile[FILE_A];
                Bitboard square1 = kingClippedFileH >> 7;
                Bitboard square2 = kingClippedFileH << 9;
                Bitboard square3 = kingClippedFileH << 1;
                Bitboard square4 = kingClippedFileA << 7;
                Bitboard square5 = kingClippedFileA >> 9;
                Bitboard square6 = kingClippedFileA >> 1;
                Bitboard square7 = loc << 8;
                Bitboard square8 = loc >> 8;

                kingAttacks[i] = square1 | square2 | square3 | square4 | square5 | square6 | square7 | square8;
            }
        }
        void initPawnAttacks()
        {
            for (size_t i = 0; i < g_boardSize; ++i) {
                Bitboard loc; loc.set(i);
                Bitboard whiteNorthWest = (loc & LookupTables::clearFile[FILE_A]) << 7;
                Bitboard whiteNorthEast = (loc & LookupTables::clearFile[FILE_H]) << 9;
                Bitboard blackSouthWest = (loc & LookupTables::clearFile[FILE_A]) >> 7;
                Bitboard blackSouthEast = (loc & LookupTables::clearFile[FILE_H]) >> 9;
                whitePawnAttacks[i] = whiteNorthWest | whiteNorthEast;
                blackPawnAttacks[i] = blackSouthWest | blackSouthEast;
            }
        }
    }
    namespace SlidingAttacks {
        MagicTable<4096> rookMagicTable;
        MagicTable<1024> bishopMagicTable;

        Bitboard setRookPossibleAttacks(const size_t &square, const Bitboard &occupancy) {
            Bitboard possibleAttacks{0};

            int pieceRank = static_cast<int>(square / g_rankSize), pieceFile = static_cast<int>(square %
                                                                                                g_fileSize);

            for (int rank = pieceRank - 1; rank >= RANK_1; --rank) {
                const size_t actualSq = rank * 8 + pieceFile;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1; rank <= RANK_8; ++rank) {
                const size_t actualSq = rank * 8 + pieceFile;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int file = pieceFile - 1; file >= FILE_A; --file) {
                const size_t actualSq = pieceRank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int file = pieceFile + 1; file <= FILE_H; ++file) {
                const size_t actualSq = pieceRank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            return possibleAttacks;
        }
        Bitboard setBishopPossibleAttacks(const size_t &square, const Bitboard &occupancy) {
            Bitboard possibleAttacks{0};

            int pieceRank = static_cast<int>(square / g_rankSize), pieceFile = static_cast<int>(square %
                                                                                                g_fileSize);

            for (int rank = pieceRank - 1, file = pieceFile - 1; rank >= RANK_1 && file >= FILE_A; --rank, --file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank - 1, file = pieceFile + 1; rank >= RANK_1 && file <= FILE_H; --rank, ++file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1, file = pieceFile - 1; rank <= RANK_8 && file >= FILE_A; ++rank, --file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1, file = pieceFile + 1; rank <= RANK_8 && file <= FILE_H; ++rank, ++file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (occupancy.test(actualSq)) break;
            }

            return possibleAttacks;
        }
        Bitboard setOccupancy(const size_t &square, const size_t &count, const std::vector<size_t> &setBitsIndexes) {
            Bitboard occupancy{0}, variation{count};

            for (size_t i = 0; i < setBitsIndexes.size(); ++i) {
                occupancy[setBitsIndexes[i]] = variation[i];
            }

            return occupancy;
        }
        auto setSetBitsIndexes(const Bitboard &bb) {
            std::vector<size_t> setBitsIndexes;
            for (size_t i = 0; i < g_boardSize; ++i) {
                if (bb.test(i)) setBitsIndexes.push_back(i);
            }
            return setBitsIndexes;
        }

        void initRookMagicTable()
        {
            for (size_t sq = 0; sq < g_boardSize; ++sq) {
                std::vector<size_t> maskSetBitsIndexes = setSetBitsIndexes(rookMasks[sq]);

                for (size_t cnt = 0; cnt < (BitOps::ONE << rookBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * rookMagics[sq]) >> (g_boardSize - rookBitsCnt[sq]);
                    rookMagicTable[sq][magicKey] = setRookPossibleAttacks(sq, occupancy);
                }
            }
        }
        void initBishopMagicTable()
        {
            for (size_t sq = 0; sq < g_boardSize; ++sq) {
                std::vector<size_t> maskSetBitsIndexes = setSetBitsIndexes(bishopMasks[sq]);

                for (size_t cnt = 0; cnt < (1ULL << bishopBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * bishopMagics[sq]) >> (g_boardSize - bishopBitsCnt[sq]);
                    bishopMagicTable[sq][magicKey] = setBishopPossibleAttacks(sq, occupancy);
                }
            }
        }
    }

    void initClearsAndMasks()
    {
        for (size_t i = 0; i < g_rankSize; ++i) {
            clearRank[i].set();
            maskRank[i].reset();
            clearFile[i].set();
            maskFile[i].reset();
            for (size_t k = 0; k < g_fileSize; ++k) {
                clearRank[i].set((i * 8) + k, false);
                maskRank[i].set((i * 8) + k, true);
                clearFile[i].set((k * 8) + i, false);
                maskFile[i].set((k * 8) + i, true);
            }
        }
    }
    void initDirections()
    {
        for (size_t i = 0; i < g_boardSize; ++i) {
            north[i][i] = south[i][i] = west[i][i] = east[i][i] =
            northEast[i][i] = northWest[i][i] = southEast[i][i] = southWest[i][i] = true;
            for (size_t k = 0; k < g_boardSize; ++k) {
                if (k % 9 == i % 9) {
                    if (k < i && k % 8 < i % 8)
                        northWest[i][k] = true;
                    if (k > i && k % 8 > i % 8)
                        southEast[i][k] = true;
                }
                if (k % 7 == i % 7) {
                    if (k < i && k % 8 > i % 8)
                        northEast[i][k] = true;
                    if (k > i && k % 8 < i % 8)
                        southWest[i][k] = true;
                }
                if (k % 8 == i % 8) {
                    if (k < i)
                        north[i][k] = true;
                    if (k > i)
                        south[i][k] = true;
                }
                if (k / 8 == i / 8) {
                    if (k < i)
                        west[i][k] = true;
                    if (k > i)
                        east[i][k] = true;
                }
            }
        }
    }
    void initRookMasks()
    {
        for (size_t i = 0; i < g_boardSize; ++i) {
            rookMasks[i] =
                    (north[i] & clearRank[RANK_1]) |
                    (south[i] & clearRank[RANK_8]) |
                    (west[i] & clearFile[FILE_A]) |
                    (east[i] & clearFile[FILE_H]);
            rookMasks[i][i] = false;
        }
    }
    void initBishopMasks()
    {
        Bitboard edges = clearRank[RANK_1] & clearRank[RANK_8] & clearFile[FILE_H] & clearFile[FILE_A];
        for (size_t i = 0; i < g_boardSize; ++i) {
            bishopMasks[i] =
                    (northWest[i] | northEast[i] | southWest[i] | southEast[i]) & edges;
            bishopMasks[i][i] = false;
        }
    }
    void initSlidingAttacks()
    {
        SlidingAttacks::initRookMagicTable();
        SlidingAttacks::initBishopMagicTable();
    }
    void initNonSlidingAttacks()
    {
        NonSlidingAttacks::initPawnAttacks();
        NonSlidingAttacks::initKnightAttacks();
        NonSlidingAttacks::initKingAttacks();
    }

    void init() {
        initClearsAndMasks();
        initDirections();
        initRookMasks();
        initBishopMasks();
        initSlidingAttacks();
        initNonSlidingAttacks();
    }
}

class Move {
public:
    Move(BIndex t_from, BIndex t_to, MoveType t_moveType = MoveType::REGULAR, PromotionType t_promotionType = PromotionType::NONE)
    : from(t_from), to(t_to), moveType(t_moveType), promotionType(t_promotionType) {};
    BIndex getFrom() const { return from; }
    BIndex getTo() const { return to; }
    MoveType getMoveType() const { return moveType; }
    PromotionType getPromotionType() const { return promotionType; }
    void setFrom(BIndex t_from) { from = t_from; }
    void setTo(BIndex t_to) { to = t_to; }
    void setMoveType(MoveType t_moveType) { moveType = t_moveType; }
    void setPromotionType(PromotionType t_promotionType) { promotionType = t_promotionType; }
private:
    BIndex from, to;
    MoveType moveType;
    PromotionType promotionType;
};

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

    static MoveSet &genWhitePseudoMoveSet(MoveSet &moveSet, const Board &board)
    {
        generateKingMoves(moveSet, board.whiteKing, board.allWhitePieces);
        generateKnightMoves(moveSet, board.whiteKnights, board.allWhitePieces);
        generateRookMoves(moveSet, board.whiteRooks, board.allWhitePieces, board.allPieces);
        generateBishopMoves(moveSet, board.whiteBishops, board.allWhitePieces, board.allPieces);
        generateQueenMoves(moveSet, board.whiteQueens, board.allWhitePieces, board.allPieces);
        generateWhitePawnMoves(moveSet, board.whitePawns, board.allBlackPieces, board.allPieces, board.attackableBlackPawnsEnPassant);

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

        return moveSet;
    }

public:
    static MoveSet generateMoveSet(const Board &board, const Color &color)
    {
        MoveSet moveSet;
        return color == Color::WHITE ? genWhitePseudoMoveSet(moveSet, board) : genBlackPseudoMoveSet(moveSet, board);
    }
};

void displayMoveSet(const MoveSet &ms)
{
    std::cout << "size = " << ms.size() << "\n";
    std::for_each(ms.cbegin(), ms.cend(),
                  [](const Move& move)
                  {
                      static int cnt = 0;
                      if (cnt % 10 == 0)
                          std::cout << "\n";
                      std::cout << "Move: " << (char)('A' + move.getFrom().to_ulong() % 8) << (char)('1' + move.getFrom().to_ulong() / 8)
                                << "-" << (char)('A' + move.getTo().to_ulong() % 8) << (char)('1' + move.getTo().to_ulong() / 8) << ",  ";
                      ++cnt;
                  });
}

int main()
{
    LookupTables::init();
    Board board;
    board.displayBoard();
    auto m = MoveGenerator::generateMoveSet(board, Color::WHITE);
    displayMoveSet(m);
//
//    displayBitboard(MoveGenerator::generateKingMoves(board.whiteKing, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateKnightMoves(board.whiteKnights, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateWhitePawnMoves(board.whitePawns, board.getAllBlackPieces(), board.getAllPieces()));
//    displayBitboard(MoveGenerator::generateBlackPawnMoves(board.blackPawns, board.getAllWhitePieces(), board.getAllPieces()));

    return 0;
}


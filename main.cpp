



#include <iostream>
#include <bitset>
#include <array>
#include <iomanip>
#include <vector>

typedef std::bitset<64> Bitboard;
constexpr size_t g_rankSize = 8, g_fileSize = 8;
constexpr size_t g_boardSize = g_rankSize * g_fileSize;
template <size_t I> using MagicTable = std::array<std::array<Bitboard, I>, g_boardSize>;

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
std::ostream& operator<< (std::ostream &os, const PieceType &t_pieceType)
{
    switch (t_pieceType) {
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

void displayBitboard(const Bitboard &t_bitboard)
{
//    for (size_t i = t_bitboard.size(); i > 0; --i) {
//        if (!(i % 8))
//            std::cout << "\n";
//        std::cout << t_bitboard[i - 1] << " ";
//    }
//    std::cout << "\n";
    for (size_t i = 0; i < t_bitboard.size(); ++i) {
        if (!(i % 8))
            std::cout << "\n";
        std::cout << t_bitboard[i] << " ";
    }
    std::cout << "\n";
}

const Bitboard rookMagics[g_boardSize] = {
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

const Bitboard bishopMagics[g_boardSize] = {
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
    Bitboard whiteQueens    {"0000000000000000000000000000000000000000000000000000000000010000"};
    Bitboard whiteKing      {"0000000000000000000000000000000000000000000000000000000000001000"};
    Bitboard blackPawns     {"0000000011111111000000000000000000000000000000000000000000000000"};
    Bitboard blackRooks     {"1000000100000000000000000000000000000000000000000000000000000000"};
    Bitboard blackKnights   {"0100001000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackBishops   {"0010010000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackQueens    {"0001000000000000000000000000000000000000000000000000000000000000"};
    Bitboard blackKing      {"0000100000000000000000000000000000000000000000000000000000000000"};

    Bitboard allWhitePieces;
    Bitboard allBlackPieces;
    Bitboard allPieces;

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
            for (size_t k = g_fileSize; k > 0; --k) {
                std::cout << std::left << std::setw(5) << board[(i - 1) * g_fileSize + (k - 1)] << " ";
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

    namespace Attacks {
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
    }

    namespace MagicBB {
        MagicTable<4096> rookMagicTable;
        MagicTable<1024> bishopMagicTable;

        Bitboard setRookPossibleAttacks(const size_t &t_square, const Bitboard &t_occupancy) {
            Bitboard possibleAttacks{0};

            int pieceRank = static_cast<int>(t_square / g_rankSize), pieceFile = static_cast<int>(t_square %
                                                                                                  g_fileSize);

            for (int rank = pieceRank - 1; rank >= RANK_1; --rank) {
                const size_t actualSq = rank * 8 + pieceFile;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1; rank <= RANK_8; ++rank) {
                const size_t actualSq = rank * 8 + pieceFile;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int file = pieceFile - 1; file >= FILE_A; --file) {
                const size_t actualSq = pieceRank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int file = pieceFile + 1; file <= FILE_H; ++file) {
                const size_t actualSq = pieceRank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            return possibleAttacks;
        }
        Bitboard setBishopPossibleAttacks(const size_t &t_square, const Bitboard &t_occupancy) {
            Bitboard possibleAttacks{0};

            int pieceRank = static_cast<int>(t_square / g_rankSize), pieceFile = static_cast<int>(t_square %
                                                                                                  g_fileSize);

            for (int rank = pieceRank - 1, file = pieceFile - 1; rank >= RANK_1 && file >= FILE_A; --rank, --file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank - 1, file = pieceFile + 1; rank >= RANK_1 && file <= FILE_H; --rank, ++file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1, file = pieceFile - 1; rank <= RANK_8 && file >= FILE_A; ++rank, --file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            for (int rank = pieceRank + 1, file = pieceFile + 1; rank <= RANK_8 && file <= FILE_H; ++rank, ++file) {
                const size_t actualSq = rank * 8 + file;
                possibleAttacks |= Bitboard{}.set(actualSq);
                if (t_occupancy.test(actualSq)) break;
            }

            return possibleAttacks;
        }
        Bitboard setOccupancy(const size_t &t_square, const size_t &t_count, const std::vector<size_t> &t_setBitsIndexes) {
            Bitboard occupancy{0}, variation{t_count};

            for (size_t i = 0; i < t_setBitsIndexes.size(); ++i) {
                occupancy[t_setBitsIndexes[i]] = variation[i];
            }

            return occupancy;
        }
        auto setSetBitsIndexes(const Bitboard &t_bb) {
            std::vector<size_t> setBitsIndexes;
            for (size_t i = 0; i < g_boardSize; ++i) {
                if (t_bb.test(i)) setBitsIndexes.push_back(i);
            }
            return setBitsIndexes;
        }

        void initRookMagicTable()
        {
            for (size_t sq = 0; sq < g_boardSize; ++sq) {
                std::vector<size_t> maskSetBitsIndexes = setSetBitsIndexes(rookMasks[sq]);

                for (size_t cnt = 0; cnt < (1ULL << rookBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * rookMagics[sq].to_ullong()) >> (g_boardSize - rookBitsCnt[sq]);
                    rookMagicTable[sq][magicKey] = setRookPossibleAttacks(sq, occupancy);;
                }
            }
        }
        void initBishopMagicTable()
        {
            for (size_t sq = 0; sq < g_boardSize; ++sq) {
                std::vector<size_t> maskSetBitsIndexes = setSetBitsIndexes(bishopMasks[sq]);

                for (size_t cnt = 0; cnt < (1ULL << bishopBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * bishopMagics[sq].to_ullong()) >> (g_boardSize - bishopBitsCnt[sq]);
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
                clearFile[i].set((k * 8) + (7 - i), false);
                maskFile[i].set((k * 8) + (7 - i), true);
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
                    (west[i] & clearFile[FILE_H]) |
                    (east[i] & clearFile[FILE_A]);
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
    void initSlidingPiecesTable()
    {
        MagicBB::initRookMagicTable();
        MagicBB::initBishopMagicTable();
    }

    void init() {
        initClearsAndMasks();
        initDirections();
        initRookMasks();
        initBishopMasks();
        initSlidingPiecesTable();
    }
}

class MoveGenerator {
public:
    static Bitboard generateKingMoves(const Bitboard &t_kingLoc, const Bitboard &t_ownPieces)
    {
        Bitboard kingClippedFileH = t_kingLoc & LookupTables::clearFile[FILE_H];
        Bitboard kingClippedFileA = t_kingLoc & LookupTables::clearFile[FILE_A];

        Bitboard square1 = kingClippedFileH >> 7;
        Bitboard square2 = kingClippedFileH << 9;
        Bitboard square3 = kingClippedFileH << 1;
        Bitboard square4 = kingClippedFileA << 7;
        Bitboard square5 = kingClippedFileA >> 9;
        Bitboard square6 = kingClippedFileA >> 1;
        Bitboard square7 = t_kingLoc << 8;
        Bitboard square8 = t_kingLoc >> 8;

        Bitboard kingMoves = square1 | square2 | square3 | square4 | square5 | square6 | square7 | square8;
        Bitboard validKing = kingMoves & ~t_ownPieces;

        return validKing;
    }
    static Bitboard generateKnightMoves(const size_t &t_loc, const Bitboard &t_ownPieces)
    {
        return LookupTables::Attacks::knightAttacks[t_loc] & ~t_ownPieces;
    }
    static Bitboard generateWhitePawnMoves(const Bitboard &t_pawnLoc, const Bitboard &t_blackPieces, const Bitboard &t_allPieces)
    {
        Bitboard oneSquareMove = (t_pawnLoc << 8) & ~t_allPieces;
        Bitboard twoSquaresMove = ((oneSquareMove & LookupTables::maskRank[RANK_3]) << 8) & ~t_allPieces;
        Bitboard validQuietMoves = oneSquareMove | twoSquaresMove;

        Bitboard leftAttackMove = (t_pawnLoc & LookupTables::clearFile[FILE_A]) << 7;
        Bitboard rightAttackMove = (t_pawnLoc & LookupTables::clearFile[FILE_H]) << 9;
        Bitboard allAttackMoves = leftAttackMove | rightAttackMove;
        Bitboard validAttackMoves = allAttackMoves & t_blackPieces;

        Bitboard pawnValid = validQuietMoves | validAttackMoves;

        return pawnValid;
    }
    static Bitboard generateBlackPawnMoves(const Bitboard &t_pawnLoc, const Bitboard &t_whitePieces, const Bitboard &t_allPieces)
    {
        Bitboard oneSquareMove = (t_pawnLoc >> 8) & ~t_allPieces;
        Bitboard twoSquaresMove = ((oneSquareMove & LookupTables::maskRank[RANK_6]) >> 8) & ~t_allPieces;
        Bitboard validQuietMoves = oneSquareMove | twoSquaresMove;

        Bitboard leftAttackMove = (t_pawnLoc & LookupTables::clearFile[FILE_A]) >> 7;
        Bitboard rightAttackMove = (t_pawnLoc & LookupTables::clearFile[FILE_H]) >> 9;
        Bitboard allAttackMoves = leftAttackMove | rightAttackMove;
        Bitboard validAttackMoves = allAttackMoves & t_whitePieces;

        Bitboard pawnValid = validQuietMoves | validAttackMoves;

        return pawnValid;
    }

};

int main()
{
    LookupTables::init();
    Board board;

    board.displayBoard();
    displayBitboard(LookupTables::MagicBB::rookMagicTable[0][2]);
//
//    displayBitboard(MoveGenerator::generateKingMoves(board.whiteKing, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateKnightMoves(board.whiteKnights, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateWhitePawnMoves(board.whitePawns, board.getAllBlackPieces(), board.getAllPieces()));
//    displayBitboard(MoveGenerator::generateBlackPawnMoves(board.blackPawns, board.getAllWhitePieces(), board.getAllPieces()));

    return 0;
}


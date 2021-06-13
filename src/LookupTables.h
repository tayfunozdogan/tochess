//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef TOCHESS_LOOKUPTABLES_H
#define TOCHESS_LOOKUPTABLES_H

#include <array>
#include <vector>
#include "common.h"

namespace MagicConstants {
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
}

namespace EvaluationConstants {

    const int pawnValue = 100;
    const int knightValue = 320;
    const int bishopValue = 330;
    const int rookValue = 500;
    const int queenValue = 900;
    const int kingValue = 20000;

    const int whitePawn[g_boardSize] = {
            0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10,-20,-20, 10, 10,  5,
            5, -5,-10,  0,  0,-10, -5,  5,
            0,  0,  0, 20, 20,  0,  0,  0,
            5,  5, 10, 25, 25, 10,  5,  5,
            10, 10, 20, 30, 30, 20, 10, 10,
            50, 50, 50, 50, 50, 50, 50, 50,
            0,  0,  0,  0,  0,  0,  0,  0,
    };

    const int blackPawn[g_boardSize] = {
            0,  0,  0,  0,  0,  0,  0,  0,
            50, 50, 50, 50, 50, 50, 50, 50,
            10, 10, 20, 30, 30, 20, 10, 10,
            5,  5, 10, 25, 25, 10,  5,  5,
            0,  0,  0, 20, 20,  0,  0,  0,
            5, -5,-10,  0,  0,-10, -5,  5,
            5, 10, 10,-20,-20, 10, 10,  5,
            0,  0,  0,  0,  0,  0,  0,  0
    };

    const int whiteKnight[g_boardSize] = {
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  5,  5,  0,-20,-40,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -30,  0, 15, 20, 20, 15,  0,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  0, 10, 15, 15, 10,  0,-30,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50,
    };

    const int blackKnight[g_boardSize] = {
            -50,-40,-30,-30,-30,-30,-40,-50,
            -40,-20,  0,  0,  0,  0,-20,-40,
            -30,  0, 10, 15, 15, 10,  0,-30,
            -30,  5, 15, 20, 20, 15,  5,-30,
            -30,  0, 15, 20, 20, 15,  0,-30,
            -30,  5, 10, 15, 15, 10,  5,-30,
            -40,-20,  0,  5,  5,  0,-20,-40,
            -50,-40,-30,-30,-30,-30,-40,-50,
    };

    const int whiteBishop[g_boardSize] = {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  5,  0,  0,  0,  0,  5,-10,
            -10, 10, 10, 10, 10, 10, 10,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10,  5,  5, 10, 10,  5,  5,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10,-10,-10,-10,-10,-20,
    };

    const int blackBishop[g_boardSize] = {
            -20,-10,-10,-10,-10,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5, 10, 10,  5,  0,-10,
            -10,  5,  5, 10, 10,  5,  5,-10,
            -10,  0, 10, 10, 10, 10,  0,-10,
            -10, 10, 10, 10, 10, 10, 10,-10,
            -10,  5,  0,  0,  0,  0,  5,-10,
            -20,-10,-10,-10,-10,-10,-10,-20,
    };

    const int whiteRook[g_boardSize] = {
            0,  0,  0,  5,  5,  0,  0,  0,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            5, 10, 10, 10, 10, 10, 10,  5,
            0,  0,  0,  0,  0,  0,  0,  0,
    };

    const int blackRook[g_boardSize] = {
            0,  0,  0,  0,  0,  0,  0,  0,
            5, 10, 10, 10, 10, 10, 10,  5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            -5,  0,  0,  0,  0,  0,  0, -5,
            0,  0,  0,  5,  5,  0,  0,  0
    };

    const int whiteQueen[g_boardSize] = {
            -20,-10,-10, -5, -5,-10,-10,-20
            -10,  0,  5,  0,  0,  0,  0,-10,
            -10,  5,  5,  5,  5,  5,  0,-10,
            0,  0,  5,  5,  5,  5,  0, -5,
            -5,  0,  5,  5,  5,  5,  0, -5,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20
    };

    const int blackQueen[g_boardSize] = {
            -20,-10,-10, -5, -5,-10,-10,-20,
            -10,  0,  0,  0,  0,  0,  0,-10,
            -10,  0,  5,  5,  5,  5,  0,-10,
            -5,  0,  5,  5,  5,  5,  0, -5,
            0,  0,  5,  5,  5,  5,  0, -5,
            -10,  5,  5,  5,  5,  5,  0,-10,
            -10,  0,  5,  0,  0,  0,  0,-10,
            -20,-10,-10, -5, -5,-10,-10,-20
    };

    const int whiteKing[g_boardSize] = {
            20, 30, 10,  0,  0, 10, 30, 20,
            20, 20,  0,  0,  0,  0, 20, 20,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30
    };

    const int blackKing[g_boardSize] = {
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -30,-40,-40,-50,-50,-40,-40,-30,
            -20,-30,-30,-40,-40,-30,-30,-20,
            -20,-30,-30,-40,-40,-30,-30,-20,
            20, 20,  0,  0,  0,  0, 20, 20,
            20, 30, 10,  0,  0, 10, 30, 20
    };

//    const int whiteKingEndGame[g_boardSize] = {
//            -50,-30,-30,-30,-30,-30,-30,-50,
//            -30,-30,  0,  0,  0,  0,-30,-30,
//            -30,-10, 20, 30, 30, 20,-10,-30,
//            -30,-10, 30, 40, 40, 30,-10,-30,
//            -30,-10, 30, 40, 40, 30,-10,-30,
//            -30,-10, 20, 30, 30, 20,-10,-30,
//            -30,-20,-10,  0,  0,-10,-20,-30,
//            -50,-40,-30,-20,-20,-30,-40,-50
//    };
//
//    const int blackKingEndGame[g_boardSize] = {
//            -50,-40,-30,-20,-20,-30,-40,-50,
//            -30,-20,-10,  0,  0,-10,-20,-30,
//            -30,-10, 20, 30, 30, 20,-10,-30,
//            -30,-10, 30, 40, 40, 30,-10,-30,
//            -30,-10, 30, 40, 40, 30,-10,-30,
//            -30,-10, 20, 30, 30, 20,-10,-30,
//            -30,-30,  0,  0,  0,  0,-30,-30,
//            -50,-30,-30,-30,-30,-30,-30,-50
//    };
}

namespace LookupTables {
    template <size_t I> using MagicTable = std::array<std::array<Bitboard, I>, g_boardSize>;

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
                Bitboard blackSouthWest = (loc & LookupTables::clearFile[FILE_H]) >> 7;
                Bitboard blackSouthEast = (loc & LookupTables::clearFile[FILE_A]) >> 9;
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

                for (size_t cnt = 0; cnt < (BitOps::ONE << MagicConstants::rookBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * MagicConstants::rookMagics[sq]) >> (g_boardSize - MagicConstants::rookBitsCnt[sq]);
                    rookMagicTable[sq][magicKey] = setRookPossibleAttacks(sq, occupancy);
                }
            }
        }
        void initBishopMagicTable()
        {
            for (size_t sq = 0; sq < g_boardSize; ++sq) {
                std::vector<size_t> maskSetBitsIndexes = setSetBitsIndexes(bishopMasks[sq]);

                for (size_t cnt = 0; cnt < (1ULL << MagicConstants::bishopBitsCnt[sq]); ++cnt) {
                    const auto occupancy = setOccupancy(sq, cnt, maskSetBitsIndexes);
                    auto magicKey = (occupancy.to_ullong() * MagicConstants::bishopMagics[sq]) >> (g_boardSize - MagicConstants::bishopBitsCnt[sq]);
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

#endif //TOCHESS_LOOKUPTABLES_H

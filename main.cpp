#include <iostream>
#include <strings.h>
#include <algorithm>
#include "MoveGenerator.h"
#include "LookupTables.h"
#include "common.h"
#include "Move.h"

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
    MoveGenerator mg;
//    board.changeActivePlayer();
    mg.generateLegalMoveSet(board, board.activePlayer);
    displayMoveSet(mg.legalMoveSet);
//
//    displayBitboard(MoveGenerator::generateKingMoves(board.whiteKing, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateKnightMoves(board.whiteKnights, board.allWhitePieces));
//    displayBitboard(MoveGenerator::generateWhitePawnMoves(board.whitePawns, board.getAllBlackPieces(), board.getAllPieces()));
//    displayBitboard(MoveGenerator::generateBlackPawnMoves(board.blackPawns, board.getAllWhitePieces(), board.getAllPieces()));

    return 0;
}


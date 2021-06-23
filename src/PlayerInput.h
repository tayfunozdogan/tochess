//
// Created by Tayfun Özdoğan on 22.06.2021.
//

#ifndef TOCHESS_PLAYERINPUT_H
#define TOCHESS_PLAYERINPUT_H

#include <iostream>
#include "MoveGenerator.h"

struct Input {
    static bool isValid(std::string t_input)
    {
        if (t_input.size() != 2 || std::tolower(t_input[0]) < 'a' || std::tolower(t_input[0]) > 'h' || t_input[1] < '1' || t_input[1] > '8')
            return false;

        return true;
    }
    static size_t algebraicToIndex(const std::string &t_input)
    {
        return ((t_input[1] - '1') * 8) + (std::tolower(t_input[0]) - 'a'); //a1_square - (move_row * 10) + move_col
    }
    static std::string indexToAlgebraic(const size_t index)
    {
        char column =(char)('A' + index % 8);
        char row = (char)('1' + index / 8);
        std::string algebraic;
        algebraic.push_back(column);
        algebraic.push_back(row);
        return algebraic;
    }
public:
    static size_t promptSquare()
    {
        std::string input;
        while(true) {
            std::cout << "Give me a square: ";
            std::cin >> input;
            if (isValid(input))
                break;
            std::cout << "\u001B[31mInvalid square!\u001B[0m\n";
            input.clear();
        }

        return algebraicToIndex(input);
    }
    static Move getMoveFromPlayer(const Board &board)
    {
        while (true) {
            size_t from = Input::promptSquare();
            std::cout << "from: " << indexToAlgebraic(from) << "\n";
            size_t to = Input::promptSquare();
            std::cout << "to: " << indexToAlgebraic(to) << "\n";
            MoveGenerator mg;
            mg.generateLegalMoveSet(board);
            for (const auto &move : mg.legalMoveSet) {
                bool isValidTo = false, isValidFrom = false;
                if (move.getFrom() == from)
                    isValidFrom = true;
                if (move.getTo() == to)
                    isValidTo = true;

                if (isValidFrom && isValidTo)
                    return move;
            }
            std::cout << "\u001B[31mInvalid Move!\033[0m\n";
        }
    }
};

#endif //TOCHESS_PLAYERINPUT_H

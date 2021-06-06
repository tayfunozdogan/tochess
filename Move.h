//
// Created by Tayfun Özdoğan on 4.06.2021.
//

#ifndef POORMANSENGINE_MOVE_H
#define POORMANSENGINE_MOVE_H


enum class MoveType {
    REGULAR, QUIET, CAPTURE, PROMOTION, CAPTURE_PROMOTION, DOUBLE_PAWN, EN_PASSANT, CASTLING
};

enum class PromotionType {
    QUEEN_PROM, ROOK_PROM, KNIGHT_PROM, BISHOP_PROM
};

enum class CastlingType {
    NONE, WHITE_KSIDE, WHITE_QSIDE, BLACK_KSIDE, BLACK_QSIDE
};

class Move {
public: // if the moveType of a move is quiet or capture, its other type variables are unimportant.
    Move(BIndex t_from, BIndex t_to, PieceType t_pieceType, MoveType t_moveType = MoveType::REGULAR)
            : from(t_from), to(t_to), pieceType(t_pieceType), moveType(t_moveType) {};

    Move(BIndex t_from, BIndex t_to, PieceType t_pieceType, MoveType t_moveType, PromotionType t_promotionType)
            : from(t_from), to(t_to),pieceType(t_pieceType), moveType(t_moveType), promotionType(t_promotionType) {};

    Move(BIndex t_from, BIndex t_to, PieceType t_pieceType, MoveType t_moveType, CastlingType t_castlingType)
            : from(t_from), to(t_to), pieceType(t_pieceType), moveType(t_moveType), castlingType(t_castlingType) {};

    BIndex getFrom() const { return from; }
    BIndex getTo() const { return to; }
    MoveType getMoveType() const { return moveType; }
    PromotionType getPromotionType() const { return promotionType; }
    PieceType getPieceType() const { return pieceType; }
    CastlingType getCastlingType() const { return castlingType; }
    void setFrom(BIndex t_from) { from = t_from; }
    void setTo(BIndex t_to) { to = t_to; }
    void setMoveType(MoveType t_moveType) { moveType = t_moveType; }
    void setPromotionType(PromotionType t_promotionType) { promotionType = t_promotionType; }
    void setPieceType(PieceType t_pieceType) { pieceType = t_pieceType; }
    void setCastlingType(CastlingType t_castlingType) { castlingType = t_castlingType; }
private:
    BIndex from, to;
    MoveType moveType;
    PromotionType promotionType;
    CastlingType castlingType;
    PieceType pieceType;
};


#endif //POORMANSENGINE_MOVE_H

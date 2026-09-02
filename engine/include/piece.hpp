#pragma once

#include "types.hpp"

class Piece 
{

private:
    PieceType type;
    Color color;
public:
    PieceType getType() {return this->type;}
    Color getColor() {return this->color;}
    void setType(PieceType type) {this->type = type;}
    void setColor(Color color) {this->color = color;}
};
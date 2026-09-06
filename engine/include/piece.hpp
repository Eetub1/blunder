#pragma once

#include "types.hpp"

class Piece 
{
private:
    PieceType type;
    Color color;
public:
    Piece() : type(PieceType::EMPTY), color(Color::NOCOLOR) {}
    Piece(PieceType type)
        : type(type)
    {
        Color color;
        if (WP <= type && type <= WK) {
            color = Color::WHITE;
        } else if (BP <= type && type <= BK) {
            color = Color::BLACK;
        } else {
            color = Color::NOCOLOR;
        }
        this->color = color;
    }

    PieceType getType() {return this->type;}
    Color getColor() {return this->color;}

    void setType(PieceType type) {this->type = type;}
    void setColor(Color color) {this->color = color;}
};
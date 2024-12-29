#pragma once

#include <iostream>
#include <cstdint>
#include <SFML/Graphics.hpp>
// macro stuff
using vec2i = sf::Vector2i;


// 0 = PAWN
// 1 = 


class Piece {
public:
    enum class Type {
        None = 0,
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    };
    enum class Color {
        None = 0,
        White,
        Black
    };

    Piece() : type(Type::None), color(Color::None) {};
    Piece(Type type, Color color) : type(type), color(color) {};

    

    // getters
    Type getType() const { return type; }
    Color getColor() const { return color; }

    // setters
    void setType(Type t) { type = t; }
    void setColor(Color c) { color = c; }

 private:
    Type type;
    Color color;

};
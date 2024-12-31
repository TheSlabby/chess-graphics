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
    vec2i getLocation() const { return location; }
    bool isValid() const { return type != Type::None; }
    bool hasMoved() const { return numberOfMoves > 0; }

    int numberOfMoves = {0};
    bool doublePushed = {false}; // for en passant
    int lastMove = {0}; // also for en passant

    // setters
    void setType(Type t) { type = t; }
    void setColor(Color c) { color = c; }
    void syncLocation(vec2i dest) { location = dest; };


    // get move direction, move offsets, etc.
    std::vector<vec2i> getMoveDirections();
    std::vector<vec2i> getMoveOffsets();

 private:
    Type type;
    Color color;
    vec2i location;

};
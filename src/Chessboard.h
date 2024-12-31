#pragma once

#include <iostream>
#include <array>
#include "Piece.h"
#include <cstdint>
#include <vector>
#include <cmath>


class Chessboard {
public:
    Chessboard();

    void initialize();
    Piece getPiece(int x, int y);
    void setPiece(int x, int y, Piece piece);
    void printBoard() const; // const means we dont actually change the elements in this class
    bool movePiece(Piece piece, vec2i destination, bool force = false);
    bool canMovePiece(Piece piece, vec2i destination);
    bool isSquareSafe(vec2i destination, Piece::Color defender);

    Piece getKing(Piece::Color color);

    std::vector<vec2i> getMoves(Piece piece);

    // render stuff
    void render(sf::RenderWindow& window);

    // io stuff
    vec2i mouseMoveSource{-1, -1};

private:
    std::array<std::array<Piece, 8>, 8> board;
    void renderPiece(Piece piece, sf::Vector2i pos, sf::RenderWindow& window);
    sf::Texture spriteSheet;
    int moveNumber = {0};

    // sync piece locations with this chessboard
    void syncPieceLocations();

};
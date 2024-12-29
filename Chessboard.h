#pragma once

#include <iostream>
#include <array>
#include "Piece.h"
#include <cstdint>
#include <vector>


class Chessboard {
public:
    Chessboard();

    void initialize();
    Piece getPiece(int x, int y);
    void setPiece(int x, int y, Piece piece);
    void printBoard() const; // const means we dont actually change the elements in this class
    void movePiece(Piece piece, vec2i destination);

    std::vector<vec2i> getMoves(Piece piece);

    // render stuff
    void render(sf::RenderWindow& window);

private:
    std::array<std::array<Piece, 8>, 8> board;
    void renderPiece(Piece piece, sf::Vector2i pos, sf::RenderWindow& window);
    sf::Texture spriteSheet;

    // sync piece locations with this chessboard
    void syncPieceLocations();

};
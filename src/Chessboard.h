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

    // copy constructor (for move filtering to keep king safe)
    Chessboard(const Chessboard& other) {
        this->board = other.board;
        this->moveNumber = other.moveNumber;
    }

    void initialize();
    Piece getPiece(int x, int y);
    void setPiece(int x, int y, Piece piece);
    void printBoard() const; // const means we dont actually change the elements in this class
    bool movePiece(Piece piece, vec2i destination, bool force = false);
    bool canMovePiece(Piece piece, vec2i destination, bool ensureKingSafe = true);
    bool isSquareSafe(vec2i destination, Piece::Color defender);
    bool isCheckmate(Piece::Color defender);
    int getMoveNumber() const { return moveNumber; }

    Piece getKing(Piece::Color color);

    std::vector<vec2i> getMoves(Piece piece, bool ensureKingSafe = false);

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
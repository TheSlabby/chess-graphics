#include "Piece.h"

std::vector<vec2i> Piece::getMoveDirections() {
    std::vector<vec2i> dirs;

    switch (type) {
        case Type::Rook:
            dirs.push_back(vec2i(1,0));
            dirs.push_back(vec2i(-1,0));
            dirs.push_back(vec2i(0,-1));
            dirs.push_back(vec2i(0,1));
            break;

        case Type::Bishop:
            dirs.push_back(vec2i(1,1));
            dirs.push_back(vec2i(-1,1));
            dirs.push_back(vec2i(-1,-1));
            dirs.push_back(vec2i(1,-1));
            break;
        
        case Type::Queen:
            dirs.push_back(vec2i(1,1));
            dirs.push_back(vec2i(-1,1));
            dirs.push_back(vec2i(-1,-1));
            dirs.push_back(vec2i(1,-1));
            dirs.push_back(vec2i(1,0));
            dirs.push_back(vec2i(-1,0));
            dirs.push_back(vec2i(0,-1));
            dirs.push_back(vec2i(0,1));
            break;
    }

    return dirs;
}

std::vector<vec2i> Piece::getMoveOffsets() {
    std::vector<vec2i> offsets;

    switch (type) {
        case Type::Knight:
            offsets.push_back(vec2i(1,2));
            offsets.push_back(vec2i(2,1));
            offsets.push_back(vec2i(-1,2));
            offsets.push_back(vec2i(-2,1));
            offsets.push_back(vec2i(1,-2));
            offsets.push_back(vec2i(2,-1));
            offsets.push_back(vec2i(-1,-2));
            offsets.push_back(vec2i(-2,-1));
            break;
        
        case Type::King:
            offsets.push_back(vec2i(1, 0));   // Right
            offsets.push_back(vec2i(-1, 0));  // Left
            offsets.push_back(vec2i(0, 1));   // Up
            offsets.push_back(vec2i(0, -1));  // Down
            offsets.push_back(vec2i(1, 1));   // Up-Right
            offsets.push_back(vec2i(-1, 1));  // Up-Left
            offsets.push_back(vec2i(1, -1));  // Down-Right
            offsets.push_back(vec2i(-1, -1)); // Down-Left
            break;
    }

    return offsets;
}
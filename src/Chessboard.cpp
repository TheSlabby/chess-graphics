#include "Chessboard.h"

// constants
constexpr int windowSize = 1200;
constexpr int squareSize = windowSize / 8;
constexpr int pieceImageSize = 480;
constexpr float pieceScale = static_cast<float>(squareSize) / pieceImageSize;
constexpr float HOVER_DARKEN_STRENGTH = .8;


bool inBounds(int pos) {
    return (pos >= 0 && pos <= 7);
}

Chessboard::Chessboard() {
    this->initialize();

    // load sprite sheet
    if (!spriteSheet.loadFromFile("assets/pieces_spritesheet.png")) {
        std::cerr << "Couldn't load spritesheet.\n";
    }

    std::cout << "Chessboard created\n";
}

void Chessboard::initialize() {
    // Set up Black pieces (top rows)
    this->setPiece(0, 0, Piece(Piece::Type::Rook, Piece::Color::Black));
    this->setPiece(1, 0, Piece(Piece::Type::Knight, Piece::Color::Black));
    this->setPiece(2, 0, Piece(Piece::Type::Bishop, Piece::Color::Black));
    this->setPiece(3, 0, Piece(Piece::Type::Queen, Piece::Color::Black));
    this->setPiece(4, 0, Piece(Piece::Type::King, Piece::Color::Black));
    this->setPiece(5, 0, Piece(Piece::Type::Bishop, Piece::Color::Black));
    this->setPiece(6, 0, Piece(Piece::Type::Knight, Piece::Color::Black));
    this->setPiece(7, 0, Piece(Piece::Type::Rook, Piece::Color::Black));

    for (int x = 0; x < 8; ++x) {
        this->setPiece(x, 1, Piece(Piece::Type::Pawn, Piece::Color::Black));
    }

    // Set up White pieces (bottom rows)
    this->setPiece(0, 7, Piece(Piece::Type::Rook, Piece::Color::White));
    this->setPiece(1, 7, Piece(Piece::Type::Knight, Piece::Color::White));
    this->setPiece(2, 7, Piece(Piece::Type::Bishop, Piece::Color::White));
    this->setPiece(3, 7, Piece(Piece::Type::Queen, Piece::Color::White));
    this->setPiece(4, 7, Piece(Piece::Type::King, Piece::Color::White));
    this->setPiece(5, 7, Piece(Piece::Type::Bishop, Piece::Color::White));
    this->setPiece(6, 7, Piece(Piece::Type::Knight, Piece::Color::White));
    this->setPiece(7, 7, Piece(Piece::Type::Rook, Piece::Color::White));

    for (int x = 0; x < 8; ++x) {
        this->setPiece(x, 6, Piece(Piece::Type::Pawn, Piece::Color::White));
    }

    this->syncPieceLocations();
}

Piece Chessboard::getPiece(int x, int y) {
    if (inBounds(x) && inBounds(y)) {
        return board[x][y];
    } else {
        return Piece();
    }
}

void Chessboard::setPiece(int x, int y, Piece piece) { board[x][y] = piece; }

void Chessboard::printBoard() const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << static_cast<int>(board[j][i].getType()) << "\t";
        }
        std::cout << "\n";
    }
}

void Chessboard::render(sf::RenderWindow &window) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            vec2i pos(i, j);
            Piece p = board[i][j];
            if (p.getType() != Piece::Type::None)
                this->renderPiece(board[i][j], pos, window);
        }
    }
}

void Chessboard::renderPiece(Piece piece, vec2i pos, sf::RenderWindow &window) {
    sf::Sprite sprite(spriteSheet);

    // now set offset
    int x = 0;  // Default offset for King
    switch (piece.getType()) {
        case Piece::Type::King:
            x = 0 * pieceImageSize;
            break;
        case Piece::Type::Queen:
            x = 1 * pieceImageSize;
            break;
        case Piece::Type::Bishop:
            x = 2 * pieceImageSize;
            break;
        case Piece::Type::Knight:
            x = 3 * pieceImageSize;
            break;
        case Piece::Type::Rook:
            x = 4 * pieceImageSize;
            break;
        case Piece::Type::Pawn:
            x = 5 * pieceImageSize;
            break;
        default:
            std::cerr << "Unknown piece type!\n";
            break;
    }
    int y = (piece.getColor() == Piece::Color::Black ? pieceImageSize : 0);

    sprite.setTextureRect(sf::IntRect(x, y, pieceImageSize, pieceImageSize));

    // now position & scale it
    sprite.setPosition(squareSize * pos.x, squareSize * pos.y);
    sprite.setScale(pieceScale, pieceScale);

    window.draw(sprite);
}



// make it so the piece themselves know where they are
void Chessboard::syncPieceLocations() {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece& piece = board[i][j];
            piece.syncLocation(vec2i(i, j));
        }
    }
}



std::vector<vec2i> Chessboard::getMoves(Piece piece, bool ensureKingSafe) {
    std::vector<vec2i> moves;

    vec2i start = piece.getLocation();

    // first lets do move directions
    std::vector<vec2i> moveDirections = piece.getMoveDirections();
    for (auto dir : moveDirections) {
        vec2i currentPos = start;
        while (true) {
            currentPos += dir;

            // check we're in bounds
            if (currentPos.x < 0 || currentPos.x > 7 || currentPos.y < 0 || currentPos.y > 7)
                break; // we ran into another piece.
            
            // what happens if we bounce into a piece
            Piece goalPiece = board[currentPos.x][currentPos.y];
            if (goalPiece.isValid() && goalPiece.getColor() == piece.getColor())
                break;
            if (goalPiece.isValid()) {
                // we bounced into another teams piece
                moves.push_back(currentPos);
                // then break as we cant go any further
                break;
            }
            

            moves.push_back(currentPos);
        }
    }

    // now do offsets
    std::vector<vec2i> moveOffsets = piece.getMoveOffsets();
    for (auto offset : moveOffsets) {
        vec2i newPos = start + offset;
        if (inBounds(newPos.x) && inBounds(newPos.y)) {
            Piece goalPiece = board[newPos.x][newPos.y];
            if (!goalPiece.isValid() || (goalPiece.getColor() != piece.getColor())) {
                moves.push_back(newPos);
            }
        }
        
    }

    // pawns are special (en passant, take diagonal, double push, etc)
    if (piece.getType() == Piece::Type::Pawn) {
        int dir = piece.getColor() == Piece::Color::White ? -1 : 1; // -1 if white, 1 if black
        vec2i pos = piece.getLocation();
        vec2i pushPos = pos + vec2i(0, dir);
        vec2i doublePushPos = pushPos + vec2i(0, dir);

        // see if pawn can push forward
        if (inBounds(pushPos.y) && !board[pushPos.x][pushPos.y].isValid()) { // check in bounds, then see if theres a pawn in the way
            moves.push_back(pushPos);

            // now check if double push is possible (no piece in way, AND 2nd or 6th rank)
            if (inBounds(doublePushPos.y) && !board[doublePushPos.x][doublePushPos.y].isValid() && (pos.y == 1 || pos.y == 6)) { 
                moves.push_back(doublePushPos);
            }
        }

        // see if pawn can take on diagonal
        vec2i diagonalLeft(pos + vec2i(-1, dir));
        vec2i diagonalRight(pos + vec2i(1, dir));
        if (inBounds(diagonalLeft.x)) {
            Piece targetPiece = board[diagonalLeft.x][diagonalLeft.y];
            Piece targetPieceEnPassant = board[diagonalLeft.x][pos.y];
            if (targetPiece.isValid() && targetPiece.getColor() != piece.getColor())
                moves.push_back(diagonalLeft);
            else if (targetPieceEnPassant.getType() == Piece::Type::Pawn && targetPieceEnPassant.doublePushed && targetPieceEnPassant.lastMove == moveNumber-1) // en passant
                moves.push_back(diagonalLeft);
        }
        if (inBounds(diagonalRight.x)) {
            Piece targetPiece = board[diagonalRight.x][diagonalRight.y];
            Piece targetPieceEnPassant = board[diagonalRight.x][pos.y];
            if (targetPiece.isValid() && targetPiece.getColor() != piece.getColor())
                moves.push_back(diagonalRight);
            else if (targetPieceEnPassant.getType() == Piece::Type::Pawn && targetPieceEnPassant.doublePushed && targetPieceEnPassant.lastMove == moveNumber-1) // en passant
                moves.push_back(diagonalRight);
        }
    }

    // castling - CONDITIONS: 1. pieces havent moved 2. all pieces are unoccupied betwen 3. spaces are safe
    // std::cout << "has moved: " << piece.hasMoved() << std::endl;
    if (piece.getType() == Piece::Type::King && piece.hasMoved() == false) {
        int y = piece.getLocation().y;
        Piece rightRook = getPiece(7, y);
        Piece leftRook = getPiece(0, y);

        // right castle
        if (rightRook.getType() == Piece::Type::Rook && !rightRook.hasMoved()) {
            // now verify all squares are safe & unoccupied
            if (!getPiece(6, y).isValid() && !getPiece(5, y).isValid()) {
                // finally make sure these spaces are safe
                if (isSquareSafe(vec2i(4, y), piece.getColor()) && isSquareSafe(vec2i(5, y), piece.getColor()) && isSquareSafe(vec2i(6, y), piece.getColor())) {
                    moves.push_back(vec2i(start.x + 2, start.y));
                }
                
            }
        }
        // left castle
        if (leftRook.getType() == Piece::Type::Rook && !leftRook.hasMoved()) {
            // now verify all squares are safe & unoccupied
            if (!getPiece(1, y).isValid() && !getPiece(2, y).isValid() && !getPiece(3, y).isValid()) {
                // finally make sure these spaces are safe
                if (isSquareSafe(vec2i(2, y), piece.getColor()) && isSquareSafe(vec2i(3, y), piece.getColor()) && isSquareSafe(vec2i(4, y), piece.getColor())) {
                    moves.push_back(vec2i(start.x - 2, start.y));
                }
                
            }
        }
    }

    std::vector<vec2i> filteredMoves;

    if (ensureKingSafe) {
        vec2i originalPos = piece.getLocation();
        auto boardCopy = board;
        piece = getPiece(originalPos.x, originalPos.y);

        for (const auto& move : moves) {
            // piece = getPiece(start.x, start.y);
            movePiece(piece, move, true);
            Piece king = getKing(piece.getColor());
            if (isSquareSafe(king.getLocation(), piece.getColor())) {
                // std::cout << "found a legal move lol\n";
                filteredMoves.push_back(move);
            }
            board = boardCopy;
            --moveNumber; // movePiece increments by 1, but we want to reverse this
        }
    } else {
        filteredMoves = moves;
    }


    return filteredMoves;
}

bool Chessboard::canMovePiece(Piece piece, vec2i dest, bool ensureKingSafe) {
    if (!piece.isValid())
        return false;
    // first things first we gotta make sure this move is legal :D
    auto moves = getMoves(piece, ensureKingSafe);

    // search moves for this move
    auto it = std::find(moves.begin(), moves.end(), dest);
    if (it == moves.end()) {
        // std::cout << "User attempted illegal move :(\n";
        return false;
    }
    return true;
}

bool Chessboard::movePiece(Piece piece, vec2i dest, bool force) {
    // first things first we gotta make sure this move is legal :D
    if (!force) {
        if (!canMovePiece(piece, dest)) return false;
    }
    
    vec2i sourcePos = piece.getLocation();
    piece.numberOfMoves++;

    // check if this is a double push (for en passant)
    piece.doublePushed = std::abs(sourcePos.y - dest.y) == 2;
    piece.lastMove = moveNumber;

    // check if this is a castling move
    if (piece.getType() == Piece::Type::King) {
        if (dest.x - piece.getLocation().x == 2) {
            // right castle detected
            setPiece(sourcePos.x + 1, sourcePos.y, getPiece(7, sourcePos.y));
            setPiece(7, sourcePos.y,  Piece());
        } else if (dest.x - piece.getLocation().x == -2) {
            // left castle detected
            setPiece(sourcePos.x - 1, sourcePos.y, getPiece(0, sourcePos.y));
            setPiece(0, sourcePos.y,  Piece());
        }
    }

    // check if this is an en passant
    int dir = piece.getColor() == Piece::Color::White ? -1 : 1; // -1 if white, 1 if black
    if (piece.getType() == Piece::Type::Pawn && dest.x != sourcePos.x && !getPiece(dest.x, dest.y).isValid()) {
        // std::cout << "En Passant!" << std::endl;
        setPiece(dest.x, sourcePos.y, Piece()); // kill the pawn
    }

    // pawn promotion
    if (piece.getType() == Piece::Type::Pawn && (dest.y == 0 || dest.y == 7)) {
        piece.setType(Piece::Type::Queen);
    }

    setPiece(sourcePos.x, sourcePos.y, Piece()); // set source to empty peace, as there is no longer a piece here
    setPiece(dest.x, dest.y, piece);

    // now sync positions for all pieces
    syncPieceLocations();

    ++moveNumber;

    return true;
}

Piece Chessboard::getKing(Piece::Color color) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece p = this->getPiece(i, j);
            if (p.getColor() == color && p.getType() == Piece::Type::King) {
                return p;
            }
        }
    }

    return Piece();
}

// check if square is safe. useful for castling & detecting check/checkmate
bool Chessboard::isSquareSafe(vec2i destination, Piece::Color defender) {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece piece = this->getPiece(i, j);
            if (piece.isValid() && piece.getColor() != defender) {
                auto moves = this->getMoves(piece);
                for (const auto& move : moves) {
                    if (move == destination) {
                        // an attacker CAN move to this square, so its NOT safe.
                        // std::cout << "UNSAFE SQUARE: " << destination.x << ", " << destination.y << std::endl;
                        return false;
                    }
                }
            }
        }
    }
    return true;
}


bool Chessboard::isCheckmate(Piece::Color defender) {
    int moveCount = 0;
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            Piece p = getPiece(i, j);
            if (p.isValid() && p.getColor() == defender) {
                auto moves = getMoves(p, true);
                moveCount += moves.size();
            }
        }
    }

    return moveCount == 0;
}
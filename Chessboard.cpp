#include "Chessboard.h"

// constants
constexpr int windowSize = 1200;
constexpr int squareSize = windowSize / 8;
constexpr int pieceImageSize = 480;
constexpr float pieceScale = static_cast<float>(squareSize) / pieceImageSize;
constexpr float HOVER_DARKEN_STRENGTH = .8;

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

Piece Chessboard::getPiece(int x, int y) { return board[x][y]; }

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



void Chessboard::movePiece(Piece piece, vec2i destination) {

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



std::vector<vec2i> Chessboard::getMoves(Piece piece) {
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
        Piece goalPiece = board[newPos.x][newPos.y];
        if (!goalPiece.isValid() || (goalPiece.getColor() != piece.getColor())) {
            moves.push_back(newPos);
        }
    }

    return moves;
}
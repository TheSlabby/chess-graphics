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
}

Piece Chessboard::getPiece(int x, int y) { return board[x][y]; }

void Chessboard::setPiece(int x, int y, Piece piece) { board[x][y] = piece; }

void Chessboard::printBoard() const {
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            std::cout << static_cast<int>(board[i][j].getType()) << "\t";
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
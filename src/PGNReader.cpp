#include "PGNReader.h"

PGNReader::PGNReader(string fileName, Chessboard& board) : board(board) {
    ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file\n";
        return;
    }

    string line;
    while (std::getline(file, line)) { // Read line by line
        parseLine(line);
    }

    // now parse the move buffer
    istringstream ss(moveBuffer);
    string token;
    int moveNum = -1;
    int moveColor = 0; // this will alternate between 0 and 1

    while (ss >> token) {
        // check if game is ended
        if (token == "1-0" || token == "0-1" || token == "1-1" || token == "0=0") {
            break;
        }

        // parse token
        cout << "Token: " << token << endl;
        if (isdigit(token[0])) {
            moveNum = token[0] - '0';
        } else {
            Piece::Color color = moveColor == 0 ? Piece::Color::White : Piece::Color::Black;
            Move move = {moveNum, color, token};
            moveColor ^= 1;
            moves.push_back(move);
        }
    }

    // now moves vector has all the moves in this chess game
}

void PGNReader::parseLine(string line) {
    if (line[0] == '[') {
        // parse metadata
        size_t keyStart = 1; // start from char 1 cuz we ignore [
        size_t keyEnd = line.find(' ', keyStart);
        size_t valueStart = line.find('"', keyEnd) + 1;
        size_t valueEnd = line.find('"', valueStart);

        string key = line.substr(keyStart, keyEnd - keyStart);
        string value = line.substr(valueStart, valueEnd - valueStart);

        // cout << key << ", " << value << endl;

        if (key == "Event") {
            Event = value;
        } else if (key == "Site") {
            Site = value;
        } else if (key == "Date") {
            Date = value;
        } else if (key == "Round") {
            Round = value;
        } else if (key == "White") {
            WhitePlayer = value;
        } else if (key == "Black") {
            BlackPlayer = value;
        } else if (key == "Result") {
            Result = value;
        }
    } else {
        // actual match data
        moveBuffer += ' ' + line;
    }
}


void PGNReader::print() {
    cout << "Metadata:\n";
    cout << "Event: " << Event << endl;
    cout << "Site: " << Site << endl;
    cout << "Date: " << Date << endl;
    cout << "Round: " << Round << endl;
    cout << "White Player: " << WhitePlayer << endl;
    cout << "Black Player: " << BlackPlayer << endl;
    cout << "Result: " << Result << endl;

    cout << "Move Buffer: " << moveBuffer << endl;
}


bool PGNReader::playNext() {
    if (currentMove >= moves.size())
        return false;

    // cout << "Total moves: " << moves.size() << endl;
    Move move = moves[currentMove];

    cout << "Playing move: " << move.moveString << " - " << currentMove << endl;
    playMove(move);

    ++currentMove;
    return true;
}

void PGNReader::playMove(Move move) {
    string m = move.moveString;

    int destX = -1;
    int destY = -1;
    int sourceX = -1;
    Piece::Type type;

    char char0 = m[0];
    char char1 = m[1];

    // PAWN HANDLING
    // if (char0 >= 'a' && char0 <= 'h') { // Validate input
    //     // we know its a pawn, because first letter is destination x
    //     type = Piece::Type::Pawn;

    //     destX = char0 - 'a';
    //     if (char1>= '1' && char1 <= '8') {
    //         destY = '8' - char1;
    //     } else if (char1 == 'x') {
    //         // PAWN is taking something
    //         sourceX = char0;

    //         destX = m[2] - 'a';
    //         destY = '8' - m[3];
    //     }
    // }


    if (char0 == 'N' || char0 == 'K' || char0 == 'R' || char0 == 'Q' || char0 == 'B' || (char0 >= 'a' && char0 <= 'h')) {
        if (char0 == 'N') type = Piece::Type::Knight;
        else if (char0 == 'R') type = Piece::Type::Rook;
        else if (char0 == 'B') type = Piece::Type::Bishop;
        else if (char0 == 'K') type = Piece::Type::King;
        else if (char0 == 'Q') type = Piece::Type::Queen;
        else {
            m = 'P' + m; // filler, so its same format as other pieces
            type = Piece::Type::Pawn;
        }

        // piece taking piece is not relevant, unless its a pawn.
        if (type != Piece::Type::Pawn) {
            m.erase(remove(m.begin(), m.end(), 'x'), m.end());
        }

        destX = m[1] - 'a';
        if (m[2] >= '1' && m[2] <= '8') {
            destY = '8' - m[2];
        } else if (m[2] == 'x') {
            // PAWN is taking something
            sourceX = m[1] - 'a';

            destX = m[3] - 'a';
            destY = '8' - m[4];
        } else if (m[2] >= 'a' && m[2] <= 'h') {
            // piece needs more context
            cout << "piece requires more context (source file)" << endl;
            sourceX = m[1] - 'a';

            destX = m[2] - 'a';
            destY = '8' - m[3];

            cout << "source X:" << sourceX << " dest X: " << destX << "dest Y: " << destY << endl;
        }

    }


    // now check if we know destination & piece type
    if (destX >= 0 && destY >= 0 && type != Piece::Type::None) {
        vec2i dest(destX, destY);
        // now we have to iterate all pieces that can move here, and fit the right type
        for (int i = 0; i < 8; ++i) {
            for (int j = 0; j < 8; ++j) {
                Piece piece = board.getPiece(i, j);
                if (piece.getType() == type && board.canMovePiece(piece, dest) && piece.getColor() == move.color) {
                    // finally, verify we're on proper source X (for pawn taking)
                    if (sourceX == -1 || sourceX == i) {
                        // this piece CAN move here AND is the right type, so return this
                        board.movePiece(piece, dest);
                        return;
                    }
                }
            }
        }
    } else if (m == "O-O-O") {
        // castle to the left
        int y = (move.color == Piece::Color::White) ? 7 : 0;
        Piece king = board.getPiece(4, y);
        Piece rook = board.getPiece(0, y);
        
        board.movePiece(king, vec2i(2, y), true); // force move these pieces, as this move is technically illegal
        board.movePiece(rook, vec2i(3, y), true);

    } else if (m == "O-O") {
        // castle to the right
        int y = (move.color == Piece::Color::White) ? 0 : 7;
        Piece king = board.getPiece(4, y);
        Piece rook = board.getPiece(7, y);

        board.movePiece(king, vec2i(6, y), true); // force move these pieces, as this move is technically illegal
        board.movePiece(rook, vec2i(5, y), true);
    } else {
        cout << "CAN'T PLAY THIS MOVE!" << endl;
    }
}
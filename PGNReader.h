#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include "Chessboard.h"

using namespace std;

struct Move {
    int moveNumber;
    Piece::Color color;
    string moveString;
};


class PGNReader {
public:
    PGNReader(string fileName, Chessboard& board);
    void print();
    bool playNext(); // play next move (requires board param)

private:

    // metadata
    string Event;
    string Site;
    string Date;
    string Round;
    string WhitePlayer;
    string BlackPlayer;
    string Result;

    int currentMove {0};
    vector<Move> moves;
    Chessboard& board;

    // parsing
    void parseLine(string line);
    string moveBuffer;
    void playMove(Move move);


};
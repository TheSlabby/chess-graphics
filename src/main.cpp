#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Chessboard.h"
#include "PGNReader.h"

// macro stuff
using vec2i = sf::Vector2i;

// constants
constexpr int windowSize = 1200;
constexpr int squareSize = windowSize / 8;
constexpr int pieceImageSize = 480;
constexpr float pieceScale = static_cast<float>(squareSize) / pieceImageSize;
constexpr float HOVER_DARKEN_STRENGTH = .8;

sf::Vector2i getHover(sf::RenderWindow& window) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);

	return mousePos / squareSize;
}

void drawChessboard(sf::RenderWindow& window, Chessboard& board) {
	const int squareSize = windowSize / 8;

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
			square.setPosition(i * squareSize, j * squareSize);

			sf::Color color;
			if ((i + j) % 2 == 0)
				color = sf::Color(250, 240, 255);
			else
				color = sf::Color(128, 64, 255);

			// now darken
			vec2i hoverSquare = getHover(window);
			if (i == hoverSquare.x && j == hoverSquare.y) {
				color.r = static_cast<uint8_t>(color.r * HOVER_DARKEN_STRENGTH);
				color.g = static_cast<uint8_t>(color.g * HOVER_DARKEN_STRENGTH);
				color.b = static_cast<uint8_t>(color.b * HOVER_DARKEN_STRENGTH);
			}

			square.setFillColor(color);

			window.draw(square);
		}
	}

	// now draw square if user is moving a piece
	if (board.mouseMoveSource.x != -1) {
		sf::RectangleShape square(sf::Vector2f(squareSize, squareSize));
		square.setPosition(board.mouseMoveSource.x * squareSize, board.mouseMoveSource.y * squareSize);
		square.setFillColor(sf::Color(219, 161, 0));

		window.draw(square);

	}
}

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(windowSize, windowSize), "Chess Graphics", sf::Style::Default, settings);
	window.setFramerateLimit(300); // limit fps

	sf::Font font;
	if (!font.loadFromFile("assets/JosefinSans-Regular.ttf")) {
		std::cerr << "Error loading font\n";
		return -1;
	}
	sf::Text text;
	text.setFont(font);
	text.setString("Chess Graphics");
	text.setCharacterSize(60);
	text.setFillColor(sf::Color(0, 0, 0));

	// audio init
	sf::SoundBuffer musicSoundBuffer;
	sf::Sound musicSound;
	sf::SoundBuffer movePieceSoundBuffer;
	sf::Sound movePieceSound;
	sf::SoundBuffer checkSoundBuffer;
	sf::Sound checkSound;
	sf::SoundBuffer captureSoundBuffer;
	sf::Sound captureSound;
	sf::SoundBuffer castleSoundBuffer;
	sf::Sound castleSound;
    if (!musicSoundBuffer.loadFromFile("assets/2009-03-30-clairdelune.ogg")) { // Replace with your file name
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
	if (!movePieceSoundBuffer.loadFromFile("assets/move-self.ogg")) { // Replace with your file name
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
	if (!checkSoundBuffer.loadFromFile("assets/move-check.ogg")) { // Replace with your file name
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
	if (!captureSoundBuffer.loadFromFile("assets/move-capture.ogg")) { // Replace with your file name
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
	if (!castleSoundBuffer.loadFromFile("assets/move-castle.ogg")) { // Replace with your file name
        std::cerr << "Error loading sound file!" << std::endl;
        return -1;
    }
	musicSound.setBuffer(musicSoundBuffer);
	movePieceSound.setBuffer(movePieceSoundBuffer);
	checkSound.setBuffer(checkSoundBuffer);
	captureSound.setBuffer(captureSoundBuffer);
	castleSound.setBuffer(castleSoundBuffer);

	musicSound.setLoop(true);
	musicSound.play();



	// instantiate clocks
	sf::Clock fpsClock;
	sf::Clock debugClock;



	// load chessboard
	Chessboard board;


	// load sprite sheet
	sf::Texture spriteSheet;
	if (!spriteSheet.loadFromFile("assets/pieces_spritesheet.png")) {
		std::cerr <<  "Couldn't load spritesheet.\n";
		return -1;
	}



	// LOAD GAME PGN
	PGNReader pgnReader("assets/kasparov_pgn.txt", board);
	pgnReader.print();


	while (window.isOpen()) {
		sf::Vector2i mousePos = getHover(window);


		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					std::cout << "Escape pressed, closing..\n";
					window.close();
				} else if (event.key.code == sf::Keyboard::Right) {
					if (pgnReader.playNext()) {
						movePieceSound.play();
					}
				}
			}


			// mouse down
			if (event.type == sf::Event::MouseButtonPressed) {
				board.mouseMoveSource = mousePos;
			}
			if (event.type == sf::Event::MouseButtonReleased) {
				if (board.mouseMoveSource.x != -1) {
					// perform peice movement
					vec2i source = board.mouseMoveSource;
					vec2i dest = mousePos;
					Piece sourcePiece = board.getPiece(source.x, source.y);

					bool willCapture = board.getPiece(dest.x, dest.y).isValid();

					if (board.movePiece(sourcePiece, dest)) {
						std::cout << "Successfully moved piece\n";

						// play move sound
						if (sourcePiece.getType() == Piece::Type::King && std::abs(dest.x - sourcePiece.getLocation().x) == 2) {
							castleSound.play();
						} else if (willCapture)
							captureSound.play();
						else
							movePieceSound.play();

						// now see if king is attacked
						Piece::Color attacker = sourcePiece.getColor() == Piece::Color::White ? Piece::Color::Black : Piece::Color::White;
						Piece king = board.getKing(attacker);
						std::cout << "King Location: " << king.getLocation().x << ", " << king.getLocation().y << std::endl;
						if (!board.isSquareSafe(king.getLocation(), attacker)) {
							std::cout << "Check!" << std::endl;
							checkSound.play();
						}
							
					} else {
						std::cout << "FAILED TO MOVE PIECE!\n";
					}
				}

				board.mouseMoveSource = vec2i(-1, -1);
			}
		}

		window.clear();


		// get fps
		float fps = 1.0f / fpsClock.restart().asSeconds();

		// debug output
		if (debugClock.getElapsedTime().asSeconds() > 1.0f) {
			// std::cout << "FPS: " << fps << "\tMouse X: " << mousePos.x << "\tMouse Y: " << mousePos.y << std::endl;
			// board.printBoard();
			debugClock.restart();
		}


		// checkerboard tiles
		drawChessboard(window, board);

		// now render the pieces on the board
		board.render(window);


		// now render possible moves based ony mouse hover
		Piece hoverPiece = board.getPiece(mousePos.x, mousePos.y);
		if (hoverPiece.isValid()) {
			std::vector<vec2i> legalMoves = board.getMoves(hoverPiece);
			for (auto move : legalMoves) {
				int sizeOffset = squareSize / 4;
				sf::CircleShape circle(sizeOffset);
				circle.setFillColor(sf::Color(128, 128, 128, 192));
				circle.setPosition(mousePos.x + squareSize*move.x + sizeOffset, mousePos.y + squareSize*move.y + sizeOffset);
				window.draw(circle);
			}
		}

		// window.draw(text);
		window.display();
	}

	std::cout << "\nBye\n";

	return 0;
}

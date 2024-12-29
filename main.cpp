#include <SFML/Graphics.hpp>
#include <iostream>
#include "Chessboard.h"

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

void drawChessboard(sf::RenderWindow& window) {
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



	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
			
			if (event.type == sf::Event::KeyPressed) {
				if (event.key.code == sf::Keyboard::Escape) {
					std::cout << "Escape pressed, closing..\n";
					window.close();
				}
			}
		}

		window.clear();

		sf::Vector2i mousePos = getHover(window);

		// get fps
		float fps = 1.0f / fpsClock.restart().asSeconds();

		// debug output
		if (debugClock.getElapsedTime().asSeconds() > 1.0f) {
			std::cout << "FPS: " << fps << "\tMouse X: " << mousePos.x << "\tMouse Y: " << mousePos.y << std::endl;
			board.printBoard();
			debugClock.restart();
		}


		// checkerboard tiles
		drawChessboard(window);

		// now render the pieces on the board
		board.render(window);

		// window.draw(text);
		window.display();
	}

	std::cout << "\nBye\n";

	return 0;
}
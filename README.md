# Chess Graphics

Chessboard and pieces, using [SFML](https://www.sfml-dev.org/) library.
\
In the future, I plan to add Stockfish for game analysis and custom Lua-based extensions.



## Build
Dev dependencies:
`sudo apt install libsfml-dev`


Build with Clang:\
`clang++ -g -o clang++ -o build/chess-graphics src/main.cpp src/Chessboard.cpp src/Piece.cpp src/PGNReader.cpp -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio && ./build/chess-graphics`
## Screenshots
En Passant
![image](https://github.com/user-attachments/assets/10cd2ba7-c974-4e54-add9-2f4f77ba26e1)
Legal moves must keep King safe
![image](https://github.com/user-attachments/assets/03cdb586-1c7a-4256-98be-0bca90e82721)
PGN Game Replay
![image](https://github.com/user-attachments/assets/823f1d0c-b1fb-412f-a636-965040587e12)





## Author

- [@theslabby](https://www.github.com/theslabby)


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
![Screenshot from 2024-12-29 03-04-23](https://github.com/user-attachments/assets/b4952bdb-6009-4019-85df-48f6ec27f1c7)



## Author

- [@theslabby](https://www.github.com/theslabby)


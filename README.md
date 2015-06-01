# Slider-Puzzle
A slider puzzle generator and solver written in C++

The initial goal of the project was to create a slider puzzle generator and solver using an A-star search algorithm.
The original method used a tree to store board states, but the constant addition of nodes was very memory-intensive and eventually crashed.
We decided to move in favor of a greedy algorithm, making use of the Manhattan Distance calculation to move tiles.

The program works by calculating the Manhattan Distance of the tiles surrounding the empty tile and making the optimal move, optimal being whichever move brings a tile closest to its home position on the board. The program continues to pick tiles using Manhattan Distance, keeping track of its last move so as not to repeat.

The function occassionally needs to forcibly make a move when an optimal move cannot be found to ensure the program doesn't stop prematurely. This causes some repetition in movement, but ultimately was necessary.

Though it is not able to solve any given puzzle, the program can handle most puzzles of varying size (3x3, 4x4, 5x5, etc.).

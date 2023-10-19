# MineSweeper
A program to solve minesweeper boards using some basic strategies

# Minesweeper Solver

This repository contains a Python program that can solve Minesweeper boards using a combination of logic and rule-based approaches. Minesweeper is a popular logic puzzle game where the objective is to uncover all safe squares on a grid while avoiding hidden mines. The game is played on a two-dimensional grid of squares, and each square can be either empty or contain a mine. Numbered squares indicate how many mines are adjacent to that square within its Moore neighborhood.

## Minesweeper Game

For those unfamiliar with the game, here's a brief overview:

- **Minesweeper**: [Learn More](https://en.wikipedia.org/wiki/Minesweeper_(video_game))
- **Moore Neighborhood**: [Learn More](https://en.wikipedia.org/wiki/Moore_neighborhood)

## Rules for Solving

In this version of Minesweeper, we'll use two rules to solve the grid by working out the unknown squares.

### Rule 1

If we've already discovered all the mines on the board, any unknown cell can simply be numbered with the count of mines in its Moore neighborhood. For example, if we know the total number of mines in the grid and have already found some mines, we can use this information to deduce the solution for unknown squares.

```plaintext
-----------------
|  0 1 1 ? 0 |
|  1 3 X 3 1 |
|  1 X X X 1 |
|  1 3 X 3 1 |
|  0 1 1 1 0 |
-----------------

```

In this case, the solution to the unknown square must be:

```plaintext
-----------------
|  0 1 1 1 0 |
|  1 3 X 3 1 |
|  1 X X X 1 |
|  1 3 X 3 1 |
|  0 1 1 1 0 |
-----------------

```

### Rule 2

For a known square with a number 'n' indicating the count of mines, with 'u' unknown and 'm' known mines in its Moore neighborhood, if `n = m + u` and `m > 0`, then all unknown squares must be mines. For instance:

```plaintext
-----------------
|  0 1 1 1 0 |
|  1 3 X 3 1 |
|  1 X X X 1 |
|  1 3 ? 3 1 |
|  0 1 1 1 0 |
-----------------

```

Applying Rule 2 to the middle square on the bottom row yields:

```plaintext
-----------------
|  0 1 1 1 0 |
|  1 3 X 3 1 |
|  1 X X X 1 |
|  1 3 X 3 1 |
|  0 1 1 1 0 |
-----------------

```

Repeated application of these rules will allow us to solve some Minesweeper boards, although not all boards may be solvable with this approach.

## How to Use the Solver

To use this Minesweeper solver, follow these steps:

1. Clone or download the repository.
2. Run the Python script to input Minesweeper board configurations and apply the solving rules.

Feel free to explore and modify the code to suit your needs.

Happy Minesweeper solving! 🎮💣

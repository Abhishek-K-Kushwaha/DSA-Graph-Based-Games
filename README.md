# Puzzle and Game Solvers

This repository contains implementations of various puzzle solvers and game algorithms. The projects demonstrate advanced programming concepts such as graph traversal, memoization, and hashing.

## Contents                                                           

1. [8-Puzzle Solver](#8-puzzle-solver)
2. [2x2x2 Rubik's Cube Solver](#2x2x2-rubiks-cube-solver)
3. [Sim Game Implementation](#sim-game-implementation)
4. [UpItUp Puzzle Solver](#upitup-puzzle-solver)
5. [Custom Queue Implementation](#custom-queue-implementation)

## 8-Puzzle Solver

**File:** `8-Puzzle.cpp`

This program implements a solver for the classic [8-Puzzle](https://www.geeksforgeeks.org/8-puzzle-problem-using-branch-and-bound/) problem. It uses breadth-first search (BFS) to find the shortest sequence of moves to transform the initial state of the puzzle into the goal state.

Key features:
- Efficient state representation
- BFS implementation for optimal solution
- Custom board state generation and manipulation

## 2x2x2 Rubik's Cube Solver

**File:** `Rubix_Cube_2X2.cpp`

This solver tackles the 2x2x2 Rubik's Cube, a simplified version of the classic 3x3x3 cube. It implements an algorithm to find a sequence of moves that solves the cube from any given scrambled state.

Key features:
- State representation for 2x2x2 Rubik's Cube
- Move generation and application
- Search algorithm for finding solution sequences

## Sim Game Implementation

**File:** `Sim.c`

This program implements the game of [Sim](https://en.wikipedia.org/wiki/Sim_(game)), a two-player graph theory game. Players take turns coloring edges of a complete graph, with the goal of avoiding the creation of a triangle of their color.

Key features:
- Graph representation and manipulation
- Game state evaluation
- Includes an AI opponent

## UpItUp Puzzle Solver

**File:** `UpItUp.cpp`

This program implements a solver for the [Up-It-Up Puzzle](https://www.youtube.com/watch?v=HqhC5u0uPi4) problem. It uses breadth-first search (BFS) to find the shortest sequence of moves to transform the initial state of the puzzle into the goal state.

Key features:
- Efficient state representation
- BFS implementation for optimal solution
- Custom board state generation and manipulation

## Custom Queue Implementation

**File:** `queue.hpp`

This is a custom implementation of a queue data structure used by the other programs in this repository. It provides basic queue operations optimized for the specific needs of the puzzle solvers and game implementations.

Key features:
- Efficient enqueue and dequeue operations
- Templated for type flexibility

# BoardGameAI Framework
## Goal
The goal is to provide an
- easy to use
- highly configurable and
- efficient

alpha beta pruning framework to implement an artificial intelligence for any zero-sum game.

The main idea is to clearly seperate between the search algoritm (incl. enhancements) and the domain specific game knowledge. Thus, making it easy to reuse the library for any board game. Any search extension like iterative deepening, transposition tables, history heuristics etc. can be enabled and disabled at compile time to provide the highest efficiency for every possible feature combination.

## Compiling
- Use CMake to generate either a Visual Studio solution on Windows or a Makefile project on Linux

## Getting started
1. Write a zero-sum game `MyGame` by inheriting from `BoardGame<MyGame>` (see Connect4Game.cpp for an example)
2. Implement the minimal set of methods: `moveGenerator_impl()`, `hasWon_impl()`, `isGameOver_impl()`, `makeMove_impl()`, `undoMove_impl()`
3. Create an AI by using the `AIBuilder` class: `auto ai = AIBuilder<MyGame>{}.iterativeDeepening().useTTable().create();` (see `AIBuilder_t` class for more options)
4. Search the whole game tree by calling: `SearchResult result = ai.search(game);` or by defining a depth and/or a  limit: `ai.search(game, INT_MAX, 3000); /* search for 3 seconds with an infinite depth`
5. `result` contains the position score and the best move

A more detailed documentation about the search extensions can be found below.

## Repository Structure
The repository consist of 4 main folders/projects:
- libs/BoardGameAI - the artifical intelligence framework
- libs/BoardGames - implementation of board games
- src/BoardGameConsoleApp - a simple console application to play those games
- src/BoardGameAIBenchmark - benchmark and compare the different search extensions
- test - test for the game logic

## Features
- negamax
- tracing (it's possible to print the game tree to get a better understanding how alpha beta pruning works)
- iterative deepening
- simple time management (search will abort after a certain time)
- move picker with move ordering (insertion sort)
- "replace by depth" tranposition table with configurable size
- principal variation search
- register callback for status updates during search (e.g. current search depth)

## Planned Features
- dynamic move ordering
- quiescence search
- other transposition table implementations (two big, replace always etc)
- killer moves
- support for more than two player games
- multi threaded
- support position book (e.g. opening book)

## Supported Games
- Connect Four

many more e.g. Othello, Quarto will be added from time to time

## General Game Playing
Compared to the [General Game Playing (GGP)](https://en.wikipedia.org/wiki/General_game_playing) approach, this framework was not designed to read the game rules from a Game Description Language (GDL) and to automatically find the best strategy by analyzing these rules. Instead, the focus is to provide an efficient library that can be used for a game which is implemented by an imperative language like C++. IMHO it's essential to have a fast move generator e.g. by using bitfields, but of course this is at the expense of loosing automatic analysing abilities for those rules. Therefore and in opposite to the GDL, the user can explicitly specify an evaluation function for games that are too complex to search till the end (e.g. chess).
Of course, you can try to use this framework as a base for a general game player.

## References
- Connect Four https://tromp.github.io/c4/c4.html

## Similar projects
- a similar implementation for python: https://github.com/Zulko/easyAI

This document describes the configurable search extensions and provides related links.

# Search algorithms

The search extensions can be usually enabled by using the Builder class ```AIBuilder<MyGame>{}```

## Negamax with alpha beta pruning
The base for the search without any extensions is a negamax implementation with alpha beta pruning (fail-soft variant):

```c++
template<bool isRoot>
int16_t alphaBeta(uint16_t depthLeft, int16_t alpha, int16_t beta)
{
  Move bestMove = MOVE_NONE;
  if (depthLeft == 0 || game->isGameOver()) {
    return evaluatePosition();
  }

  int16_t bestValue = -INFINITE;
  Move move;
  while ((move = movePicker.nextMove()) != MOVE_NONE)
  {
    game->makeMove(move);
    int16_t value = -alphaBeta<false>(depthLeft - 1, -beta, -alpha);
    game->undoMove();

    if (value >= beta) {
      bestValue = value;
      bestMove = move;
      break;
    }
    if (value > bestValue)
    {
      bestValue = value;
      bestMove = move;
      STATIC_IF(isRoot)
      {
        bestRootMove = move;
      }
      if (value > alpha)
      {
        alpha = value;
      }
    }
  }
  return bestValue;
}
```


More information can be found here:
https://en.wikipedia.org/wiki/Negamax

Usage: `AIBuilder<MyGame>{}.create();`

## Iterative Deepening

```c++
void searchIterative(unsigned int maxDepth)
{
  unsigned int currentMaxDepth = 0;
  int16_t score;

  while (currentMaxDepth < maxDepth && currentMaxDepth < MAX_PLY)
  {
    currentMaxDepth++;

    int score = alphaBeta<true>(currentMaxDepth, -INFINITE, INFINITE);
  }
}
```
More information can be found here:
https://en.wikipedia.org/wiki/Negamax

Usage: `AIBuilder<MyGame>{}.iterativeDeepening();`

## Transposition Table

More information can be found here:
http://mediocrechess.blogspot.de/2007/01/guide-transposition-tables.html

Usage:
- table with default (128MB) size: `AIBuilder<MyGame>{}.useTTable();`
- specify table size in MB (e.g. 512MB): `AIBuilder<MyGame>{}.useTTable<512>();`

### Table Entry

An entry in the transposition table stores the following properties:
* 32 bit key (high 32 bits of the Zobrist key)
* 16 bit move (best move for this positition)
* 8 bit bound (one of the following values: BOUND_NONE, BOUND_UPPER (fail low), BOUND_LOWER (fail high), BOUND_EXACT (exact score for certain depth))
* 16 bit depth (searched depth)
* 8 bit generation (to distinguish between new and old entries from a previous search)

### Replace by depth

This is currently the only supported option. The table entry is only replaced if either the depth of the new entry is greater than the existing one or if the table entry is from a previous search.


## Principal Variation Search

More information can be found here:
https://chessprogramming.wikispaces.com/Principal+Variation+Search

Usage: `AIBuilder<MyGame>{}.usePVS();`

More information can be found here:
https://chessprogramming.wikispaces.com/Principal+Variation+Search

## Mate Distance Pruning

```c++
STATIC_IF(MATE_DISTANCE_PRUNING_ENABLED(options))
{
  int16_t maxBeta = game->getMaxPossibleEvaluation();
  if (beta > maxBeta)
  {
    beta = maxBeta;
    if (alpha >= beta)
    {
      return beta;
    }
  }
}
```
More information can be found here:
https://chessprogramming.wikispaces.com/Mate+Distance+Pruning

Usage: `AIBuilder<MyGame>{}.useMateDistancePruning();`

If an evaluation function was implemented by overwriting `evaluate()` in the `BoardGame`, the method `maxPossibleEvaluation()` should then also be overwritten which returns the maximum possible value in the current position.

## Move picker

The move picker uses an internal state machine to try a move from the transposition table first and to postpone the (expensive) generation of moves (e.g. it is not required if the move from the transposition table leads to a cut off).
If static move evalution is used, then moves are sorted according to their value. However, the sorting is not done in advance but on-demand when the next move is picked. This is usally faster if only a few moves are available.

The implementation is based on the Stockfish move picker:

https://github.com/mcostalba/Stockfish/blob/master/src/movepick.cpp

# Game specific extensions

Some optimizations require a game dependent logic. Those can be usually implemented by inheriting from the class `BoardGame` and overwriting the methods.

## Position evaluation

Especially for complex games that are too difficult to analyze till the terminal state, it might be handy to evaluate an intermediate game state with a certain value.

Usage:
Overwrite `evaluate_impl()`

## Static move evaluation/ordering

For some games it is possible to sort the moves by a certain value e.g. in Connect4 it is usually better to place a piece in the center and not on the margins. Assign higher values to moves which have a higher chance to be the best move and smaller values for bad moves.

Usage:
To enable static move evaluation/ordering, the inhterited Board Game must set the template parameter `useStaticMoveEvaluation` to true:

```c++
class MyGame : public BoardGame<MyGame, true>
{

}
```

If the method `auto moveGenerator() const` is overwritten, the user must return an `ExtMove` in the `nextMove()` method instead of a `Move` whereas `ExtMove` is a tuple of move and (static) value.

If this option is enabled, move picking takes a bit longer because the moves are sorted first.

Please note: If possible, try to generate the moves in the move generator already by their value. This is usually faster than returning all moves and sorting them afterwards.

## Upper bound of plies

If the maximum number of [plies](https://en.wikipedia.org/wiki/Ply_(game_theory%29}) in a whole game is known an array with a fixed size is used, otherwise it's a vector.

Usage:
```c++
// A Connect4 game has maximum 42 moves per position:
class MyGame : public BoardGame<MyGame, useStaticMoveEvaluation, 42>
{

}
```

## Upper bound of moves per position

If the maxmum number of possible moves in any position is known in advance, an array with a fixed size is used, otherwise it's a vector. This has only an effect if static move evaluation is enabled, otherwise no array will be allocated.

Usage:
```c++
//A Connect4 game has maximum 7 moves per position:
class MyGame : public BoardGame<MyGame, useStaticMoveEvaluation, 42, 7>
{

}
```

## The MIT License (MIT)
Copyright (c) 2017 Alexander Krasnogolowy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

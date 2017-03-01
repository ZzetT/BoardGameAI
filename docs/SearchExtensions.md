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

Usage: `AIBuilder<Connect4Game>{}.usePVS();`

More information can be found here:
https://chessprogramming.wikispaces.com/Principal+Variation+Search

## Move picker

The move picker uses an internal state machine to try a move from the transposition table first and to delay the (expensive) generation of moves (e.g. it is not required if the move from the transposition table leads to a cut off).
Moves are sorted according to their value. However, the sorting is not done in advance but on-demand when the next move is picked. This is usally faster if only a few moves are available.

The implementation is based on the Stockfish move picker:

https://github.com/mcostalba/Stockfish/blob/master/src/movepick.cpp

# Game specific extensions

Some optimizations require a game dependent logic. Those can be usually implemented by inheriting from the class `BoardGame` and overwriting the methods.

## Static move evaluation/ordering

For some games it is possible to sort the moves by a certain value e.g. in Connect4 it is usually better to place a piece in the center and not on the margins.

Usage: If the method `void BoardGame::getMoves_impl(MoveList* moves)` is overwritten, it it possible to set a value for each move by using the method `void MoveList::addMove(move, value);`.

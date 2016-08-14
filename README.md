# BoardGameEngine
This library provides an alpha beta pruning C++ implementation that can be used for different board games (zero-sum game).

#Goal
The goal is to provide an efficient but easy to use framework to implement an artifical intelligence for any zero-sum game.
For each board game the user only needs to inherit from the class `AbstractBoardGame` and implements very basic methods like `getMoves()`,
 `makeMove()`, `undoMove()`, `isGameOver()`, `getHash()` and `evaluate()`.
 A lot of engines are available that play Chess or other games like Connect Four or Othello. And altohugh the search algorithms are the same or very similar,
 the alpha beta pruning is implemented again and again. The *BoardGameEngine* framework shall generalize this so that it can be used with any board game. If optimizations are added, they shall be applicable for other games, too.

#Repository Structure
The repository consist of 4 main folders/projects:
- BoardGameEngine - the artifical intelligence framework
- BoardGames - implementation of board games
- BoardGamesUI - a simple console application to play those games
- BoardGameEngineTest - tests for the search algorithms as well as for the game implementations (please provide tests for any new game)

#Features
- negamax 
- tracing (it's possible to print the game tree to get a better understanding how alpha beta pruning works)
- iterative deepening
- simple time management (search will abort after a certain time)

#Planned Features
- transposition tables
- quiescence search

#Supported Games
- Connect Four 

many more e.g. Othello, Quarto will be added from time to time

# References
- Connect Four https://tromp.github.io/c4/c4.html

# Similar projects
- a similar implementation for python: https://github.com/Zulko/easyAI

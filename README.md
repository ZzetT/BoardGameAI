# BoardGameEngine Framework
This library provides an alpha beta pruning C++ implementation that can be used for different board games (zero-sum game).

#Goal
The goal is to provide an efficient but easy to use framework to implement an artifical intelligence for any zero-sum game.
For each board game the user only needs to inherit from the class `AbstractBoardGame` and implements very basic methods like `getMoves()`,
 `makeMove()`, `undoMove()`, `isGameOver()`, `getHash()` and `hasWon()`.
 A lot of engines are available that play Chess or other games like Connect Four or Othello. And although the search algorithms are the same or very similar,
 the alpha beta pruning is implemented again and again. The *BoardGameEngine Framework* shall generalize this so that it can be used with any board game. If optimizations are added, they shall be applicable for other games, too.

#General Game Playing
Compared to the [General Game Playing (GGP)](https://en.wikipedia.org/wiki/General_game_playing) approach, this framework was not designed to read the game rules from a Game Description Language (GDL) and to automatically find the best strategy by analyzing these rules. Instead, the focus is to provide an efficient library that can be used for a game which is implemented by an imperative language like C++. IMHO it's essential to have a fast move generator e.g. by using bitfields, but of course this is at the expense of loosing automatic analysing abilities for those rules. Therefore and in opposite to the GDL, the user can explicitly specify an evaluation function for games that are too complex to search till the end (e.g. chess).
Of course, you can try to use this framework as a base for a general game player.

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
- static move ordering

#Planned Features
- transposition tables
- dynamic move ordering
- quiescence search

#Supported Games
- Connect Four 

many more e.g. Othello, Quarto will be added from time to time

# References
- Connect Four https://tromp.github.io/c4/c4.html

# Similar projects
- a similar implementation for python: https://github.com/Zulko/easyAI

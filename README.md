# BoardGameEngine Framework
#Goal
The goal is to provide an efficient but easy to use alpha beta pruning framework to implement an artifical intelligence for any zero-sum game.
For each board game the user only needs to inherit from the class `AbstractBoardGame` and implements very basic methods like `getMoves()`,
 `makeMove()`, `undoMove()`, `isGameOver()` and `hasWon()`.
 A lot of engines are available that play Chess or other games like Connect Four or Othello. And although the search algorithms and its extensions are the same or very similar,
 the search algorithms are implemented again and again. The *BoardGameEngine Framework* shall generalize this so that it can be used with any board game. If optimizations are added, they shall be applicable for other games, too (if possible).

#General Game Playing
Compared to the [General Game Playing (GGP)](https://en.wikipedia.org/wiki/General_game_playing) approach, this framework was not designed to read the game rules from a Game Description Language (GDL) and to automatically find the best strategy by analyzing these rules. Instead, the focus is to provide an efficient library that can be used for a game which is implemented by an imperative language like C++. IMHO it's essential to have a fast move generator e.g. by using bitfields, but of course this is at the expense of loosing automatic analysing abilities for those rules. Therefore and in opposite to the GDL, the user can explicitly specify an evaluation function for games that are too complex to search till the end (e.g. chess).
Of course, you can try to use this framework as a base for a general game player.

#Getting started
1. Write a zero-sum game by inheriting from `AbstractBoardGame` (see Connect4Game.cpp for an example)
2. Create an AI by using the `AIBuilder` class: `auto ai = AIBuilder{}.iterativeDeepening().useTTable().create();` (see `AIBuilder_t` class for more options)
3. Search the whole game tree by calling: `SearchResult result = ai.search(game);` or by defining a depth and/or a  limit: `ai.search(game, INT_MAX, 3000); /* search for 3 seconds with an infinite depth` 
4. `result` contains the position score and the best move


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
- move picker with move ordering (insertion sort)
- replace by depth tranposition table with configurable size

#Planned Features
- dynamic move ordering
- quiescence search
- other transposition table implementations (two big, replace always etc)
- history heuristic
- killer moves

#Supported Games
- Connect Four 

many more e.g. Othello, Quarto will be added from time to time

# References
- Connect Four https://tromp.github.io/c4/c4.html

# Similar projects
- a similar implementation for python: https://github.com/Zulko/easyAI

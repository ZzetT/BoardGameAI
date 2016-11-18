# BoardGameEngine Framework
#Goal
The goal is to provide an
- easy to use
- highly configurable and
- efficient

alpha beta pruning framework to implement an artifical intelligence for any zero-sum game.

The main idea is to clearly seperate between the search algoritm (incl. enhancements) and the domain specific game knowledge. Thus, making it easy to reuse the library for any board game. Any feature like iterative deepening, transposition tables, heuristic etc. can be enabled and disabled at compile time to provide the highest efficiency for every possible feature combination.

#Compiling
- Currently, the project is only available as a Visual Studio 2015 project (I'll try to provide a makefile in the future)
- To compile the project Clang for Windows (Version >= 3.9) is required (msvc is not working) or any compiler that supports the C++17 'constexpr' feature (used for compile time optimizations) (see http://llvm.org/releases/download.html)

#General Game Playing
Compared to the [General Game Playing (GGP)](https://en.wikipedia.org/wiki/General_game_playing) approach, this framework was not designed to read the game rules from a Game Description Language (GDL) and to automatically find the best strategy by analyzing these rules. Instead, the focus is to provide an efficient library that can be used for a game which is implemented by an imperative language like C++. IMHO it's essential to have a fast move generator e.g. by using bitfields, but of course this is at the expense of loosing automatic analysing abilities for those rules. Therefore and in opposite to the GDL, the user can explicitly specify an evaluation function for games that are too complex to search till the end (e.g. chess).
Of course, you can try to use this framework as a base for a general game player.

#Getting started
1. Write a zero-sum game `MyGame` by inheriting from `BoardGame<MyGame>` (see Connect4Game.cpp for an example)
2. Implement the minimal set of methods: `getMoves_impl()`, `hasWon_impl()`, `isGameOver_impl()`, `makeMove_impl()`, `undoMove_impl()`
3. Create an AI by using the `AIBuilder` class: `auto ai = AIBuilder<MyGame>{}.iterativeDeepening().useTTable().create();` (see `AIBuilder_t` class for more options)
4. Search the whole game tree by calling: `SearchResult result = ai.search(game);` or by defining a depth and/or a  limit: `ai.search(game, INT_MAX, 3000); /* search for 3 seconds with an infinite depth` 
5. `result` contains the position score and the best move


#Repository Structure
The repository consist of 4 main folders/projects:
- BoardGameEngine - the artifical intelligence framework
- BoardGames - implementation of board games
- BoardGamesUI - a simple console application to play those games
- BoardGameEngineCatchTest - tests for the search algorithms as well as for the game implementations (please provide tests for any new game)

#Features
- negamax 
- tracing (it's possible to print the game tree to get a better understanding how alpha beta pruning works)
- iterative deepening
- simple time management (search will abort after a certain time)
- move picker with move ordering (insertion sort)
- "replace by depth" tranposition table with configurable size
- history heuristic (same which is used by Tromp in his Connect4 engine)

#Planned Features
- dynamic move ordering
- quiescence search
- other transposition table implementations (two big, replace always etc)
- killer moves
- support for more than two player games
- multi threaded

#Supported Games
- Connect Four 

many more e.g. Othello, Quarto will be added from time to time

#References
- Connect Four https://tromp.github.io/c4/c4.html

#Similar projects
- a similar implementation for python: https://github.com/Zulko/easyAI

#The MIT License (MIT) 
Copyright (c) 2016 Alexander Krasnogolowy

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

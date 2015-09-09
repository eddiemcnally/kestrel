
First attempt at a C-based chess engine.

The code is developed on Debian Testing (gcc 5.2.x). Currently requires GNU C libraries to compile (I may remove this dependancy later). 

The ./test directory contains test fixtures for running unit tests (uses seatest test framework - see https://github.com/keithn/seatest). 

This is a work-in-progress. Current state:
- Can read in FEN
- Can generate moves
- Currently (Sept 2015) completes the perft tests without errors to a depth of 4 (going any deeper will require some performance optimisations).





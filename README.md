
First attempt at a C-based chess engine.

The code is developed on Debian Jessie (gcc 4.9.x). Currently requires GNU C libraries to compile (I may remove this dependancy later). 

The ./test directory contains test fixtures for running unit tests (uses seatest test framework - see https://github.com/keithn/seatest). 

This is a work-in-progress. Current state:
- Can read in FEN
- Can generate moves
- Currently (Sept 2015) ironing out bugs in the Perft tests.







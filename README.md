
First attempt at a C-based chess engine.

The code is developed on Debian Jessie (gcc 4.9.x). I avoided using using non-standard (non-GNU-C) libs to improve portability. At some stage, i'll replace 'make' with 'cmake'. 

The ./test directory contains test fixtures for running unit tests (uses seatest test framework - see https://github.com/keithn/seatest). 

To compile kestrel:
      - make all

To compile test framework
      - make test

This is a work-in-progress. Current state:
- can read in FEN notation and populate a board position
- can generate moves for all pieces on a board (albeit, the moves aren't scored yet)








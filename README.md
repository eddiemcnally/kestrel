
First attempt at a C-based chess engine.

The code is developed on Debian Jessie and is compiled with gcc 4.9.2. Currently there is a dependancy on gcc built-in functions.

The ./test directory contains test fixtures for running unit tests (uses the 'seatest' test framework - see https://github.com/keithn/seatest).

The ./src directory contains the main source code for the engine.

The ./debug directory contains some Python 3 code that was developed to help parse and analyse log files when debugging move generation.


Some implementation notes:
* At a very high level, I am using BlueFever Software (see youtube) and the CPW-Engine (http://chessprogramming.wikispaces.com/CPW-Engine) as guides to the implementation steps. 
* The engine makes heavy use of bitboards (uint64_t types to represent the board). Using bitboards allows for using pre-generated lookup bitmasks ("occupancy masks") to identify potential attacking squares for any piece on the board.
* As mentioned above, there is an dependency on gcc builtin functions. Specifically:
  * __builtin_ctzll
  * __builtin_bswap64
  * There is also a dependency on some gcc-specific #pragma's to disable some compiler warnings (sign conversions) in specific functions.


This is a work-in-progress. Current state:
* Can read in FEN
* Can generate moves with an initial implementation of an AlphaBeta search/evaluate functionality.
* Current state (Feb 2016) :
    * correctly completes the 126 perft tests to a depth of 6 
    * implements alpha-beta search with principle variation  
    * implements iterative deepening
    * can solve a mate-in-2 problem
    * Contains a primative Transposition Table
    * Contains a primative position eval function.
* The code is unoptimised, in the sense that no explicit effort has been undertaken to profile and tune the code paths (other than some rudamentary moving of code to improve inlining, removal of debug code and assert's and tweaking the gcc compile options).
* The main code (./src) doesn't execute any meaningful operations. The main launch point is the test framework. To build and execute the test framework, run the following:
	* cd ./test
	* make
	* ./test_kestrel
* In terms of performance, does about 15x10^6 moves/sec during the perft testing. Spec of host:
	- Intel® Core™ i5-3337U CPU @ 1.80GHz × 4
	- Debian GNU/Linux 8 (jessie) 64-bit
	- Laptop, clocked at 3592.11 Bogomips :-)


Feature set:
* Bitboards (and use of occupancy mask lookups)
* Iterative Deepening
* Alpha-beta with Principle Variation
* Zobrist hashing 
* Quiessence Search
* Piece Square Valuation




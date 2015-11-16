
First attempt at a C-based chess engine.

The code is developed on Debian Testing, and is compiled with gcc 5.2.x. Currently there is a dependancy on gcc built-in functions.

The ./test directory contains test fixtures for running unit tests (uses the 'seatest' test framework - see https://github.com/keithn/seatest).

The ./src directory contains the main source code for the engine.

The ./debug directory contains some Python 3 code that was developed to help parse and analyse log files when debugging move generation.


Some implementation notes:
* At a very high level, I am using BlueFever Software as a guide to the implementation steps. There are video tutorials on YouTube.
* The engine makes heavy use of bitboards (uint64_t types to represent the board). Using bitboards allows for using pre-generated lookup bitmasks ("occupancy masks") to identify potential attacking squares for any piece on the board.
* As mentioned above, there is an dependency on gcc builtin functions. The engine makes use of an inbuilt functions for determing LSB and MSB in a word, and to count bits in a word.
* The search is alpha-beta, with principle variation. The alpha-beta routine is called using iterative deepening.


This is a work-in-progress. Current state:
* Can read in FEN
* Can generate moves with an initial implementation of an AlphaBeta search/evaluate functionality.
* Current state (Nov 2015) :
    * completes the 126 perft tests to a depth of 6 (> 12x10^9 moves generated)
    * implements alpha-beta search with principle variation  
    * implements iterative deepening
* The code is unoptimised, in the sense that no explicit effort has been undertaken to profile and tune the code paths (other than some rudamentary moving of code to improve inlining, removal of debug code and assert's and tweaking the gcc compile options).
* The main code (./src) doesn't execute any meaningful operations. The main launch point is the test framework. To build and execute the test framework, run the following:
	* cd ./test
	* make
	* ./test_kestrel
* In terms of performance, does about 13x10^6 moves/sec during the perft testing. Spec of host:
	- Intel® Core™ i5-3337U CPU @ 1.80GHz × 4
	- Debian GNU/Linux stretch/sid 64-bit

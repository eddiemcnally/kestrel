
First attempt at a C-based chess engine.

The code is developed on Debian Testing (gcc 5.2.x). Currently requires GNU C libraries to compile (I may remove this dependancy later). 

The ./test directory contains test fixtures for running unit tests (uses the 'seatest' test framework - see https://github.com/keithn/seatest). 

The ./src directory contains the main source code for the engine.

The ./debug directory contains some Python 3 code that was developed to help parse and analyse log files when debugging move generation.



Some implementation notes:
* At a very high level, I am using BlueFever Software as a guide to the implementation steps. There are video tutorials on YouTube.
* The engine makes heavy use of bitboards (64bit 'unsigned long long int' to represent the board). Using bitboards allows for using pre-generated lookup (occupancy) bitmasks to identify potential attacking squares for any piece on the board.
* As mentioned above, there is an dependency on GNU C library (glibc). The engine makes use of an inbuilt functions for determing LSB and MSB in a word.


This is a work-in-progress. Current state:
* Can read in FEN
* Can generate moves (currently un-scored, and searching/pruning not implemented yet)
* Currently (Oct 2015) completes the perft tests without errors to a depth of 6 (> 12x10^9 moves generated)
* The code is unoptimised, in the sense that no explicit effort has been undertaken to profile and tune the code paths (other than some rudamentary moving of code to improve inlining, removal of debug code and assert's and tweaking the gcc compile options).
* The main code (./src) doesn't execute any meaningful operations. The main launch point is the test framework. To build and execute the test framework, run the following:
	* cd ./test
	* make
	* ./test_kestrel
* In terms of performance, does about 13x10^6 moves/sec. Spec of host:
	- Intel® Core™ i5-3337U CPU @ 1.80GHz × 4 
	- Debian GNU/Linux Testing (Stretch) 64-bit
	

Next steps:
- implement move scoring
- implement search (plus move pruning)
- look into what parts of the move gen code can be multi-threaded.





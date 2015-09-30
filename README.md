
First attempt at a C-based chess engine.

The code is developed on Debian Testing (gcc 5.2.x). Currently requires GNU C libraries to compile (I may remove this dependancy later). 

The ./test directory contains test fixtures for running unit tests (uses the 'seatest' test framework - see https://github.com/keithn/seatest). 

The ./src directory contains the main source code for the engine.

The ./debug directory contains some Python 3 code that was developed to help parse and analyse log files when debugging move generation.



Some implementation notes:
* At a very high level, I am using BlueFever Software as a guide to the implementation steps. There are video tutorials on YouTube.
* The engine makes heavy use of bitboards (64bit 'unsigned long long int' to represent the board). Using bitboards allows for using pre-generated lookup bitmasks to identify potential attacking squares for any piece on the board.
* As mentioned above, there is an dependency on GNU C library (glibc). The engine makes use of an inbuilt function for determing LSB and MSB in a word.


This is a work-in-progress. Current state:
* Can read in FEN
* Can generate moves (currently un-scored, and pruning not implemented)
* Currently (Sept 2015) completes the perft tests without errors to a depth of 6 (> 12x10^9 move generated)
* The code is unoptimised, in the sense that no explicit effort has been undertaken to profile and tune the code paths.
* The main code (./src) doesn't execute any meaningful operations. The main launch point is the test framework. To build and execute the test framework, run the following:
	* cd ./test
	* make
	* ./test_kestrel
	

Next steps:
- implement move scoring
- implement pruning of search tree
- look into what parts of the move gen code can be multi-threaded.





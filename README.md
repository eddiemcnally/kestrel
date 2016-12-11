
#Overview
First attempt at a C-based chess engine.

The code is developed on Debian Jessie and OpenSUSE Leap 42.1, and is compiled with Clang with LTO. Currently there is a dependancy on gcc built-in functions.

The ./test directory contains test fixtures for running unit tests (uses the 'seatest' test framework - see https://github.com/keithn/seatest).

The ./src directory contains the main source code for the engine.

The ./debug directory contains some Python 3 code that was developed to help parse and analyse log files when debugging move generation.

# How To Compile
The build environment is CMake.

To compile:

	mkdir build

	cd build

	cmake .. -G "Unix Makefiles"


##Notes

###Use of LLVM LTO
- The cmake files are set up to use LLVM/Clang LTO (see http://llvm.org/docs/LinkTimeOptimization.html).
- For the link to succeed, you need ensure that the "gold" linker is used, as follows:
'''
$ update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.gold" 20
$ update-alternatives --install "/usr/bin/ld" "ld" "/usr/bin/ld.bfd" 10
'''

To check the correct version is enabled:
'''
$ ld --version
'''

You should see something like the following:

	>GNU gold (GNU Binutils for Debian 2.25) 1.11
	>Copyright (C) 2014 Free Software Foundation, Inc.



###Runtime Asserts
- To aid debugging, there is a single compile-time variable called ENABLE_ASSERTS. Compiling with this set will enable various runtime checker code and asserts.


#Some implementation notes:
* At a very high level, I am using BlueFever Software (see youtube) and the CPW-Engine (http://chessprogramming.wikispaces.com/CPW-Engine) as guides to the implementation steps. 
* The engine makes heavy use of bitboards (uint64_t types to represent the board). Using bitboards allows for using pre-generated lookup bitmasks ("occupancy masks") to identify potential attacking squares for any piece on the board.
* As mentioned above, there is an dependency on gcc builtin functions. Specifically:
  * __builtin_ctzll
  * __builtin_bswap64
  * __builtin_popcntll
  * There is also a dependency on some gcc-specific #pragma's to disable some compiler warnings (sign conversions) in specific functions.


#Current Status
This is a work-in-progress. Current state:
* Can read in FEN
* Can generate moves (pertf tests)
* Implements alpha-beta searching with quiescence.
* Current state (March 2016) :
    * correctly completes the 126 perft tests to a depth of 6 
    * implements alpha-beta search with principle variation  
    * implements iterative deepening
    * can solve a mate-in-3 problem
    * Contains a primative position eval function.
    * Starting on an implementation of the UCI protocol
* The code is unoptimised, in the sense that no explicit effort has been undertaken to profile and tune the code paths (other than some rudamentary moving of code to improve inlining, removal of debug code and assert's and tweaking the gcc compile options).
* The main code (./src) doesn't execute any meaningful operations. The main launch point is the test framework. 

#Performance
In terms of performance, does about 12x10^6 moves/sec during the perft testing. Spec of host:
	- Intel® Core™ i5-3337U CPU @ 1.80GHz × 4
	- Laptop, clocked at 3592.11 Bogomips :-)
Note: the engine is still incomplete, so expecting these numbers to only worsen!

#Feature set
* Bitboards (and use of occupancy mask lookups)
* Iterative Deepening
* Alpha-beta with Principle Variation
* Move sorting using PV, captures, killer moves, history heuristics
* Zobrist hashing 
* Quiessence Search
* Piece Square Valuation




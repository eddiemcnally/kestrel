cmake_minimum_required(VERSION 3.0)

project(kestrel)

set(CMAKE_C_COMPILER "/usr/bin/clang")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

#
# sources and headers shared between the main (kestrel) binary
# and the test runner binary
#
set(COMMON_SOURCES  
		src/occupancy_mask.c
     		src/occupancy_mask.h
     		src/board.c
     		src/board.h
     		src/board_utils.c
     		src/board_utils.h
     		src/fen.c
     		src/fen.h
     		src/pieces.c
     		src/pieces.h
     		src/types.h
     		src/init.c
     		src/init.h
     		src/attack.c
     		src/attack.h
     		src/makemove.c
     		src/makemove.h
     		src/move_gen.c
     		src/move_gen.h
     		src/move_gen_utils.c
     		src/move_gen_utils.h
     		src/evaluate.c
     		src/evaluate.h
     		src/hashkeys.c
     		src/hashkeys.h
     		src/search.c
     		src/search.h
     		src/tt.c
     		src/tt.h
     		src/uci_protocol.c
     		src/uci_protocol.h
     		src/utils.c
     		src/utils.h)


#
# main entry point for kestrel engine
#
set(TARGET_SOURCES src/kestrel.c)


#
# source and header files for the test runner
#
set(TEST_SOURCES 
		test/all_tests.c
		test/attack_test_fixture.c
		test/board_test_fixture.c
		test/move_test_fixture.c
		test/occupancy_mask_test_fixture.c
		test/performance_tests.c
		test/piece_test_fixture.c
		test/search_tests.c
		test/seatest.c
		test/utils_test_feature.c
		test/all_tests.h
		test/attack_test_fixture.h
		test/board_test_fixture.h
		test/move_test_fixture.h
		test/occupancy_mask_test_fixture.h
		test/performance_tests.h
		test/piece_test_fixture.h
		test/search_tests.h
		test/seatest.h
		test/utils_test_feature.h
)


add_executable(kestrel ${COMMON_SOURCES} ${TARGET_SOURCES})
add_executable(test_kestrel ${COMMON_SOURCES} ${TEST_SOURCES})

#
# set up so the test runner app sees the header files in 
# the main src directory
#
target_include_directories(test_kestrel BEFORE PUBLIC src)


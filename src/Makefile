#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'kestrel'
# 'make all'	makes executable file 'kestrel'
# 'make clean'  removes all .o and executable files
# 'make test'	makes the unit test executable 'test_kestrel'

# define the C compiler to use
	CC	=	gcc

# define any compile-time flags
# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
	CFLAGS	=	-O3 -g -std=c11 -mtune=native -lmcheck -fsanitize=undefined \
				--param large-function-growth=8000 --param max-inline-insns-single=30000 \
				-fomit-frame-pointer -Wpedantic -Wformat=2 -Wall \
				-Wstrict-prototypes -Werror \
				-Wlogical-op -Wextra -Wconversion \
				-Wsign-conversion -Wshadow -Wmissing-include-dirs \
				-Wpointer-arith -Winline -lubsan

# flags for profiling code
#	CFLAGS	=	-O3 -pg -std=c11

# more lax compiler options
#	CFLAGS	=	-Wall -g -std=c11

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I.	-Itest

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm

# define the C source files
	SRCS		=	main.c board.c pieces.c board_utils.c move.c \
					init.c hashkeys.c fen.c utils.c attack.c \
					occupancy_mask.c makemove.c
	TEST_SRCS	=	board.c pieces.c board_utils.c move.c init.c \
					hashkeys.c fen.c utils.c attack.c occupancy_mask.c \
					makemove.c \
					./test/all_tests.c ./test/board_test_fixture.c \
					./test/piece_test_fixture.c ./test/seatest.c \
					./test/occupancy_mask_test_fixture.c \
					./test/attack_test_fixture.c ./test/utils_test_feature.c \
					./test/move_test_fixture.c ./test/performance_tests.c


# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
	OBJS		=	$(SRCS:.c=.o)
	TEST_OBJS	=	$(TEST_SRCS:.c=.o)


# define the executable files
	MAIN		=	kestrel
	TEST_MAIN	=	test_kestrel

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY:	depend	clean

all:	$(MAIN)
		@echo  'kestrel' build complete

test:	$(TEST_MAIN)
		@echo  'test_kestrel' build complete

$(MAIN):		$(OBJS)
				$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

$(TEST_MAIN):	$(TEST_OBJS)
				$(CC) $(CFLAGS) $(INCLUDES) -o $(TEST_MAIN) $(TEST_OBJS) $(LFLAGS) $(LIBS)


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		$(RM) ./test/*.o *~ $(TEST_MAIN)


depend:	$(SRCS)
		makedepend $(INCLUDES) $^
		$(TEST_SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

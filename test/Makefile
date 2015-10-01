#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'test_kestrel'
# 'make all'	makes executable file 'test_kestrel'
# 'make clean'  removes all .o and executable files
#
# define the C compiler to use
	CC	=	gcc

# define any compile-time flags
# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
	CFLAGS	=	-O3 \
				-g -std=c11 \
				-mtune=native \
				-lmcheck \
				-fsanitize=undefined \
				--param large-function-growth=20000 \
				--param max-inline-insns-single=90000 \
				-Wformat=2 \
				-Wall \
				-Werror \
				-Wlogical-op \
				-Wextra \
				-Wconversion \
				-Wsign-conversion \
				-Wshadow \
				-Wmissing-include-dirs \
				-Wpointer-arith \
				-Winline \
				-lubsan
#
# flags for profiling code
#	CFLAGS	=	-O3 -pg -std=c11

# more lax compiler options
#	CFLAGS	=	-Wall -g -std=c11 -lubsan

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I. -I../src

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
#LFLAGS = -L/home/newhall/lib  -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname
#   option, something like (this will link in libmylib.so and libm.so:
#LIBS = -lmylib -lm

# define the C source files
	SRCS	=	../src/board.c \
				../src/pieces.c \
				../src/board_utils.c \
				../src/move.c \
				../src/init.c \
				../src/hashkeys.c \
				../src/fen.c \
				../src/utils.c \
				../src/attack.c \
				../src/occupancy_mask.c \
				../src/makemove.c \
				all_tests.c \
				board_test_fixture.c \
				piece_test_fixture.c \
				seatest.c \
				occupancy_mask_test_fixture.c \
				attack_test_fixture.c \
				utils_test_feature.c \
				move_test_fixture.c \
				performance_tests.c


# define the C object files
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
	OBJS		=	$(SRCS:.c=.o)


# define the executable files
	MAIN		=	test_kestrel

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY:	depend	clean

all:	$(MAIN)
		@echo  'test_kestrel' build complete

$(MAIN):		$(OBJS)
				$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		$(RM) ../src/*.o *~ $(MAIN)


depend:	$(SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'test_kestrel'
# 'make all'	makes executable file 'test_kestrel'
# 'make clean'  removes all .o and executable files
#
	CC	=	gcc

# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
CFLAGS	=		-O3 -flto -std=c11 \
				-march=native \
				-mtune=native \
				-lmcheck \
				--param large-function-growth=9000000 \
				--param max-inline-insns-single=900000 \
				-Wformat=2 \
				-Wall \
				-Werror \
				-Wlogical-op \
				-Wextra \
				-Wconversion \
				-Wsign-conversion \
				-Wshadow \
				-Wcast-qual \
				-Wcast-align \
				-Wmissing-include-dirs \
				-Wpointer-arith \
				-lubsan

# flags for profiling code
#	CFLAGS	=	-O3 -pg -std=c11

# more lax compiler options
#	CFLAGS	=	-Wall -g -std=c11 -lubsan

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I. -I../src

# define the C source files
	SRCS	=	../src/board.c \
				../src/pieces.c \
				../src/board_utils.c \
				../src/move.c \
				../src/init.c \
				../src/hashkeys.c \
				../src/fen.c \
				../src/search.c \
				../src/pv_table.c \
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
	OBJS		=	$(SRCS:.c=.o)


# define the executable files
	MAIN		=	test_kestrel


.PHONY:	depend	clean

all:	$(MAIN)
		@echo  'test_kestrel' build complete

$(MAIN):		$(OBJS)
				$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)


.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)
		$(RM) ../src/*.o *~ $(MAIN)


depend:	$(SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

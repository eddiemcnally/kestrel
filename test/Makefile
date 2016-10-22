#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'test_kestrel'
# 'make all'	makes executable file 'test_kestrel'
# 'make clean'  removes all .o and executable files
#
	CC	=	clang

# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
#	CFLAGS	=	-O2 \
#				-flto \
#				-std=c11 \
#				-march=native \
#				-mtune=native \
#				-fsanitize=cfi \
#				-fsanitize=undefined 
				
#
# flags for profiling code
#	CFLAGS	=	-O3 -pg -std=c11 -march=native -mtune=native
	
	
#
# more lax compiler options
	CFLAGS	=	-Wall -g -std=c11 -fsanitize=address -DENABLE_ASSERTS=TRUE

#	CFLAGS	=	-Wall -g -std=c11 -fsanitize=address 

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I. -I../src

# define the C source files
	SRCS	=	../src/board.c \
				../src/pieces.c \
				../src/board_utils.c \
				../src/move_gen.c \
				../src/move_gen_utils.c \
				../src/init.c \
				../src/hashkeys.c \
				../src/fen.c \
				../src/search.c \
				../src/tt.c \
				../src/utils.c \
				../src/attack.c \
				../src/occupancy_mask.c \
				../src/makemove.c \
				../src/evaluate.c \
				../src/uci_protocol.c \
				all_tests.c \
				board_test_fixture.c \
				piece_test_fixture.c \
				seatest.c \
				occupancy_mask_test_fixture.c \
				attack_test_fixture.c \
				utils_test_feature.c \
				move_test_fixture.c \
				performance_tests.c \
				search_tests.c


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

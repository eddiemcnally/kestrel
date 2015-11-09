#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'kestrel'
# 'make all'	makes executable file 'kestrel'
# 'make clean'  removes all .o and executable files

	CC	=	gcc

# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
	CFLAGS	=	-O3 -std=iso9899:2011 \
				-flto \
				-march=native \
				-mtune=native \
				-lmcheck \
				-fsanitize=undefined \
				-fomit-frame-pointer \
				--param large-function-growth=20000 \
				--param max-inline-insns-single=90000 \
				-Wpedantic \
				-Wformat=2 \
				-Wall \
				-Wstrict-prototypes \
				-Wmissing-prototypes \
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
#	CFLAGS	=	-O3 -pg -std=iso9899:2011

# more lax compiler options
#	CFLAGS	=	-Wall -g -std=iso9899:2011 -lubsan

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I.

# define the C source files
	SRCS		=	kestrel.c board.c pieces.c board_utils.c \
				move.c init.c hashkeys.c fen.c utils.c \
				attack.c occupancy_mask.c makemove.c \
				search.c pv_table.c evaluate.c

# define the C object files
#
	OBJS		=	$(SRCS:.c=.o)


# define the executable files
	MAIN		=	kestrel

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY:	depend	clean

all:	$(MAIN)
		@echo  'kestrel' build complete

$(MAIN):		$(OBJS)
				$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)


.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend:	$(SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

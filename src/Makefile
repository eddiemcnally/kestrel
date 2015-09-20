#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'kestrel'
# 'make all'	makes executable file 'kestrel'
# 'make clean'  removes all .o and executable files

# define the C compiler to use
	CC	=	gcc

# define any compile-time flags
# NOTE: setting the inline limit values to be obscenely high since the size of
#		the binary isn't a concern
	CFLAGS	=	-O3 -std=iso9899:2011 \
				-mtune=native \
				-lmcheck \
				-fsanitize=undefined \
				--param large-function-growth=20000 \
				--param max-inline-insns-single=90000 \
				-fomit-frame-pointer \
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
				-Winline \
				-lubsan

# flags for profiling code
#	CFLAGS	=	-O3 -pg -std=iso9899:2011

# more lax compiler options
#	CFLAGS	=	-Wall -g -std=iso9899:2011 -lubsan

# define any directories containing header files other than /usr/include
#
	INCLUDES =	-I.

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


# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# (see the gnu make manual section about automatic variables)
.c.o:
		$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
		$(RM) *.o *~ $(MAIN)

depend:	$(SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

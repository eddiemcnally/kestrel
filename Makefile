#
# 'make depend' uses makedepend to automatically generate dependencies
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'ece'
# 'make all'	makes executable file 'ece'
# 'make clean'  removes all .o and executable files
# 'make test'	makes the unit test executable 'test_ece'

# define the C compiler to use
	CC	=	gcc

# define any compile-time flags
	CFLAGS	=	-Wall -g -std=c11 -o3

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
	SRCS		=	board.c	main.c	pieces.c	board_utils.c
	TEST_SRCS	=	board.c	pieces.c	board_utils.c 	test/all_tests.c 	test/board_test_fixture.c	test/piece_test_fixture.c   test/seatest.c


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


# define the executable file
	MAIN		=	ece
	TEST_MAIN	=	test_ece

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY:	depend	clean

all:	$(MAIN)
		@echo  'ece' App build complete

test:	$(TEST_MAIN)
		@echo  'test_ece' App build complete

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
		$(RM) *.o *~ $(TEST_MAIN)


depend:	$(SRCS)
		makedepend $(INCLUDES) $^
		$(TEST_SRCS)
		makedepend $(INCLUDES) $^
# DO NOT DELETE THIS LINE -- make depend depends on it.

board.o: /usr/include/stdio.h /usr/include/features.h
board.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
board.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
board.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
board.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
board.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
board.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
board.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
board.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
board.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
board.o: /usr/include/time.h /usr/include/sys/select.h
board.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
board.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
board.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
board.o: /usr/include/bits/stdlib-float.h board.h pieces.h
main.o: /usr/include/stdio.h /usr/include/features.h
main.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
main.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
main.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
main.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
main.o: /usr/include/time.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
main.o: /usr/include/bits/stdlib-float.h board.h
pieces.o: /usr/include/stdio.h /usr/include/features.h
pieces.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
pieces.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
pieces.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
pieces.o: /usr/include/libio.h /usr/include/_G_config.h /usr/include/wchar.h
pieces.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
pieces.o: /usr/include/stdlib.h /usr/include/bits/waitflags.h
pieces.o: /usr/include/bits/waitstatus.h /usr/include/endian.h
pieces.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
pieces.o: /usr/include/bits/byteswap-16.h /usr/include/sys/types.h
pieces.o: /usr/include/time.h /usr/include/sys/select.h
pieces.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
pieces.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
pieces.o: /usr/include/bits/pthreadtypes.h /usr/include/alloca.h
pieces.o: /usr/include/bits/stdlib-float.h /usr/include/ctype.h
pieces.o: /usr/include/xlocale.h board.h pieces.h
board_utils.o: /usr/include/stdio.h /usr/include/features.h
board_utils.o: /usr/include/stdc-predef.h /usr/include/sys/cdefs.h
board_utils.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
board_utils.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
board_utils.o: /usr/include/libio.h /usr/include/_G_config.h
board_utils.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
board_utils.o: /usr/include/bits/sys_errlist.h /usr/include/stdlib.h
board_utils.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
board_utils.o: /usr/include/endian.h /usr/include/bits/endian.h
board_utils.o: /usr/include/bits/byteswap.h /usr/include/bits/byteswap-16.h
board_utils.o: /usr/include/sys/types.h /usr/include/time.h
board_utils.o: /usr/include/sys/select.h /usr/include/bits/select.h
board_utils.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
board_utils.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
board_utils.o: /usr/include/alloca.h /usr/include/bits/stdlib-float.h
board_utils.o: /usr/include/string.h /usr/include/xlocale.h board.h pieces.h

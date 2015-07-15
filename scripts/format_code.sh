#!/bin/bash 

# script to reformat the code to agree with Linux Kernel coding standards (as good as any other standard!)
#
# uses the gnu indent utility (see https://www.gnu.org/software/indent/manual/indent.html)
#
#
options="-nbad -bap -nbc -bbo -hnl -br -brs -c33 -cd33 -ncdb -ce -ci4 -cli0 -d0 -di1 -nfc1 -i8 -ip0 -l80 -lp -npcs -nprs -npsl -sai -saf -saw -ncs -nsc -sob -nfca -cp33 -ss -ts8 -il1 "


indent $options ../*.c
indent $options ../*.h

indent $options ../test/*.c
indent $options ../test/*.h




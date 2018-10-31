# Makefile: solutions_to_assgn/ch7
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../../common.h and ../../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  query_creds query_creds_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN :=
 # user will need to explicitly set libraries to link in as required;
 # f.e. -lrt -pthread

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../../common.c ../../common.h
	${CC} ${CFLAGS} -c ../../common.c -o common.o
common_dbg.o: ../../common.c ../../common.h
	${CC} ${CFLAGS_DBG} -c ../../common.c -o common_dbg.o

#--- Target :: query_creds
query_creds.o: query_creds.c
	${CC} ${CFLAGS} -c query_creds.c -o query_creds.o
query_creds: common.o query_creds.o
	${CC} -o query_creds query_creds.o common.o ${LINKIN}

query_creds_dbg.o: query_creds.c
	${CC} ${CFLAGS_DBG} -c query_creds.c -o query_creds_dbg.o
query_creds_dbg: query_creds_dbg.o common_dbg.o
	${CC} -o query_creds_dbg query_creds_dbg.o common_dbg.o ${LINKIN}

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

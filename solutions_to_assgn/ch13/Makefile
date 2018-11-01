# Makefile
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
#----------------------------------------------------------------------
ALL :=  3intv 3intv_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN=
   # set as required to libraries to link in; f.e.  -lrt

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../../common.c ../../common.h
	${CC} ${CFLAGS} -c ../../common.c -o common.o
common_dbg.o: ../../common.c ../../common.h
	${CC} ${CFLAGS_DBG} -c ../../common.c -o common_dbg.o

#--- Target :: 3intv
3intv.o: 3intv.c
	${CC} ${CFLAGS} -c 3intv.c -o 3intv.o
3intv: common.o 3intv.o
	${CC} -o 3intv 3intv.o common.o ${LINKIN}

3intv_dbg.o: 3intv.c
	${CC} ${CFLAGS_DBG} -c 3intv.c -o 3intv_dbg.o
3intv_dbg: 3intv_dbg.o common_dbg.o
	${CC} -o 3intv_dbg 3intv_dbg.o common_dbg.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

# Makefile : ch14/param_passing/
#----------------------------------------------------------------------
#  ASSUMPTIONS ::
#   1. the convenience files ../common.h and ../common.c
#      are present
#   2. the clang/LLVM compiler is installed
#   3. the indent utility is installed
#
#   WARNING! Do NOT start a source filename with 'core' !
#       (will get Erased when 'make clean' is performed).
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  struct_as_param struct_as_param_dbg \
	pthreads1_wrong pthreads1_wrong_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

#LINKIN=
 # user will need to explicitly set libraries to link in as required; f.e. -lrt

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../../common.c ../../common.h
	${CC} ${CFLAGS} -c ../../common.c -o common.o
common_dbg.o: ../../common.c ../../common.h
	${CC} ${CFLAGS_DBG} -c ../../common.c -o common_dbg.o

 #--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../../common.c ../../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../../common.c -o common_dbg_msan.o

#--- Target :: struct_as_param
struct_as_param.o: struct_as_param.c
	${CC} ${CFLAGS} -c struct_as_param.c -o struct_as_param.o
struct_as_param: common.o struct_as_param.o
	${CC} -o struct_as_param struct_as_param.o common.o -lpthread

struct_as_param_dbg.o: struct_as_param.c
	${CC} ${CFLAGS_DBG} -c struct_as_param.c -o struct_as_param_dbg.o
struct_as_param_dbg: struct_as_param_dbg.o common_dbg.o
	${CC} -o struct_as_param_dbg struct_as_param_dbg.o common_dbg.o -lpthread

#--- Target :: pthreads1_wrong
pthreads1_wrong.o: pthreads1_wrong.c
	${CC} ${CFLAGS} -c pthreads1_wrong.c -o pthreads1_wrong.o
pthreads1_wrong: common.o pthreads1_wrong.o
	${CC} -o pthreads1_wrong pthreads1_wrong.o common.o -lpthread

pthreads1_wrong_dbg.o: pthreads1_wrong.c
	${CC} ${CFLAGS_DBG} -c pthreads1_wrong.c -o pthreads1_wrong_dbg.o
pthreads1_wrong_dbg: pthreads1_wrong_dbg.o common_dbg.o
	${CC} -o pthreads1_wrong_dbg pthreads1_wrong_dbg.o common_dbg.o -lpthread


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

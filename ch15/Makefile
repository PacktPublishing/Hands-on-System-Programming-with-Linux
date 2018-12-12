# Makefile : ch15/
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
ALL :=  mutex1 mutex1_dbg \
	facto facto_dbg \
	pshared_mutex_demo pshared_mutex_demo_dbg \
	cv_simple cv_simple_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG -pthread
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG -pthread
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined
CFLAGS_DBG_TSAN=${CFLAGS_DBG} -fsanitize=thread

LINKIN := -pthread
   # set as required to libraries to link in; f.e.  -lrt

# user will need to explicitly set libraries to link in as required; f.e. -lrt

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o

 #--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_ASAN} -c ../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_UB} -c ../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_MSAN} -c ../common.c -o common_dbg_msan.o
common_dbg_tsan.o: ../common.c ../common.h
	${CL} ${CFLAGS_DBG_TSAN} -c ../common.c -o common_dbg_tsan.o

#--- Target :: mutex1
mutex1.o: mutex1.c
	${CC} ${CFLAGS} -c mutex1.c -o mutex1.o
mutex1: common.o mutex1.o
	${CC} -o mutex1 mutex1.o common.o ${LINKIN}

mutex1_dbg.o: mutex1.c
	${CC} ${CFLAGS_DBG} -c mutex1.c -o mutex1_dbg.o
mutex1_dbg: mutex1_dbg.o common_dbg.o
	${CC} -o mutex1_dbg mutex1_dbg.o common_dbg.o ${LINKIN}
mutex1_dbg_tsan.o: mutex1.c
	${CL} ${CFLAGS_DBG_TSAN} -c mutex1.c -o mutex1_dbg_tsan.o
mutex1_dbg_tsan: mutex1_dbg_tsan.o common_dbg_tsan.o
	${CL} ${CFLAGS_DBG_TSAN} -o mutex1_dbg_tsan mutex1_dbg_tsan.o common_dbg_tsan.o ${LINKIN}

#--- Target :: facto
facto.o: facto.c
	${CC} ${CFLAGS} -c facto.c -o facto.o
facto: common.o facto.o
	${CC} -o facto facto.o common.o ${LINKIN}

facto_dbg.o: facto.c
	${CC} ${CFLAGS_DBG} -c facto.c -o facto_dbg.o
facto_dbg: facto_dbg.o common_dbg.o
	${CC} -o facto_dbg facto_dbg.o common_dbg.o ${LINKIN}

#--- Target :: pshared_mutex_demo
pshared_mutex_demo.o: pshared_mutex_demo.c
	${CC} ${CFLAGS} -c pshared_mutex_demo.c -o pshared_mutex_demo.o
pshared_mutex_demo: common.o pshared_mutex_demo.o
	${CC} -o pshared_mutex_demo pshared_mutex_demo.o common.o ${LINKIN}

pshared_mutex_demo_dbg.o: pshared_mutex_demo.c
	${CC} ${CFLAGS_DBG} -c pshared_mutex_demo.c -o pshared_mutex_demo_dbg.o
pshared_mutex_demo_dbg: pshared_mutex_demo_dbg.o common_dbg.o
	${CC} -o pshared_mutex_demo_dbg pshared_mutex_demo_dbg.o common_dbg.o ${LINKIN}

#--- Target :: cv_simple
cv_simple.o: cv_simple.c
	${CC} ${CFLAGS} -c cv_simple.c -o cv_simple.o
cv_simple: common.o cv_simple.o
	${CC} -o cv_simple cv_simple.o common.o ${LINKIN}

cv_simple_dbg.o: cv_simple.c
	${CC} ${CFLAGS_DBG} -c cv_simple.c -o cv_simple_dbg.o
cv_simple_dbg: cv_simple_dbg.o common_dbg.o
	${CC} -o cv_simple_dbg cv_simple_dbg.o common_dbg.o ${LINKIN}

cv_simple_dbg_tsan.o: cv_simple.c
	${CL} ${CFLAGS_DBG_TSAN} -c cv_simple.c -o cv_simple_dbg_tsan.o
cv_simple_dbg_tsan: cv_simple_dbg_tsan.o common_dbg_tsan.o
	${CL} ${CFLAGS_DBG_TSAN} -o cv_simple_dbg_tsan cv_simple_dbg_tsan.o common_dbg_tsan.o ${LINKIN}

#--- Target :: cv_simple_noprintf ; no printf's used, so that cry-baby helgrind stays happy
cv_simple_noprintf.o: cv_simple_noprintf.c
	${CC} ${CFLAGS} -c cv_simple_noprintf.c -o cv_simple_noprintf.o
cv_simple_noprintf: common.o cv_simple_noprintf.o
	${CC} -o cv_simple_noprintf cv_simple_noprintf.o common.o ${LINKIN}

cv_simple_noprintf_dbg.o: cv_simple_noprintf.c
	${CC} ${CFLAGS_DBG} -c cv_simple_noprintf.c -o cv_simple_noprintf_dbg.o
cv_simple_noprintf_dbg: cv_simple_noprintf_dbg.o common_dbg.o
	${CC} -o cv_simple_noprintf_dbg cv_simple_noprintf_dbg.o common_dbg.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

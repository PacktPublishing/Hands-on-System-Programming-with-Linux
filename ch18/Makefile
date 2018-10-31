# Makefile : HOSPL book, ch18/
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
ALL := sgio_simple sgio_simple_dbg \
	mmap_file_simple mmap_file_simple_dbg \
	diskspc

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

#--- Target :: sgio_simple
sgio_simple.o: sgio_simple.c
	${CC} ${CFLAGS} -DUSE_LOCAL_GOTO=1 -c sgio_simple.c -o sgio_simple.o
sgio_simple: common.o sgio_simple.o
	${CC} -o sgio_simple sgio_simple.o common.o ${LINKIN}

sgio_simple_dbg.o: sgio_simple.c
	${CC} ${CFLAGS_DBG} -DUSE_LOCAL_GOTO=1 -c sgio_simple.c -o sgio_simple_dbg.o
sgio_simple_dbg: sgio_simple_dbg.o common_dbg.o
	${CC} -o sgio_simple_dbg sgio_simple_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for sgio_simple :: (use clang): <foo>_dbg_[asan|ub|msan]
sgio_simple_dbg_asan.o: sgio_simple.c
	${CL} ${CFLAGS_DBG_ASAN} -DUSE_LOCAL_GOTO=1 -c sgio_simple.c -o sgio_simple_dbg_asan.o
sgio_simple_dbg_asan: sgio_simple_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o sgio_simple_dbg_asan sgio_simple_dbg_asan.o common_dbg_asan.o ${LINKIN}

sgio_simple_dbg_ub.o: sgio_simple.c
	${CL} ${CFLAGS_DBG_UB} -DUSE_LOCAL_GOTO=1 -c sgio_simple.c -o sgio_simple_dbg_ub.o
sgio_simple_dbg_ub: sgio_simple_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o sgio_simple_dbg_ub sgio_simple_dbg_ub.o common_dbg_ub.o ${LINKIN}

sgio_simple_dbg_msan.o: sgio_simple.c
	${CL} ${CFLAGS_DBG_MSAN} -DUSE_LOCAL_GOTO=1 -c sgio_simple.c -o sgio_simple_dbg_msan.o
sgio_simple_dbg_msan: sgio_simple_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o sgio_simple_dbg_msan sgio_simple_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: mmap_file_simple
mmap_file_simple.o: mmap_file_simple.c
	${CC} ${CFLAGS} -c mmap_file_simple.c -o mmap_file_simple.o
mmap_file_simple: common.o mmap_file_simple.o
	${CC} -o mmap_file_simple mmap_file_simple.o common.o ${LINKIN}

mmap_file_simple_dbg.o: mmap_file_simple.c
	${CC} ${CFLAGS_DBG} -c mmap_file_simple.c -o mmap_file_simple_dbg.o
mmap_file_simple_dbg: mmap_file_simple_dbg.o common_dbg.o
	${CC} -o mmap_file_simple_dbg mmap_file_simple_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for mmap_file_simple :: (use clang): <foo>_dbg_[asan|ub|msan]
mmap_file_simple_dbg_asan.o: mmap_file_simple.c
	${CL} ${CFLAGS_DBG_ASAN} -c mmap_file_simple.c -o mmap_file_simple_dbg_asan.o
mmap_file_simple_dbg_asan: mmap_file_simple_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o mmap_file_simple_dbg_asan mmap_file_simple_dbg_asan.o common_dbg_asan.o ${LINKIN}

#--- Target :: diskspc
diskspc.o: diskspc.c
	${CC} ${CFLAGS} -c diskspc.c -o diskspc.o
diskspc: common.o diskspc.o
	${CC} -o diskspc diskspc.o common.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

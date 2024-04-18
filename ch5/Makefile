# Makefile, Ch 5
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 5 : Linux Memory Issues
#----------------------------------------------------------------------
# Update(s):
#  We check for clang; if installed, we use it, else we use GCC
#
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
## as desired. We have deliberately not kept the sanitizer targets; add
## them to the 'ALL := ' statement if you wish to build them by default.

ALL := membugs membugs_dbg membugs_dbg_asan membugs_dbg_ub membugs_dbg_msan membugs_dbg_lsan membugs_dbg_tsan

CFLAGS=-O2 -Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address -fsanitize-address-use-after-scope
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory -fPIE -pie
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined
CFLAGS_DBG_LSAN=${CFLAGS_DBG} -fsanitize=leak
CFLAGS_DBG_TSAN=${CFLAGS_DBG} -fsanitize=thread

LINKIN := -static-libasan
 # user will need to explicitly set libraries to link in as required;
 # f.e. -lrt -pthread

all: ${ALL}
# GCC doesn't support MSAN, clang does
CC := gcc
LINKIN := -static-libasan
ifeq (, $(shell which clang))
 $(warning === WARNING! No clang (compiler) in PATH (reqd for MSAN); consider doing 'sudo apt install clang' ===)
else
 CC := clang
 LINKIN := -static-libsan
endif
#---
# I find that, with Valgrind at least, compiling with clang causes Valgrind
# to miss catching several memory bugs - like leakage! With GCC, though, it
# does seem to catch these bugs. So: lets hardcode to use GCC only.
# Of course, do change this (commenting out the below 2 lines) to test with
# clang whenever required.
#---
CC := gcc
LINKIN := -static-libasan
$(info Compiler set to $(CC))

CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o

 #--- Sanitizers (use clang): common_dbg_*
common_dbg_asan.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG_ASAN} -c ../common.c -o common_dbg_asan.o
common_dbg_ub.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG_UB} -c ../common.c -o common_dbg_ub.o
common_dbg_msan.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG_MSAN} -c ../common.c -o common_dbg_msan.o
common_dbg_lsan.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG_LSAN} -c ../common.c -o common_dbg_lsan.o

# ThreadSanitizer (TSan):
# For clang ver < 18.1.0 (Mar '24), need to set vm.mmap_rnd_bits sysctl to 28 (default is 32)
# else it bombs on execution (ref: https://stackoverflow.com/a/77856955/779269)
	sudo sysctl vm.mmap_rnd_bits=28
common_dbg_tsan.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG_TSAN} -c ../common.c -o common_dbg_tsan.o

#--- Target :: membugs
membugs.o: membugs.c
	${CC} ${CFLAGS} -c membugs.c -o membugs.o
membugs: common.o membugs.o
	${CC} -o membugs membugs.o common.o ${LINKIN}

membugs_dbg.o: membugs.c
	${CC} ${CFLAGS_DBG} -c membugs.c -o membugs_dbg.o
membugs_dbg: membugs_dbg.o common_dbg.o
	${CC} -o membugs_dbg membugs_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for membugs :: (use clang): <foo>_dbg_[asan|ub|msan]
membugs_dbg_asan.o: membugs.c
	${CC} ${CFLAGS_DBG_ASAN} -c membugs.c -o membugs_dbg_asan.o
membugs_dbg_asan: membugs_dbg_asan.o common_dbg_asan.o
	${CC} ${CFLAGS_DBG_ASAN} -o membugs_dbg_asan membugs_dbg_asan.o common_dbg_asan.o

membugs_dbg_ub.o: membugs.c
	${CC} ${CFLAGS_DBG_UB} -c membugs.c -o membugs_dbg_ub.o
membugs_dbg_ub: membugs_dbg_ub.o common_dbg_ub.o
	${CC} ${CFLAGS_DBG_UB} -o membugs_dbg_ub membugs_dbg_ub.o common_dbg_ub.o ${LINKIN}

membugs_dbg_msan.o: membugs.c
ifeq (gcc, $(CC))
	$(error === ERROR! No clang (compiler) in PATH (reqd for MSAN); consider doing 'sudo apt install clang' ===)
endif
	${CC} ${CFLAGS_DBG_MSAN} -c membugs.c -o membugs_dbg_msan.o
membugs_dbg_msan: membugs_dbg_msan.o common_dbg_msan.o
ifeq (gcc, $(CC))
	$(error === ERROR! No clang (compiler) in PATH (reqd for MSAN); consider doing 'sudo apt install clang' ===)
endif
	${CC} ${CFLAGS_DBG_MSAN} -o membugs_dbg_msan membugs_dbg_msan.o common_dbg_msan.o

membugs_dbg_lsan.o: membugs.c
	${CC} ${CFLAGS_DBG_LSAN} -c membugs.c -o membugs_dbg_lsan.o
membugs_dbg_lsan: membugs_dbg_lsan.o common_dbg_lsan.o
	${CC} ${CFLAGS_DBG_LSAN} -o membugs_dbg_lsan membugs_dbg_lsan.o common_dbg_lsan.o

membugs_dbg_tsan.o: membugs.c
	${CC} ${CFLAGS_DBG_TSAN} -c membugs.c -o membugs_dbg_tsan.o
membugs_dbg_tsan: membugs_dbg_tsan.o common_dbg_tsan.o
	${CC} ${CFLAGS_DBG_TSAN} -o membugs_dbg_tsan membugs_dbg_tsan.o common_dbg_tsan.o

# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

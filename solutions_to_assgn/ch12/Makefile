# Makefile : solutions_to_assgn/ch12/
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired.
ALL :=  segv_altstack segv_altstack_dbg \
	murder murder_dbg \
	killer killer_dbg \
	sigtmwtinfo sigtmwtinfo_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall -UDEBUG
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

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

segv_altstack.o: segv_altstack.c
	${CC} ${CFLAGS} -c segv_altstack.c -o segv_altstack.o
segv_altstack: common.o segv_altstack.o
	${CC} ${CFLAGS} -o segv_altstack segv_altstack.o common.o

segv_altstack_dbg.o: segv_altstack.c
	${CC} ${CFLAGS_DBG} -c segv_altstack.c -o segv_altstack_dbg.o
segv_altstack_dbg: segv_altstack_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o segv_altstack_dbg segv_altstack_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
segv_altstack_dbg_asan.o: segv_altstack.c
	${CL} ${CFLAGS_DBG_ASAN} -c segv_altstack.c -o segv_altstack_dbg_asan.o
segv_altstack_dbg_asan: segv_altstack_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o segv_altstack_dbg_asan segv_altstack_dbg_asan.o common_dbg_asan.o

segv_altstack_dbg_ub.o: segv_altstack.c
	${CL} ${CFLAGS_DBG_UB} -c segv_altstack.c -o segv_altstack_dbg_ub.o
segv_altstack_dbg_ub: segv_altstack_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o segv_altstack_dbg_ub segv_altstack_dbg_ub.o common_dbg_ub.o

segv_altstack_dbg_msan.o: segv_altstack.c
	${CL} ${CFLAGS_DBG_MSAN} -c segv_altstack.c -o segv_altstack_dbg_msan.o
segv_altstack_dbg_msan: segv_altstack_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o segv_altstack_dbg_msan segv_altstack_dbg_msan.o common_dbg_msan.o

murder.o: murder.c
	${CC} ${CFLAGS} -c murder.c -o murder.o
murder: common.o murder.o
	${CC} ${CFLAGS} -o murder murder.o common.o

murder_dbg.o: murder.c
	${CC} ${CFLAGS_DBG} -c murder.c -o murder_dbg.o
murder_dbg: murder_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o murder_dbg murder_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
murder_dbg_asan.o: murder.c
	${CL} ${CFLAGS_DBG_ASAN} -c murder.c -o murder_dbg_asan.o
murder_dbg_asan: murder_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o murder_dbg_asan murder_dbg_asan.o common_dbg_asan.o

murder_dbg_ub.o: murder.c
	${CL} ${CFLAGS_DBG_UB} -c murder.c -o murder_dbg_ub.o
murder_dbg_ub: murder_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o murder_dbg_ub murder_dbg_ub.o common_dbg_ub.o

murder_dbg_msan.o: murder.c
	${CL} ${CFLAGS_DBG_MSAN} -c murder.c -o murder_dbg_msan.o
murder_dbg_msan: murder_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o murder_dbg_msan murder_dbg_msan.o common_dbg_msan.o

killer.o: killer.c
	${CC} ${CFLAGS} -c killer.c -o killer.o
killer: common.o killer.o
	${CC} ${CFLAGS} -o killer killer.o common.o

killer_dbg.o: killer.c
	${CC} ${CFLAGS_DBG} -c killer.c -o killer_dbg.o
killer_dbg: killer_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o killer_dbg killer_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
killer_dbg_asan.o: killer.c
	${CL} ${CFLAGS_DBG_ASAN} -c killer.c -o killer_dbg_asan.o
killer_dbg_asan: killer_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o killer_dbg_asan killer_dbg_asan.o common_dbg_asan.o

killer_dbg_ub.o: killer.c
	${CL} ${CFLAGS_DBG_UB} -c killer.c -o killer_dbg_ub.o
killer_dbg_ub: killer_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o killer_dbg_ub killer_dbg_ub.o common_dbg_ub.o

killer_dbg_msan.o: killer.c
	${CL} ${CFLAGS_DBG_MSAN} -c killer.c -o killer_dbg_msan.o
killer_dbg_msan: killer_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o killer_dbg_msan killer_dbg_msan.o common_dbg_msan.o

sigtmwtinfo.o: sigtmwtinfo.c
	${CC} ${CFLAGS} -c sigtmwtinfo.c -o sigtmwtinfo.o
sigtmwtinfo: common.o sigtmwtinfo.o
	${CC} ${CFLAGS} -o sigtmwtinfo sigtmwtinfo.o common.o

sigtmwtinfo_dbg.o: sigtmwtinfo.c
	${CC} ${CFLAGS_DBG} -c sigtmwtinfo.c -o sigtmwtinfo_dbg.o
sigtmwtinfo_dbg: sigtmwtinfo_dbg.o common_dbg.o
	${CC} ${CFLAGS_DBG} -o sigtmwtinfo_dbg sigtmwtinfo_dbg.o common_dbg.o

#--- Sanitizers (use clang): <foo>_dbg_[asan|ub|msan]
sigtmwtinfo_dbg_asan.o: sigtmwtinfo.c
	${CL} ${CFLAGS_DBG_ASAN} -c sigtmwtinfo.c -o sigtmwtinfo_dbg_asan.o
sigtmwtinfo_dbg_asan: sigtmwtinfo_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o sigtmwtinfo_dbg_asan sigtmwtinfo_dbg_asan.o common_dbg_asan.o

sigtmwtinfo_dbg_ub.o: sigtmwtinfo.c
	${CL} ${CFLAGS_DBG_UB} -c sigtmwtinfo.c -o sigtmwtinfo_dbg_ub.o
sigtmwtinfo_dbg_ub: sigtmwtinfo_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o sigtmwtinfo_dbg_ub sigtmwtinfo_dbg_ub.o common_dbg_ub.o

sigtmwtinfo_dbg_msan.o: sigtmwtinfo.c
	${CL} ${CFLAGS_DBG_MSAN} -c sigtmwtinfo.c -o sigtmwtinfo_dbg_msan.o
sigtmwtinfo_dbg_msan: sigtmwtinfo_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o sigtmwtinfo_dbg_msan sigtmwtinfo_dbg_msan.o common_dbg_msan.o


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

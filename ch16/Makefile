# Makefile : HOSPL book, ch16/
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
ALL :=  mt_iobuf_mtx mt_iobuf_mtx_dbg \
        mt_iobuf_rfct mt_iobuf_rfct_dbg \
        mt_iobuf_tls mt_iobuf_tls_dbg \
	cancelit cancelit_dbg \
	cleanup_hdlr cleanup_hdlr_dbg \
	tsig tsig_dbg \
	pthreads_app_template pthreads_app_template_dbg

CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-O2 -Wall -UDEBUG -pthread
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DDEBUG -pthread
CFLAGS_DBG_ASAN=${CFLAGS_DBG} -fsanitize=address
CFLAGS_DBG_MSAN=${CFLAGS_DBG} -fsanitize=memory
CFLAGS_DBG_UB=${CFLAGS_DBG} -fsanitize=undefined

LINKIN := -pthread
 # set as required to libraries to link in; f.e.  -lrt

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

#--- Target :: mt_iobuf_mtx
mt_iobuf_mtx.o: mt_iobuf_mtx.c
	${CC} ${CFLAGS} -c mt_iobuf_mtx.c -o mt_iobuf_mtx.o
mt_iobuf_mtx: common.o mt_iobuf_mtx.o
	${CC} -o mt_iobuf_mtx mt_iobuf_mtx.o common.o ${LINKIN}

mt_iobuf_mtx_dbg.o: mt_iobuf_mtx.c
	${CC} ${CFLAGS_DBG} -c mt_iobuf_mtx.c -o mt_iobuf_mtx_dbg.o
mt_iobuf_mtx_dbg: mt_iobuf_mtx_dbg.o common_dbg.o
	${CC} -o mt_iobuf_mtx_dbg mt_iobuf_mtx_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for mt_iobuf_mtx :: (use clang): <foo>_dbg_[asan|ub|msan]
mt_iobuf_mtx_dbg_asan.o: mt_iobuf_mtx.c
	${CL} ${CFLAGS_DBG_ASAN} -c mt_iobuf_mtx.c -o mt_iobuf_mtx_dbg_asan.o
mt_iobuf_mtx_dbg_asan: mt_iobuf_mtx_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o mt_iobuf_mtx_dbg_asan mt_iobuf_mtx_dbg_asan.o common_dbg_asan.o ${LINKIN}

mt_iobuf_mtx_dbg_ub.o: mt_iobuf_mtx.c
	${CL} ${CFLAGS_DBG_UB} -c mt_iobuf_mtx.c -o mt_iobuf_mtx_dbg_ub.o
mt_iobuf_mtx_dbg_ub: mt_iobuf_mtx_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o mt_iobuf_mtx_dbg_ub mt_iobuf_mtx_dbg_ub.o common_dbg_ub.o ${LINKIN}

mt_iobuf_mtx_dbg_msan.o: mt_iobuf_mtx.c
	${CL} ${CFLAGS_DBG_MSAN} -c mt_iobuf_mtx.c -o mt_iobuf_mtx_dbg_msan.o
mt_iobuf_mtx_dbg_msan: mt_iobuf_mtx_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o mt_iobuf_mtx_dbg_msan mt_iobuf_mtx_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: mt_iobuf_rfct
mt_iobuf_rfct.o: mt_iobuf_rfct.c
	${CC} ${CFLAGS} -c mt_iobuf_rfct.c -o mt_iobuf_rfct.o
mt_iobuf_rfct: common.o mt_iobuf_rfct.o
	${CC} -o mt_iobuf_rfct mt_iobuf_rfct.o common.o ${LINKIN}

mt_iobuf_rfct_dbg.o: mt_iobuf_rfct.c
	${CC} ${CFLAGS_DBG} -c mt_iobuf_rfct.c -o mt_iobuf_rfct_dbg.o
mt_iobuf_rfct_dbg: mt_iobuf_rfct_dbg.o common_dbg.o
	${CC} -o mt_iobuf_rfct_dbg mt_iobuf_rfct_dbg.o common_dbg.o ${LINKIN}

mt_iobuf_rfct_dbg_asan.o: mt_iobuf_rfct.c
	${CL} ${CFLAGS_DBG_ASAN} -c mt_iobuf_rfct.c -o mt_iobuf_rfct_dbg_asan.o
mt_iobuf_rfct_dbg_asan: mt_iobuf_rfct_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o mt_iobuf_rfct_dbg_asan mt_iobuf_rfct_dbg_asan.o common_dbg_asan.o ${LINKIN}

mt_iobuf_rfct_dbg_msan.o: mt_iobuf_rfct.c
	${CL} ${CFLAGS_DBG_MSAN} -c mt_iobuf_rfct.c -o mt_iobuf_rfct_dbg_msan.o
mt_iobuf_rfct_dbg_msan: mt_iobuf_rfct_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o mt_iobuf_rfct_dbg_msan mt_iobuf_rfct_dbg_msan.o common_dbg_msan.o ${LINKIN}

#--- Target :: mt_iobuf_tls
mt_iobuf_tls.o: mt_iobuf_tls.c
	${CC} ${CFLAGS} -c mt_iobuf_tls.c -o mt_iobuf_tls.o
mt_iobuf_tls: common.o mt_iobuf_tls.o
	${CC} -o mt_iobuf_tls mt_iobuf_tls.o common.o ${LINKIN}

mt_iobuf_tls_dbg.o: mt_iobuf_tls.c
	${CC} ${CFLAGS_DBG} -c mt_iobuf_tls.c -o mt_iobuf_tls_dbg.o
mt_iobuf_tls_dbg: mt_iobuf_tls_dbg.o common_dbg.o
	${CC} -o mt_iobuf_tls_dbg mt_iobuf_tls_dbg.o common_dbg.o ${LINKIN}

#--- Target :: cancelit
cancelit.o: cancelit.c
	${CC} ${CFLAGS} -c cancelit.c -o cancelit.o
cancelit: common.o cancelit.o
	${CC} -o cancelit cancelit.o common.o ${LINKIN}

cancelit_dbg.o: cancelit.c
	${CC} ${CFLAGS_DBG} -c cancelit.c -o cancelit_dbg.o
cancelit_dbg: cancelit_dbg.o common_dbg.o
	${CC} -o cancelit_dbg cancelit_dbg.o common_dbg.o ${LINKIN}

#--- Target :: cleanup_hdlr
cleanup_hdlr.o: cleanup_hdlr.c
	${CC} ${CFLAGS} -c cleanup_hdlr.c -o cleanup_hdlr.o
cleanup_hdlr: common.o cleanup_hdlr.o
	${CC} -o cleanup_hdlr cleanup_hdlr.o common.o ${LINKIN}

cleanup_hdlr_dbg.o: cleanup_hdlr.c
	${CC} ${CFLAGS_DBG} -c cleanup_hdlr.c -o cleanup_hdlr_dbg.o
cleanup_hdlr_dbg: cleanup_hdlr_dbg.o common_dbg.o
	${CC} -o cleanup_hdlr_dbg cleanup_hdlr_dbg.o common_dbg.o ${LINKIN}

#--- Target :: tsig
tsig.o: tsig.c
	${CC} ${CFLAGS} -c tsig.c -o tsig.o
tsig: common.o tsig.o
	${CC} -o tsig tsig.o common.o ${LINKIN}

tsig_dbg.o: tsig.c
	${CC} ${CFLAGS_DBG} -c tsig.c -o tsig_dbg.o
tsig_dbg: tsig_dbg.o common_dbg.o
	${CC} -o tsig_dbg tsig_dbg.o common_dbg.o ${LINKIN}

#--- Target :: pthreads_app_template
pthreads_app_template.o: pthreads_app_template.c
	${CC} ${CFLAGS} -c pthreads_app_template.c -o pthreads_app_template.o
pthreads_app_template: common.o pthreads_app_template.o
	${CC} -o pthreads_app_template pthreads_app_template.o common.o ${LINKIN}

pthreads_app_template_dbg.o: pthreads_app_template.c
	${CC} ${CFLAGS_DBG} -c pthreads_app_template.c -o pthreads_app_template_dbg.o
pthreads_app_template_dbg: pthreads_app_template_dbg.o common_dbg.o
	${CC} -o pthreads_app_template_dbg pthreads_app_template_dbg.o common_dbg.o ${LINKIN}

 #--- Sanitizers for pthreads_app_template :: (use clang): <foo>_dbg_[asan|ub|msan]
pthreads_app_template_dbg_asan.o: pthreads_app_template.c
	${CL} ${CFLAGS_DBG_ASAN} -c pthreads_app_template.c -o pthreads_app_template_dbg_asan.o
pthreads_app_template_dbg_asan: pthreads_app_template_dbg_asan.o common_dbg_asan.o
	${CL} ${CFLAGS_DBG_ASAN} -o pthreads_app_template_dbg_asan pthreads_app_template_dbg_asan.o common_dbg_asan.o ${LINKIN}

pthreads_app_template_dbg_ub.o: pthreads_app_template.c
	${CL} ${CFLAGS_DBG_UB} -c pthreads_app_template.c -o pthreads_app_template_dbg_ub.o
pthreads_app_template_dbg_ub: pthreads_app_template_dbg_ub.o common_dbg_ub.o
	${CL} ${CFLAGS_DBG_UB} -o pthreads_app_template_dbg_ub pthreads_app_template_dbg_ub.o common_dbg_ub.o ${LINKIN}

pthreads_app_template_dbg_msan.o: pthreads_app_template.c
	${CL} ${CFLAGS_DBG_MSAN} -c pthreads_app_template.c -o pthreads_app_template_dbg_msan.o
pthreads_app_template_dbg_msan: pthreads_app_template_dbg_msan.o common_dbg_msan.o
	${CL} ${CFLAGS_DBG_MSAN} -o pthreads_app_template_dbg_msan pthreads_app_template_dbg_msan.o common_dbg_msan.o ${LINKIN}


# indent- "beautifies" C code into the "Linux kernel style".
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

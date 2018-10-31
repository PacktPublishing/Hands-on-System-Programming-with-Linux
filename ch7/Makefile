# Makefile, Ch 7
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 7 : Process Credentials
#----------------------------------------------------------------------
## Pl check and keep or remove <foo>_dbg_[asan|ub|msan] targets
## as desired. We have deliberately not kept the sanitizer targets; add
## them to the 'ALL := ' statement if you wish to build them by default.
CC=${CROSS_COMPILE}gcc
CL=${CROSS_COMPILE}clang

CFLAGS=-Wall
CFLAGS_DBG=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra
CFLAGS_DBG_ASAN= -fsanitize=address
CFLAGS_DBG_MSAN= -fsanitize=memory -fPIE -pie
CFLAGS_DBG_UB= -fsanitize=undefined

ALL := hello_pause hello_pause_dbg \
	query_creds query_creds_dbg \
	savedset_demo savedset_demo_dbg \
	rootsh_hack2 rootsh_hack2_dbg

all: ${ALL}
CB_FILES := *.[ch]

common.o: ../common.c ../common.h
	${CC} ${CFLAGS} -c ../common.c -o common.o
common_dbg.o: ../common.c ../common.h
	${CC} ${CFLAGS_DBG} -c ../common.c -o common_dbg.o

hello_pause: common.o hello_pause.o
	${CC} ${CFLAGS} -o hello_pause hello_pause.c common.o
	#sudo setcap cap_net_admin,cap_net_raw+ep ./hello_pause
hello_pause_dbg.o: hello_pause.c
	${CC} ${CFLAGS_DBG} -c hello_pause.c -o hello_pause_dbg.o
hello_pause_dbg: hello_pause_dbg.o common_dbg.o
	${CC} -o hello_pause_dbg hello_pause_dbg.o common_dbg.o
	#sudo setcap cap_net_admin,cap_net_raw+ep ./hello_pause_dbg

query_creds: common.o query_creds.o
	${CC} ${CFLAGS} -o query_creds query_creds.c common.o
query_creds_dbg.o: query_creds.c
	${CC} ${CFLAGS_DBG} -c query_creds.c -o query_creds_dbg.o
query_creds_dbg: query_creds_dbg.o common_dbg.o
	${CC} -o query_creds_dbg query_creds_dbg.o common_dbg.o

rootsh_hack2: common.o rootsh_hack2.o
	${CC} ${CFLAGS} -o rootsh_hack2 rootsh_hack2.c common.o
rootsh_hack2_dbg.o: rootsh_hack2.c
	${CC} ${CFLAGS_DBG} -c rootsh_hack2.c -o rootsh_hack2_dbg.o
rootsh_hack2_dbg: rootsh_hack2_dbg.o common_dbg.o
	${CC} -o rootsh_hack2_dbg rootsh_hack2_dbg.o common_dbg.o

savedset_demo: common.o savedset_demo.o
	${CC} ${CFLAGS} -o savedset_demo savedset_demo.c common.o
	#sudo chown root savedset_demo
	#sudo chmod u+s savedset_demo
savedset_demo_dbg.o: savedset_demo.c
	${CC} ${CFLAGS_DBG} -c savedset_demo.c -o savedset_demo_dbg.o
savedset_demo_dbg: savedset_demo_dbg.o common_dbg.o
	${CC} -o savedset_demo_dbg savedset_demo_dbg.o common_dbg.o
	#sudo chown root savedset_demo_dbg
	#sudo chmod u+s savedset_demo_dbg

query_pcap: common.o query_pcap.o
	${CC} ${CFLAGS} -o query_pcap query_pcap.c common.o -lcap
query_pcap_dbg.o: query_pcap.c
	${CC} ${CFLAGS_DBG} -c query_pcap.c -o query_pcap_dbg.o
query_pcap_dbg: query_pcap_dbg.o common_dbg.o
	${CC} -o query_pcap_dbg query_pcap_dbg.o common_dbg.o -lcap

set_pcap: common.o set_pcap.o
	${CC} ${CFLAGS} -o set_pcap set_pcap.c common.o -lcap
	sudo setcap cap_setuid,cap_sys_admin+ep ./set_pcap
set_pcap_dbg.o: set_pcap.c
	${CC} ${CFLAGS_DBG} -c set_pcap.c -o set_pcap_dbg.o
set_pcap_dbg: set_pcap_dbg.o common_dbg.o
	${CC} -o set_pcap_dbg set_pcap_dbg.o common_dbg.o -lcap
	sudo setcap cap_setuid,cap_sys_admin+ep ./set_pcap_dbg


# indent- "beautifies" C code into the "Linux kernel style"\.
# (cb = C Beautifier :) )
# Note! original source file(s) is overwritten, so we back it up.
cb: ${CB_FILES}
	mkdir bkp 2> /dev/null; cp -f ${CB_FILES} bkp/
	indent -linux ${CB_FILES}

clean:
	rm -vf ${ALL} core* vgcore* *.o *~

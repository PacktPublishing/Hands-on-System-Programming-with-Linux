#!/bin/bash
# A_iobuf/perf_runs.sh
#
# This is part of the source code released for the book
#  "Hands-On System Programming with Linux", by Kaiwan N Billimoria
#  Packt Publishers
#  For Ch A : File IO Part I - The Essentials
#
# NOTE! This script is provided "as-is" as a small value-add to the book;
# the code is lightly tested and is for the reader to try...
#
# Profile the running of the IO buffer demo apps:
#  a) with the 'plain vanilla' (default) IO buffer mode/size 
#  b) with our own larger IO buffer mode/size
# The sampling and profiling is done using 'perf'!
#
# For details, please refer the book, Ch A, File I/O Essentials.

# runcmd
# Display and run the provided command.
# Parameter 1 : the command to run
runcmd()
{
SEP="------------------------------"
[ $# -eq 0 ] && return
echo "${SEP}
$@
${SEP}"
eval $@
}


## 'main' ---
PERF_REC_ARGS="-F4096 --realtime=99 --call-graph dwarf -s -T"
 # collect samples @ hi-freq (4 Khz), SCHED_FIFO prio 99, gen stack traces,
 # -s : per-thread counts, -T : sample timestamps (see with perf script)
PERF_DAT_1=/tmp/perf.data.defaultbuf
PERF_DAT_2=/tmp/perf.data.largebuf
PERF_DAT_3=/tmp/perf.data.smallbuf

DO_PERF_RECORD_REPORT=0

sync

[ ! -f iobuf_dbg ] && {
	echo "Require iobuf_dbg to be built."
	exit 1
}
[ ! -f io_setbuf_dbg ] && {
	echo "Require io_setbuf_dbg to be built."
	exit 1
}

# Syntax
# $ ./iobuf_dbg 
# Usage: ./iobuf_dbg number-of-times-to-rdwr
# $ 
# $ ./io_setbuf_dbg 
# Usage: ./io_setbuf_dbg new-iobuf-size number-of-times-to-rdwr
# $ 

# Case 1 : 'plain vanilla' default IO buffer size
runcmd "perf stat ./iobuf_dbg 10000"
[ ${DO_PERF_RECORD_REPORT} -eq 1 ] && {
 runcmd "sudo perf record ${PERF_REC_ARGS} --output=${PERF_DAT_1} ./iobuf_dbg 10000"
 runcmd "sudo perf report --input ${PERF_DAT_1} --sort=comm,dso #--stdio"
}

sync
sleep 1

# Case 2 : our own IO buffer, large size
runcmd "perf stat ./io_setbuf_dbg 512000 10000"
[ ${DO_PERF_RECORD_REPORT} -eq 1 ] && {
 runcmd "sudo perf record ${PERF_REC_ARGS} --output=${PERF_DAT_2} ./io_setbuf_dbg 512000 10000"
 runcmd "sudo perf report --input ${PERF_DAT_2} --sort=comm,dso #--stdio"
}

# Case 3 : our own IO buffer, small size
runcmd "perf stat ./io_setbuf_dbg 128 10000"
[ ${DO_PERF_RECORD_REPORT} -eq 1 ] && {
 runcmd "sudo perf record ${PERF_REC_ARGS} --output=${PERF_DAT_3} ./io_setbuf_dbg 128 10000"
 runcmd "sudo perf report --input ${PERF_DAT_3} --sort=comm,dso #--stdio"
}

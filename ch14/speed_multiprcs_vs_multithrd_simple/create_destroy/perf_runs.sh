#!/bin/bash
# ch14/speed_multiprcs_vs_multithrd_simple/create_destroy/perf_runs.sh
#
# A simple wrapper over 'perf record' and 'perf report'.
# NOTE! This script is provided "as-is" as a small value-add to the book;
# the code is lightly tested and is for the reader to try...

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

which perf >/dev/null || {
	echo "${name}: 'perf' not installed? Please install and retry..."
	exit 1
}

PERF_REC_ARGS="-F4096 --realtime=99 --call-graph dwarf -s -T"
 # collect samples @ hi-freq (4 Khz), SCHED_FIFO prio 99, gen stack traces,
 # -s : per-thread counts, -T : sample timestamps (see with perf script)

# PUT - Program Under Test
PUT1="./fork_test"
PUT2="./pthread_test"
PERF_DAT_1=/tmp/perf.data.PUT1
PERF_DAT_2=/tmp/perf.data.PUT2

DO_PERF_RECORD_REPORT=1

sync

[ ! -f ${PUT1} ] && {
	echo "Require PUT1 \"${PUT1}\" to be built."
	exit 1
}
[ ! -f ${PUT1} ] && {
	echo "Require PUT2 \"${PUT2}\" to be built."
	exit 1
}

# PUT 1
#runcmd "perf stat ./iobuf_dbg 10000"
[ ${DO_PERF_RECORD_REPORT} -eq 1 ] && {
 runcmd "sudo perf record ${PERF_REC_ARGS} --output=${PERF_DAT_1} ${PUT1}"
 runcmd "sudo perf report --input ${PERF_DAT_1} --sort=comm,dso #--stdio"
}

sync
sleep 1

# PUT 2
#runcmd "perf stat ./io_setbuf_dbg 512000 10000"
[ ${DO_PERF_RECORD_REPORT} -eq 1 ] && {
 runcmd "sudo perf record ${PERF_REC_ARGS} --output=${PERF_DAT_2} ${PUT2}"
 runcmd "sudo perf report --input ${PERF_DAT_2} --sort=comm,dso #--stdio"
}

exit 0

#!/bin/bash
# solutions_to_assgn/ch12/bombard_sig.sh
#
# Shell script to bombard a given process with signals.
# Really a test case for the 'C' signal handling programs..
name=$(basename $0)
SIG=SIGUSR1

if [ $# -ne 2 ] ; then
	echo "Usage: ${name} PID-of-process num-signals-to-send
 (-1 to continously bombard the process with signals)."
	exit 1
fi
kill -0 $1 2>/dev/null || {
	echo "${name}: PID $1 invalid (or dead, or perm issue)?"
	exit 1
}

NUMSIGS=$2
n=1
if [ ${NUMSIGS} -eq -1 ] ; then
	echo "Sending signal ${SIG} continually to process ${1} ..."
	while [ true ] ; do
		kill -${SIG} $1
		sleep 10e-03 # 10 ms
	done
else
	echo "Sending ${NUMSIGS} instances of signal ${SIG} to process ${1} ..."
	while [ ${n} -le ${NUMSIGS} ] ; do
		kill -${SIG} $1 || {
			echo "${name}: kill failed, loop count=${n}"
			exit 1
		}
		sleep 10e-03 # 10 ms
		let n=n+1
	done
fi
exit 0

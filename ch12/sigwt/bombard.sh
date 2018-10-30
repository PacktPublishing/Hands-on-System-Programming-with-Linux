#!/bin/bash
# ch13/sigwt/bombard.sh
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 13 : Signaling Part II
#----------------------------------------------------------------------
# Brief Description:
# Shell script to bombard a given process with All signals.
# Really a test case for the 'C' signal handling programs..
#
# For details, please refer the book, Ch 13.
name=$(basename $0)

# Parameters: 
# $1 : signal to deliver
# $2 : PID to deliver it to
sendsig()
{
 kill -${1} $2 || {
	echo "${name}: \"kill -$1 $2\" failed, loop count=${n}"
	exit 1
 }
 echo -n "$1 "
}

# "main" here

if [ $# -ne 2 ] ; then
	echo "Usage: ${name} PID-of-process num-batches-to-send(of all sigs)
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
	echo "Sending ALL signals continually to process ${1} ..."
	while [ true ] ; do
		for SIG in 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 17 18 20 21 22 23 24 25 26 27 28 29 30 31 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64
		do
			sendsig ${SIG} $1
			sleep 10e-03 # 10 ms
		done
		let n=n+1
	done
else
	echo "Sending ${NUMSIGS} instances each of ALL signals to process ${1}" 
	while [ ${n} -le ${NUMSIGS} ] ; do
		for SIG in 1 2 3 4 5 6 7 8 10 11 12 13 14 15 16 17 18 20 21 22 23 24 25 26 27 28 29 30 31 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64
		do
			sendsig ${SIG} $1
			sleep 10e-03 # 10 ms
		done
		let n=n+1
	done
	echo
fi
exit 0

#!/bin/bash
# ch12/bombard_sigrt.sh
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From:  Ch 12 : Signaling Part II
#----------------------------------------------------------------------
# Shell script to bombard a given process with some specific RT signals.
# Really a test case for the 'C' signal handling programs..
# For details, please refer the book, Ch 12.
name=$(basename $0)
SIG1=SIGRTMAX-5   # sig# 59
SIG2=SIGRTMAX     # sig# 64
SIG3=SIGRTMIN+5   # sig# 39

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
	echo "Usage: ${name} PID-of-process num-RT-signals-batches-to-send
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
	echo "Sending RT signal batch {${SIG1}, ${SIG2}, ${SIG3}} continually to process ${1} ..."
	echo " i.e. signal #s {$(kill -l ${SIG1}), $(kill -l ${SIG2}), $(kill -l ${SIG3})}"
	while [ true ] ; do
		sendsig ${SIG1} $1
		sleep 10e-03 # 10 ms
		sendsig ${SIG2} $1
		sleep 10e-03 # 10 ms
		sendsig ${SIG3} $1
		sleep 10e-03 # 10 ms
	done
else
	echo "Sending ${NUMSIGS} instances each of RT signal batch" 
	echo " {${SIG1}, ${SIG2}, ${SIG3}} to process ${1} ..."
	echo " i.e. signal #s {$(kill -l ${SIG1}), $(kill -l ${SIG2}), $(kill -l ${SIG3})}"
	while [ ${n} -le ${NUMSIGS} ] ; do
		sendsig ${SIG1} $1
		sleep 10e-03 # 10 ms
		sendsig ${SIG2} $1
		sleep 10e-03 # 10 ms
		sendsig ${SIG3} $1
		sleep 10e-03 # 10 ms
		let n=n+1
	done
	echo
fi
exit 0

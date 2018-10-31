#!/bin/bash
# A_iobuf/trc_iobuf.sh
#
# This is part of the source code released for the book
#  "Hands-On System Programming with Linux", by Kaiwan N Billimoria
#  Packt Publishers
#  For Ch A : File IO Part I - The Essentials
#
# Simple wrapper script over the 'simpcp?.c' utils to test-run them.
#
# Trace the running of the IO buffer demo apps:
#  a) with the 'plain vanilla' (default) IO buffer mode/size 
#  b) with our own larger IO buffer mode/size
# The tracing is done using 'ltrace'.
#
# For details, please refer the book, Ch A, File I/O Essentials.
TRCFILE1=ltrc_vanilla.txt
TRCFILE2=ltrc_setvbuf_large.txt
TRCFILE3=ltrc_setvbuf_small.txt

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

# show_stat()
# Parameter 1 : regex- API to show/count
# Parameter 2 : 'English' name of above
# Parameter 3 : file to grep
show_stat()
{
[ $# -eq 0 ] && return
num=$(grep -w "$1" $3 |wc -l)
printf "# calls to %10s   : %6d\n" $2 ${num}
}

ltrace_stats()
{
[ $# -eq 0 ] && return
echo "------------ ltrace stats -----------------"
show_stat "^fread" fread ${1}
show_stat "^read" read ${1}
show_stat "^fwrite" fwrite ${1}
show_stat "^write" write ${1}
}

## 'main' here
[ ! -f iobuf ] && {
	echo "Require iobuf to be built."
	exit 1
}
[ ! -f io_setbuf ] && {
	echo "Require io_setbuf to be built."
	exit 1
}

sync
# Case 1 : plain vanilla stdio buffering
runcmd "ltrace -S ./iobuf 10000 2>${TRCFILE1}"
ltrace_stats ${TRCFILE1}

sync
sleep 1

# Case 2 : programmer-settable stdio buffering : large IO buffers
runcmd "ltrace -S ./io_setbuf 512000 10000 2>${TRCFILE2}"
ltrace_stats ${TRCFILE2}

# Case 3 : programmer-settable stdio buffering : small IO buffers
runcmd "ltrace -S ./io_setbuf 128 10000 2>${TRCFILE3}"
ltrace_stats ${TRCFILE3}

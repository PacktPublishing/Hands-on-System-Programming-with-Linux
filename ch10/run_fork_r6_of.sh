#!/bin/bash
#--------------------------------------------------------------
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From: Ch 11 : Process Creation
#--------------------------------------------------------------
# run_fork_r6_of.sh : simple wrapper over fork_r6_of
# For details, please refer the book, Ch 11.
FILE=tst
rm -f fork_r6_of
make fork_r6_of_dbg || exit 1
./fork_r6_of_dbg ${FILE}

# valgrind ./fork_r6_of_dbg ${FILE}
#   -to check with valgrind

ls -l ${FILE}
echo "Examine the file '${FILE}'"

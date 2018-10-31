#!/bin/bash
# ch8/show_caps.sh
#***************************************************************
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From:  Ch 8 : Process Capabilities
#****************************************************************
# Brief Description:
# This bash script scans the system (some dirs) for all installed 
# 'modern' POSIX capabilities-embedded programs.
#
# For details, please refer the book, Ch 8.
name=$(basename $0)
SEP="------------------------------------------------------------------"
declare -a gDirArr=('/bin' '/usr/bin' '/sbin' '/usr/sbin' '/usr/local/bin' '/usr/local/sbin');

########### Functions follow #######################

scanforcaps()
{
[ ! -d $1 ] && return 1
for fname in /$1/*
do
  getcap ${fname}
done
}

show_files_with_caps()
{
 echo "Scanning various folders for binaries with (modern) 'capabilities' embedded ..."
 echo "${SEP}"

 for ((i=0; i<${#gDirArr[@]}; i++))
 do
	dir=${gDirArr[${i}]}
	printf "[+] Scanning %-15s ...\n" "${dir}"
	scanforcaps ${dir}
 done
}

sysinfo()
{
echo "${SEP}"
echo "System Information (LSB):"
echo "${SEP}"
lsb_release -a
echo -n "kernel: "
uname -r
echo "${SEP}"
}

##### "main" : execution starts here #####

sysinfo
show_files_with_caps
exit 0

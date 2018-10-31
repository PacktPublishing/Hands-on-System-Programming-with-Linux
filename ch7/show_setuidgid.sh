#!/bin/bash
# ch7/show_setuidgid.sh
#***************************************************************
# This program is part of the source code released for the book
#  "Hands-on System Programming with Linux"
#  (c) Author: Kaiwan N Billimoria
#  Publisher:  Packt
#
# From:   Ch 7 : Process Credentials
#****************************************************************
# Brief Description:
# This bash script scans the system for all installed traditional
# setuid-root and setgid programs.
#
# For details, please refer the book, Ch 7.
name=$(basename $0)
SEP="------------------------------------------------------------------"
declare -a gDirArr=('/bin' '/usr/bin' '/sbin' '/usr/sbin' '/usr/local/bin' '/usr/local/sbin');

########### Functions follow #######################

show_traditional_setgid_prg()
{
 echo "Scanning various directories for (traditional) SETGID binaries ..."
 echo "${SEP}"

 for ((i=0; i<${#gDirArr[@]}; i++))
 do
	dir=${gDirArr[${i}]}
	printf "[+] Scanning %-15s ...\n" "${dir}"
	ls -l ${dir} | grep "^-.....s" #| awk '$3=="root" {print $0}'
	echo "${SEP}"
 done
}

show_traditional_setuid_root_prg()
{
 echo "Scanning various directories for (traditional) SETUID-ROOT binaries ..."
 echo "${SEP}"

 for ((i=0; i<${#gDirArr[@]}; i++))
 do
	dir=${gDirArr[${i}]}
	printf "[+] Scanning %-15s ...\n" "${dir}"
	ls -l ${dir} | grep "^-..s" | awk '$3=="root" {print $0}'
	echo "${SEP}"
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
show_traditional_setuid_root_prg
echo
show_traditional_setgid_prg
exit 0

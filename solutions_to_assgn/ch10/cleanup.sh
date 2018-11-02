#!/bin/bash
# Quick cleanup script; useful for ch10:assignments Q5 and Q6 - the 'forkbomb's!
alive_before=$(ps -le|grep forkbomb|wc -l)
[ ${alive_before} -eq 0 ] && exit 0
echo "forkbomb: ${alive_before} processes alive, killing them off now ..."

for p in $(ps -A -To pid,comm | grep forkbomb |awk '{print $1}')
do
	echo -n "Killing $p ... "
	kill -9 $p || echo "failed" && echo "[ok]"
done
ps -le|grep forkbomb|wc -l
exit 0

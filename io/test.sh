#!/bin/bash

envoke()
{
	echo "$@"
	$@
}

run()
{
	for i in 512 1024 2048 3584 4096 8192 16384 32768 65536; 
	do
		dd if=/dev/zero of=.data count=1 bs=$i
		time envoke ./o_direct_test .data $i
	done
}

run

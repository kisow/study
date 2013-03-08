#!/bin/bash

envoke()
{
	echo "$@"
	$@
}

for i in 512 1024 2048 3584 4096 8192 16384 32768 65536; 
do
	nfpdrop data
	#nfpmon data
	time envoke ./test_read data $i
done

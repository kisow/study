#!/bin/bash

function cl()
{
	local LD_LIBRARY_PATH=/usr/local/packages/gcc-4.6.3/usr/local/lib64:/usr/local/packages/binutils-2.22_/lib:$LD_LIBRARY_PATH 
	local CPATH=/usr/local/include/boost:/usr/local/include/boost/compatibility/cpp_c_headers:/usr/local/packages/gcc-4.6.3/usr/local/include
	local PATH=/usr/local/packages/binutils-2.22_/bin:$PATH
	LD_LIBRARY_PATH=$LD_LIBRARY_PATH CPATH=$CPATH PATH=$PATH clang++ \
	-Wl,-L/usr/local/packages/gcc-4.6.3/usr/local/lib64 \
	-Wl,-L/usr/local/packages/gcc-4.6.3/usr/local/lib/gcc/x86_64-unknown-linux-gnu/4.6.3 \
	"$@"
}

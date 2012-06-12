#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "x86simd.hpp"

using namespace std;

typedef char v8c __attribute__ ((vector_size(8)));
typedef char v16c __attribute__ ((vector_size(16)));
//typedef char v32c __attribute__ ((vector_size(32)));

union v16
{
	v16c x;
	char  c[16];
	int   i[4];

	v16() {}
	v16(const v16c& src) { *this = src; }
	const v16& operator=(const v16c& src) { x = src; return *this; }

	operator v16c&() { return x; }
	operator const v16c&() const { return x; }
	char& operator[](size_t idx) { return c[idx]; }
	char operator[](size_t idx) const { return c[idx]; }
};

//static inline v16c pshufb(v16c buf, v16c mask)
//{
//	__asm__ __volatile__ ("pshufb %1, %0" : "+x" (buf) : "xm" (mask));
//
//	return buf;
//}

int main()
{
	v16 a;
	v16 mask;
	v16 res;
   
	for(size_t i = 0; i < 16; i++) {
		a[i] = (char)i;
		mask[i] = (char)(15 - i);
	}

	int b;
	int c;

	res	= pshufb(a.x, mask.x); 
//	res	= __builtin_ia32_pshufb128(a, mask); 

	cout << sizeof(v16) << endl;
	for(size_t i = 0; i < (sizeof(v16)); i++) {
		cout << (int)res[i] << ' ';
	}
	cout << endl;
	for(size_t i = 0; i < (sizeof(v16) / sizeof(int)); i++) {
		cout << (int)res.i[i] << ' ';
	}
	cout << endl;

	return 0;
}


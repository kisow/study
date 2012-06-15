#include <stdint.h>
#include <iostream>
#include <stdio.h>
#include "x86simd.hpp"

using namespace std;
using namespace x86simd;

template <typename Type>
struct UniqueNumber
{
	Type v_;
	UniqueNumber(Type v = 0) : v_(v) {}
	Type operator()() { return ++v_; }
};

template <typename Type>
void print(const M128& value) 
{
	for(const Type* p = value.cbegin<Type>(); p != value.cend<Type>(); p++) {
		cout << int64_t(*p) << ' ';
	}
	cout << endl;
}

int main()
{
	M128	a;
	M128	mask;
	M128	res;
   
	generate(a.begin<int>(), a.end<int>(), UniqueNumber<int>());
	print<int>(a);

	generate(mask.begin<char>(), mask.end<char>(), UniqueNumber<char>(-2));
	print<char>(mask);

	res	= pshufb(a, mask); 
	print<int>(res);

	return 0;
}


#include <string>
#include <iostream>
#include "x86simd.hpp"

using namespace std;
using namespace x86simd;

int main()
{
	Cpuid x;

	cout.setf(ios_base::boolalpha);
	cout << "vendor : " << x.getVendorString() << endl;
	cout << "mmx : " << x.mmx() << endl;
	cout << "sse : " << x.sse() << endl;
	cout << "sse2 : " << x.sse2() << endl;
	cout << "htt : " << x.htt() << endl;
	cout << "sse3 : " << x.sse3() << endl;
	cout << "ssse3 : " << x.ssse3() << endl;
	cout << "3dnow : " << x.amd3dnow() << endl;
	cout << "3dnow2 : " << x.amd3dnow2() << endl;

	return 0;
}


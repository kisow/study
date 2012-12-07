#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <bitset>
#include <string>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "x86simd.hpp"

using namespace std;
using namespace x86simd;
using namespace boost::assign;

template <typename Type>
struct UniqueNumber
{
	Type v_;
	UniqueNumber(Type v = 0) : v_(v) {}
	Type operator()() { return ++v_; }
};

template <typename Type>
void print(ostream& out, const M128& value) 
{
	for(const Type* p = value.cbegin<Type>(); p != value.cend<Type>(); p++) {
		out << int64_t(*p) << ' ';
	}
	out << endl;
}

class X86SimdTestSuite : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(X86SimdTestSuite);
	CPPUNIT_TEST(testCpuid);
	CPPUNIT_TEST(testPSHUFB);
	CPPUNIT_TEST_SUITE_END();

public:
	void testCpuid()
	{
		Cpuid x;

		cout.setf(ios_base::boolalpha);
		cout << endl;
		cout << "vendor : " << x.getVendorString() << endl;
		cout << "mmx : " << x.mmx() << endl;
		cout << "sse : " << x.sse() << endl;
		cout << "sse2 : " << x.sse2() << endl;
		cout << "htt : " << x.htt() << endl;
		cout << "sse3 : " << x.sse3() << endl;
		cout << "ssse3 : " << x.ssse3() << endl;
		cout << "3dnow : " << x.amd3dnow() << endl;
		cout << "3dnow2 : " << x.amd3dnow2() << endl;
		cout << endl;
	}

	void testPSHUFB()
	{
		M128	src;
		M128	mask;

		cout << endl;
		generate(src.begin<char>(), src.end<char>(), UniqueNumber<char>());
		print<char>(cout, src);

		generate(mask.begin<char>(), mask.end<char>(), UniqueNumber<char>(-1));
		sort(mask.begin<char>(), mask.end<char>(), greater<char>());
		print<char>(cout, mask);

		pshufb(src, mask); 
		print<char>(cout, src);

		vector<uint8_t> v;
		vector<int8_t> m;
	}
};

CPPUNIT_TEST_SUITE_REGISTRATION(X86SimdTestSuite);

// vim:sw=4 ts=4 smarttab autoindent

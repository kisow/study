#include <iostream>
#include <memory>

namespace sigma_base
{
#include "args_to_string.hpp"
};

using namespace std;

struct Base
{
	virtual void test() = 0;
};

struct Derived : public Base
{
	virtual void test() {}
};

#define F(...) {    \
	cout << '(' << #__VA_ARGS__ << ") = (" << sigma_base::args::toString(__VA_ARGS__) << ')' << endl; \
}

int main()
{
	void (*f1_call)();

	char c = 'a';
	unsigned char uc = 'A';
	char array[3] = "ar";

	int8_t  i8 = -8;
	int16_t i16= -16;
	int32_t i32 = -32;
	int64_t i64 = -64;
	long        l = -100;
	long long   ll = -1000;

	uint8_t     ui8 = 8;
	uint16_t    ui16= 16;
	uint32_t    ui32 = 32;
	uint64_t    ui64 = 64;
	unsigned long       ul = 100;
	unsigned long long  ull = 1000;

	float   f32 = 32.0;
	double  d64 = 64.0;

	struct FunctionNested {} fn;
	void *voidPtr = &ull;
	void *& voidPtrRef = voidPtr;
	Base *basePtr = new Derived();
	std::shared_ptr<Base> baseSharedPtr(new Derived());

	F();
	F(c, uc, 'y', "YYY", array, string("std::string"));
	F(i8, i16, i32, i64, -1, -10L, -100LL);
	F(ui8, ui16, ui32, ui64, 1, 10UL, 100ULL);
	F(l, ll, ul, ull, f1_call);
	F(f32, d64, fn, voidPtr, voidPtrRef, basePtr, baseSharedPtr);

	return 0;
}

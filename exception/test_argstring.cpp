#include <iostream>
#include <memory>

#include "cubs_string.hpp"

using namespace std;
using namespace cubs;

struct Base
{
	virtual void test() = 0;
};

struct Derived : public Base
{
	virtual void test() {}
};

#define F(...) {    \
	cout << '(' << #__VA_ARGS__ << ") = (" << cubs::toString(__VA_ARGS__) << ')' << endl; \
}

enum ani_oooml
{
	stop = 0,
	cut = 1,
};

inline void fromString(const string& str, ani_oooml& value) 
{ 
	if(str.compare("cut") == 0) value = cut;
	else value = stop;
}

inline string toString(ani_oooml value) 
{ 
	switch(value)
	{
		case cut: return "cut";
		case stop: 
		default:
				  return "stop";
	}
}

template<typename T>
T myFromString(const string& str)
{
	return fromString(str);
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

	i32 = fromString("-10");
	cout << i32 << endl;
	ui32 = fromString("10");
	cout << ui32 << endl;
	f32 = fromString("10");
	cout << f32 << endl;

	ani_oooml oooml = fromString("cut");
	cout << toString(oooml) << endl;

	string str = fromString("str");		// copy constructor
	str = fromString("str");			// copy operator
	cout << str << endl;

	string b = fromString("aaa").to<string>();
	cout << toString(b) << endl;
	uint32_t kk = myFromString("-10");
	cout << kk << endl;

	return 0;
}

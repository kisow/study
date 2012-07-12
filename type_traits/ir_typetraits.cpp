#include <cstdint>
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "ir_typetraits.hpp"

using namespace std;
using namespace ir;

#define IS_SAME(a, b) \
	cout << "is_same(" << #a << ',' << #b << ") = " << is_same<a, b>::value << endl;

int (&k(...))[10][10] {
	static int a[10][10];

	for(size_t i = 0; i < 10; i++) {
		for(size_t j = 0; j < 10; j++) {
			a[i][j] = i * 10 + j;
		}
	}

	return a;
}

int main()
{
	for(size_t i = 0; i < 10; i++) {
		for(size_t j = 0; j < 10; j++) {
			int (&ss)[10][10] =  k(0);
			cout << ss[i][j] << ' ';
		}
		cout << endl;
	}
	cout << endl;

	cout << boolalpha;
	IS_SAME(int8_t, char);
	IS_SAME(uint8_t, char);
	IS_SAME(int16_t, wchar_t);
	IS_SAME(uint16_t, wchar_t);
	IS_SAME(int32_t, int);
	IS_SAME(int32_t, long);
	IS_SAME(uint32_t, unsigned);
	IS_SAME(int64_t, long);
	IS_SAME(int64_t, long long);
	IS_SAME(int64_t, ssize_t);
	IS_SAME(int64_t, off_t);
	IS_SAME(uint64_t, unsigned);
	IS_SAME(uint64_t, unsigned long);
	IS_SAME(uint64_t, unsigned long long);
	IS_SAME(uint64_t, size_t);
	IS_SAME(float, double);
	IS_SAME(double, long double);

	cout << is_container<vector<int>>::value << endl;	//true
	cout << is_container<list<int>>::value << endl;		//true 
	cout << is_container<map<int, int>>::value << endl; //true
	cout << is_container<set<int>>::value << endl;		//true
	cout << is_container<int>::value << endl;			//false
}

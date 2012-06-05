#include "exception.h"
#include <iostream>

using namespace std;

extern "C" void f1() {
    try {
		throw Exception();
    } catch (Exception& e) {
            cerr << "f1: Exception. " << endl;
    } catch (...) {
            cerr << "f1: unknown exception - expected: Exception. " << endl;
    }
}

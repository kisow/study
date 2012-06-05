#include <iostream>
#include <dlfcn.h>
#include "exception.h"
using namespace std;

void f0 () {
   throw Exception();
}

main() {
    void *lib_f1;
    void (*f1_call)();

    if (!(lib_f1=dlopen("./libf1.so",RTLD_LAZY|RTLD_GLOBAL))) {
            cerr << endl;
            cerr << "Can't open ./libf1.so." << endl;
            cerr << endl;
            exit(1);
    }

    if (!(f1_call=(void (*)())dlsym(lib_f1,"f1"))) {
            cerr << endl;
            cerr << "Can't bind f1." << endl;
            cerr << endl;
            exit(2);
    }

	(*f1_call)();

    dlclose(lib_f1);
}


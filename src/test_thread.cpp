#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <chrono>
#include <thread>
#include <future>

using namespace std;

class ThreadTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ThreadTest);
    CPPUNIT_TEST(emcpp35_1);
    CPPUNIT_TEST_SUITE_END();

public:
    void emcpp35_1()/*{{{*/
    {
        auto func = [](char c) -> int {
            for(auto i = 0; i < 100; i++) {
                this_thread::sleep_for(chrono::milliseconds(10));
                cout << c;
                cout.flush();
            }
            return c;
        };

        auto f1 = [&]() { return func('+'); };
        auto f2 = [&]() { return func('-'); };

        future<int> r1 = async(launch::async, f1);
        int r2 = f2();

        int r = r1.get() + r2;
        cout << "\n\nfunc1() + func2() = " << r << endl ;
    }/*}}}*/
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadTest);
// vim:sw=4:ts=4:expandtab:smarttab:autoindent:fdm=marker

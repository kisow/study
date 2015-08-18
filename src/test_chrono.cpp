#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class ChronoTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(ChronoTest);
    CPPUNIT_TEST(tp_u64);
    CPPUNIT_TEST_SUITE_END();

public:
    void tp_u64()/*{{{*/
    {
        using u64_nano = duration<uint64_t, nano>;
        auto u64_to_tp = [](uint64_t timestamp) -> auto {
            return time_point<system_clock, u64_nano>{u64_nano{timestamp}};
        };

        auto tp_to_u64 = [](auto tp) -> uint64_t {
            return tp.time_since_epoch().count();
        };

        auto tp = system_clock::now();
        auto u64 = tp_to_u64(tp);
        auto tp_1 = u64_to_tp(u64);
        auto u64_1 = tp_to_u64(tp_1);
        CPPUNIT_ASSERT_EQUAL(u64, u64_1);
        CPPUNIT_ASSERT(time_point_cast<u64_nano>(tp) == tp_1);
    }/*}}}*/
};

CPPUNIT_TEST_SUITE_REGISTRATION(ChronoTest);
// vim:sw=4:ts=4:expandtab:smarttab:autoindent:fdm=marker

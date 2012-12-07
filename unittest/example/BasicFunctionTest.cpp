
// Header

#include <cppunit/extensions/HelperMacros.h>

class BasicFunctionTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( BasicFunctionTest );
  CPPUNIT_TEST( testGo );
  CPPUNIT_TEST( testEat );
  CPPUNIT_TEST_SUITE_END();

public:  
  virtual void setUp();
  virtual void tearDown();

  void testGo();
  void testEat();

private:
  int tiger_handle;
};





// Body

extern "C"{
#include "tiger.h"
}
#include <cppunit/TestResult.h>
#include "ir_test.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(BasicFunctionTest);

void BasicFunctionTest::setUp()
{
  tiger_handle = tiger_init("my new tiger");
}

void BasicFunctionTest::tearDown()
{
  tiger_finalize(tiger_handle);
}


void BasicFunctionTest::testGo()
{
  int result;

  result = tiger_go(tiger_handle, "Go get 'em tiger");

  CPPUNIT_ASSERT_EQUAL(4, result);
}

void BasicFunctionTest::testEat()
{
  char result_buf[256];
  tiger_eat(tiger_handle, "씨리얼", result_buf);

  CPPUNIT_ASSERT_STRING_EQUAL("호랑이 기운 솟아나요", result_buf);
}

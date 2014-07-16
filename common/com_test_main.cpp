//
// Copyright 2006, 2007, 2008, 2009 (c) NHN Corporation. All Rights Reserved.
//
//    THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED,
//    INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, 
//    FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL 
//    PROPERTY RIGHTS. 
// 
// NHN(R) Nexus(TM) VLV(VeryLargeVolume) IR System Development Environment 
//

/** 
	@mainpage IrMiscBasicTest
	@section intro 소개
	  
	CppUnit을 기반으로 한 IR_COMM의 단위테스트 프레임워크를 만드는 정의 파일.
	대용량 검색엔진 Nexus의 개발에 사용되는 단위 테스트에는 모두 이 파일이 컴파일되어 붙는다. 

	@section developer 개발자 
	 - 김홍준, Kim Hong-Joon (prolinko@nhncorp.com)

	@section info 추가정보
	
	 본 결과물은 NHN(주) CTO 기술부문 검색엔진개발팀의 공통 라이브러리 프로젝트인 "ir_comm"에서 지원,
     개발, 유지된다. 

	@section copyright 저작권 정보
	- Copyright 2006, 2007, 2008, 2009 (c) NHN Corporation. All Rights Reserved.
*/

#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <stdexcept>
#include <fstream>


int 
main( int argc, char* argv[] )
{
  // Retreive test path from command line first argument. Default to "" which resolve
  // to the top level suite.
  std::string testPath = (argc > 1) ? std::string(argv[1]) : std::string("");

  // Create the event manager and test controller
  CPPUNIT_NS::TestResult controller;

  // Add a listener that colllects test result
  CPPUNIT_NS::TestResultCollector result;
  controller.addListener( &result );        

  // Add a listener that print dots as test run.
  CPPUNIT_NS::BriefTestProgressListener progress;
  controller.addListener( &progress );      

  // Add the top suite to the test runner
  CPPUNIT_NS::TestRunner runner;
  
  CPPUNIT_NS::Test* allTests = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();

  runner.addTest(allTests);

  try
  {
    CPPUNIT_NS::stdCOut() << "Running "  <<  testPath << std::endl;
    runner.run( controller, testPath );

    CPPUNIT_NS::stdCOut() << "\n";

    // Print test in a compiler compatible format.
	CPPUNIT_NS::CompilerOutputter outputter( &result, CPPUNIT_NS::stdCOut() );
    outputter.write(); 

  }
  catch ( std::invalid_argument &e )  // Test path not resolved
  {
    CPPUNIT_NS::stdCOut()  <<  "\n"  
                            <<  "ERROR: "  <<  e.what()
                            << "\n";
    return 0;
  }
 
  return result.wasSuccessful() ? 0 : 1;
}

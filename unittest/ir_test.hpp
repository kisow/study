/** 
	@mainpage ir_test
	@section intro 소개
	  
	CppUnit을 기반으로 한 IR_COMM의 단위테스트 프레임워크를 만드는 헤더 파일.

	@section developer 개발자 
	 - 김홍준, Kim Hong-Joon (prolinko@nhncorp.com)

	@section info 추가정보
	
	 본 결과물은 NHN(주) CTO 기술부문 검색엔진개발팀의 공통 라이브러리 프로젝트인 "ir_comm"에서 지원,
     개발, 유지된다. 

	@section copyright 저작권 정보
	- Copyright 2006, 2007, 2008, 2009 (c) NHN Corporation. All Rights Reserved.
*/

#ifndef _IR_TEST_HPP
#define _IR_TEST_HPP

#include <cppunit/TestAssert.h>


#define CPPUNIT_ASSERT_STRING_EQUAL(expected, actual) \
	( CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(actual)) )




#endif // _IR_TEST_HPP

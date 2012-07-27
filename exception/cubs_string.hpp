#ifndef __CUBS_STRING_HPP__
#define __CUBS_STRING_HPP__

// author : kisow@nhn.com

#include <string>
#include <sstream>
#include <iostream>
#include <type_traits>
#include <boost/lexical_cast.hpp>

namespace cubs
{

using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// toString : value-to-String
//
// 디폴트		-> <?>
// 산술타입		-> 10진수 
// bool			-> true or false
// char			-> '문자'
// const char*	-> "문자열"
// string		-> 문자열
// 포인터		-> 0x 16진수 
//
// 기본적으로 변환 가능 타입은 위와 같으며, 
// 위의 규칙에 따라 임의의 파라미터 목록은 아래와 같이 변환한다.
//
// 임의의 파라미터 목록 -> (변환문자열, 변환문자열, ...)
// 빈 파라미터 -> 빈 문자열
//
// 예) printf("%s\n", toString("10", string("11"), true, '\n')).c_str());
//
// string toString(XXX) 함수를 overloading 함으로써
// 임의의 파라미터 변환도 비관입적(non-intrusive)으로 확장할 수 있다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail/*{{{*/
{
	template <typename T, typename E = void>
	struct ToStringImpl
	{ 
		static string convert(const T& t) 
		{ 
			return string("<?>"); 
		} 
	};

	template <typename T>
	struct ToStringImpl<T, typename enable_if<is_arithmetic<T>::value>::type>
	{ 
		static string convert(const T& t) 
		{ 
			return to_string(t); 
		} 
	};

	template <typename T>
	struct ToStringImpl<T, typename enable_if<is_pointer<T>::value>::type>
	{ 
		static string convert(const T& t) 
		{ 
			stringstream ss;
			ss << t;
			return ss.str();
		} 
	};

	template <typename T>
	struct ToStringImpl<T, typename enable_if<is_array<T>::value && is_same<T, char>::value>::type>
	{ 
		static string convert(const T& t) 
		{ 
			return string(t); 
		} 
	};
};/*}}}*/

template <typename T> inline string toString(const T& t) { return detail::ToStringImpl<T>::convert(t); }
inline string toString(bool t) { return t ? "true" : "false"; }
inline string toString(char t) { return string(1, '\'') + string(1, t) + string(1, '\''); }
inline string toString(const char* t) { return string(1, '\"') + string(t) + string(1, '\"'); }
inline string toString(const string& t) { return toString(t.c_str()); }

namespace detail/*{{{*/
{
	template<typename T, size_t N>
	struct TupleConverter
	{
		static string convert(const T& t)
		{
			string str;
			if(N > 1) str += TupleConverter<T, N-1>::convert(t) + ", ";
			return str + toString(get<N-1>(t));
		}
	};

	template<typename T>
	struct TupleConverter<T, 0>
	{
		static string convert(const T& t) { return string(""); }
	};

	template<size_t N, typename T, typename... A>
	struct TupleMaker
	{
		static constexpr std::tuple<T, A...>
		make(const T& head, const A&... tail)
		{
			return tuple_cat(tie(head), TupleMaker<N-1, A...>::make(tail...));
		}
	};

	template<typename T>
	struct TupleMaker<1, T>
	{
		static constexpr std::tuple<T>
		make(const T& head)
		{
			return tuple_cat(tie(head));
		}
	};
};/*}}}*/

template<typename... A>
inline string toString(A&&... a)
{
	auto t = detail::TupleMaker<sizeof...(A), A...>::make(a...);
	return detail::TupleConverter<decltype(t), sizeof...(A)>::convert(t);
}
inline string toString() { return string(); }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// fromString : value-from-String
//
// 기본 변환가능 타입 : 산술타입, bool, string		
// - 파싱 에러시
// -- 산술타입 : boost::bad_lexical_cast 예외 발생
// -- bool : false 값으로 반환됨.
//
// 암시적 형변환을 이용하여 아래처럼 return type only overloading 효과를 볼 수 있다.
//
// 예)  int value = fromString("10");		// or fromString("10", value);
//		bool value = fromString("true");	// or fromString("true", value);
//		string value = fromString("aaaa");	// or fromString("aaaa", value);
//		
// fromString(const string&, T&) 함수를 overloading 함으로써 
// fromString(const string&) 함수를 비 관입적(non-intrusive)으로 확장할 수 있다.
//
// 특정 enum type에 대한 overloading을 추가하면
// 암시적 형변환을 이용하는 형태에서도 해당 enum type의 암시적 형변환을 지원한다.
// 다만 그 밖의 타입에 대해서는 암시적 형변환의 모호성에 따른 부작용 때문에 막아뒀다.
//
// 그 외의 타입에 대한 overloading을 추가하고, fromString을 이용하려면
// 리턴되는 FromStringProxy객체의 to 멤버를 사용할 수 있다.
//
// 예) A a = fromString("a").to<A>(); // fromString(const string&, A&) 함수에 대한 오버로딩 구현이 있어야 한다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
inline void fromString(const string& str, T& value,
		typename enable_if<
			is_arithmetic<T>::value
			|| is_same<T, string>::value
			, T>::type* = 0)
{
	value = boost::lexical_cast<T>(str);
}

inline void fromString(const string& str, bool& value)
{
	value = str.compare("true") ? false : true;
}

struct FromStringProxy 
{
	string str_;
	FromStringProxy(const string& str) : str_(str) {}
	template <typename T,
		typename = typename enable_if<
			is_arithmetic<T>::value
			|| is_enum<T>::value
			|| is_same<T, string>::value
			|| is_same<T, bool>::value
			, T>::type
	>
	operator T()
	{
		T value;
		fromString(str_, value);
		return value;
	}
	
	template <typename T>
	T to()
	{
		T value;
		fromString(str_, value);
		return value;
	}
};

inline FromStringProxy fromString(const string& str) { return FromStringProxy(str); }

}; // end of cubs

#endif // end of include guard: __CUBS_STRING_HPP__ 

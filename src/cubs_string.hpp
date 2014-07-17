#ifndef CUBS_STRING_HPP__
#define CUBS_STRING_HPP__

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
// 산술타입		-> 10진수
// bool			-> true or false
// char			-> 문자
// const char*	-> 문자열
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
// string toString(XXX) 함수 템플릿을 특수화 함으로써
// 임의의 파라미터 변환도 비관입적(non-intrusive)으로 확장할 수 있다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T,
		 typename = typename enable_if<
			 !(is_arithmetic<T>::value && !is_same<T, bool>::value && !is_same<T, char>::value)
			 && !is_array<T>::value
			 && !is_same<T, string>::value
			 && !is_pointer<T>::value
			>::type
		 > inline string toString(T t);
template <> inline string toString<bool>(bool t) { return t ? "true" : "false"; }
template <> inline string toString<char>(char t) { return string(1, t); }

template <typename T>
inline string toString(const T& t,
		typename enable_if<
			is_arithmetic<T>::value
			&& !is_same<T, char>::value
			&& !is_same<T, bool>::value
		>::type* = 0)
{ return to_string(t); }

template <typename T>
inline string toString(const T& t,
		typename enable_if<
			is_array<T>::value
		>::type* = 0)
{ return string(t); }

template <typename T>
inline string toString(const T& t,
		typename enable_if<is_same<T, string>::value>::type* = 0)
{ return t; }

template <typename T>
inline string toString(const T& t,
		typename enable_if<is_pointer<T>::value>::type* = 0)
{ stringstream ss; ss << t; return ss.str(); }

namespace detail/*{{{*/
{
	template <typename T, typename E = void>
	struct ToStringImpl { static string convert(const T& t) { return string("<?>"); } };

	template <typename T>
	struct ToStringImpl<T,
		typename enable_if<
			is_arithmetic<T>::value
			|| is_pointer<T>::value
			|| (is_array<T>::value && is_same<typename remove_extent<T>::type, char>::value)
		>::type>
	{
		static string convert(const T& t) { return toString(t); }
	};
}/*}}}*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// toStringWithDefault : value-to-String
//
// 디폴트		-> <?>
//
// 기본 구현은 toString 과 같지만 구현되어 있지 않은 타입에 대해서는
// 컴파일 에러가 나는 대신 "<?>"로 변환하는 점이 다르다.
//
// toString을 확장하면 toStringWithDefault에도 반영된다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> inline string toStringWithDefault(const T& t) { return detail::ToStringImpl<T>::convert(t); }

namespace detail/*{{{*/
{
	template<typename T, size_t N>
	struct TupleConverter
	{
		static string convert(const T& t)
		{
			string str;
			if(N > 1) str += TupleConverter<T, N-1>::convert(t) + ", ";
			return str + toStringWithDefault(get<N-1>(t));
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
}/*}}}*/

template<typename... A>
inline string toStringWithDefault(A&&... a)
{
	auto t = detail::TupleMaker<sizeof...(A), A...>::make(a...);
	return detail::TupleConverter<decltype(t), sizeof...(A)>::convert(t);
}
inline string toStringWithDefault() { return string(); }

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
// fromString(const string&, T&) 함수 템플릿을 특수화 함으로써
// fromString(const string&) 함수를 비 관입적(non-intrusive)으로 확장할 수 있다.
//
// 특정 enum type에 대한 템플릿 특수화 구현을 추가하면
// 암시적 형변환을 이용하는 형태에서도 해당 enum type의 암시적 형변환을 지원한다.
// 다만 그 밖의 타입에 대해서는 암시적 형변환의 모호성에 따른 부작용 때문에 막아뒀다.
//
// 그 외의 타입에 대한 템플릿 특수화를 추가하고, fromString을 이용하려면
// 리턴되는 FromStringProxy객체의 to 멤버를 사용할 수 있다.
//
// 예) A a = fromString("a").to<A>(); // fromString<A>(const string&, A&) 함수에 대한 구현이 있어야 한다.
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

namespace detail
{
	template <typename T>
		struct has_default_from_string_impl
		: integral_constant<
		  bool,
			(is_arithmetic<T>::value
			 && is_same<T, char>::value == false
			 && is_same<T, bool>::value == false
			 )
			|| is_same<T, string>::value
	 > {};
}

template <typename T,
		 typename = typename enable_if<
			 detail::has_default_from_string_impl<T>::value == false
			>::type
		 >
inline void fromString(const string& str, T& value);

template <> inline void fromString<bool>(const string& str, bool& value)
{
	value = str.compare("true") ? false : true;
}

template <typename T>
inline void fromString(const string& str, T& value,
		typename enable_if<detail::has_default_from_string_impl<T>::value, T>::type* = 0)
{
	value = boost::lexical_cast<T>(str);
}

struct FromStringProxy
{
	string str_;
	FromStringProxy(const string& str) : str_(str) {}

	template <typename T,
		typename = typename enable_if<
			(is_arithmetic<T>::value && is_same<T, char>::value == false)
			|| is_enum<T>::value
			|| is_same<T, string>::value
			|| is_same<T, bool>::value
			, T>::type
		> operator T() const { return to<T>(); }

	template <typename T> T to() const
	{
		T value;
		fromString(str_, value);
		return value;
	}
};

inline FromStringProxy fromString(const string& str) { return FromStringProxy(str); }

inline string escapeNewLine(const string& orgStr)
{
	string str(orgStr);
	size_t found = string::npos;

	while((found = str.find("\n")) != string::npos) {
		str.replace(found, 1, "\\n");
	}

	return str;
}

string format(const char* format, ...)
	__attribute__((format(printf, 1, 2)));
string vformat(const char* format, va_list ap)
	__attribute__((format(printf, 1, 0)));


} // end of cubs

#endif // end of include guard: CUBS_STRING_HPP__

#ifndef __TO_STRING_HPP__
#define __TO_STRING_HPP__

// author : kisow@nhn.com

#include <string>

using namespace std;

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
	struct ToStringImpl<T, typename enable_if<is_array<T>::value && is_same<T, char>::value>::type>
	{ 
		static string convert(const T& t) 
		{ 
			return string(t); 
		} 
	};
};/*}}}*/

template <typename T> inline string toString(const T& t) { return detail::ToStringImpl<T>::convert(t); }
inline string toString(char t) { return string(1, '\'') + string(1, t) + string(1, '\''); }
inline string toString(const char* t) { return string(1, '\"') + string(t) + string(1, '\"'); }
inline string toString(const string& t) { return toString(t.c_str()); }

#endif // end of include guard: __TO_STRING_HPP__ 

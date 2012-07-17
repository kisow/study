#ifndef __ARGS_TO_STRING_HPP__
#define __ARGS_TO_STRING_HPP__

// author : kisow@nhn.com

#include <tuple>
#include "to_string.hpp"

namespace args
{
	using namespace std;

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
	static inline string toString(A&&... a)
	{
		auto t = detail::TupleMaker<sizeof...(A), A...>::make(a...);
		return detail::TupleConverter<decltype(t), sizeof...(A)>::convert(t);
	}
};

#endif // end of include guard: __ARGS_TO_STRING_HPP__ 

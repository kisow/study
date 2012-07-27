#ifndef __IR_TYPETRAITS_HPP__
#define __IR_TYPETRAITS_HPP__

#include <type_traits>
	
namespace ir {

struct yes_no
{
	typedef char yes;
	typedef char (&no)[2];  
};

template <typename T>
struct has_const_iterator : private yes_no
{
	private:
		template<typename C> static yes test(typename C::const_iterator*);
		template<typename C> static no  test(...);

	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(yes);
		typedef T type;
};

template <typename T>
struct has_begin : private yes_no
{
	private:
		template <typename C> static yes f(
					typename std::enable_if<
						std::is_same<
							decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
							typename C::const_iterator (C::*)() const
						>::value
					>::type*);

		template<typename C> static no f(...);

	public:
		static const bool value = sizeof(f<T>(0)) == sizeof(yes);
};

template <typename T>
struct has_end : private yes_no
{
	private:
		template<typename C> static yes f(
					typename std::enable_if<
						std::is_same<
							decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
							typename C::const_iterator(C::*)() const
						>::value
					>::type*);

		template<typename C> static no f(...);

	public:
		static const bool value = sizeof(f<T>(0)) == sizeof(yes);
};

template <typename T>
struct is_serializable 
	: std::integral_constant<bool,
		std::is_enum<T>::value ||
		std::is_arithmetic<T>::value
> {};

template <typename T> 
struct is_container 
	: std::integral_constant<bool, 
		has_const_iterator<T>::value && 
		has_begin<T>::value && 
		has_end<T>::value
> {};
	
} // ir

#endif // end of include guard: __IR_TYPETRAITS_HPP__ 

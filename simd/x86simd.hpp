#ifndef __X86SIMD_HPP__

#define __X86SIMD_HPP__

// author : kisow@nhn.com

#include <stdint.h>
#include <emmintrin.h>
#include <tmmintrin.h>
#include <algorithm>
#include <iostream>

namespace x86simd {

class Cpuid 
{
public:
	Cpuid() : end_(0) {}

	bool mmx() { cpuid(1); return flag(dx_, 23); }
	bool sse() { cpuid(1); return flag(dx_, 25); }
	bool sse2() { cpuid(1); return flag(dx_, 26); }
	bool htt() { cpuid(1); return flag(dx_, 28); }

	bool sse3() { cpuid(1); return flag(cx_, 0); }
	bool ssse3() { cpuid(1); return flag(cx_, 9); }
	bool amdmmxe() { cpuid(0x80000001); return flag(dx_, 22); }
	bool amd3dnow2() { cpuid(0x80000001); return flag(dx_, 30); }
	bool amd3dnow() { cpuid(0x80000001); return flag(dx_, 31); }

	const char* getVendorString() { cpuid(0); return str_; }

private:
	static bool flag(int x, int b) { return ((1 << b) & x) != 0; }
	void cpuid(int func) 
	{ 
		__asm__ __volatile__ ("cpuid": "=a" (ax_), "=b" (bx_), "=c" (cx_), "=d" (dx_) : "a" (func));
	}

public:
	int		ax_;
	union {
		struct {
			int		bx_;
			int		dx_;
			int		cx_;
		};
		char str_[12];
	};
	int		end_;
};

class M128 
{
public:
	M128() {}
	M128(const M128& src) : i_(src.i_) {}
	M128(const __m128i& i) : i_(i) {}
	M128(const __m128d& d) : d_(d) {}
	~M128() {}

	const M128& operator=(const M128& src) { i_ = src.i_; return *this; }

	operator __m128i&() { return i_; } 
	operator const __m128i&() const { return i_; } 

	operator __m128d&() { return d_; } 
	operator const __m128d&() const { return d_; } 

	template <typename Type>
	const Type* cbegin() const { return reinterpret_cast<const Type*>(&i_); }
	template <typename Type>
	Type* begin() { return const_cast<Type*>(cbegin<Type>()); }

	template <typename Type>
	const Type* cend() const { return cbegin<Type>() + (sizeof(i_) / sizeof(Type)); }
	template <typename Type>
	Type* end() { return const_cast<Type*>(cend<Type>()); }

private:
	union {
		__m128i		i_;
		__m128d		d_;
	};
};

template <typename Type>
void pshufb(Type& buf, const Type& mask)
{
	//buf = _mm_shuffle_epi8(buf, mask);
	__asm__ __volatile__ ("pshufb %1, %0" : "+x" (buf) : "xm" (mask));
}

template <>
inline void pshufb(M128& buf, const M128& mask) 
{ 
	pshufb<__m128i>(buf, mask); 
}

template <typename BufType, typename MaskType>
inline void pshufb(BufType& buf, const MaskType& mask)
{
	pshufb(*((M128*)&buf), *((M128*)&mask));
}

template <typename BufType, typename MaskType>
inline void pshufb(BufType* buf, const MaskType* mask)
{
	pshufb(*((M128*)buf), *((M128*)mask));
}

}; // end of namespace x86simd

#endif // end of include guard: __X86SIMD_HPP__ 

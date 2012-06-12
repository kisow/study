#ifndef __X86SIMD_HPP__

#define __X86SIMD_HPP__

// author : kisow@nhn.com

class X86Cpuid 
{
public:
	X86Cpuid() : end_(0) {}

	bool mmx() { cpuid(1); return flag(dx_, 23); }
	bool sse() { cpuid(1); return flag(dx_, 25); }
	bool sse2(){ cpuid(1); return flag(dx_, 26); }
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

template <typename Type>
Type pshufb(Type buf, Type mask)
{
	__asm__ __volatile__ ("pshufb %1, %0" : "+x" (buf) : "xm" (mask));

	return buf;
}

#endif // end of include guard: __X86SIMD_HPP__ 

#include <tuple>                                                                                          
#include <string>                                                                                         
#include <cstdlib>                                                                                        
#include <iostream>                                                                                       
#include <memory>                                                                                         

using namespace std;                                                                                      

template <typename T, typename E = void>                                                                  
struct ToString                                                                                           
{ static std::string str(const T& t) { return std::string("<?>"); } };                                    

template <typename T>                                                                                     
struct ToString<T, typename std::enable_if<std::is_arithmetic<T>::value>::type>                           
{ static std::string str(const T& t) { return std::to_string(t); } };                                     

template <typename T>                                                                                     
struct ToString<T, typename std::enable_if<std::is_array<T>::value>::type>                                
{ static std::string str(const T& t) { return std::string(t); } };                                        

template <typename T> inline std::string ts(const T& t) { return ToString<T>::str(t); }                   
std::string ts(const char& t) { return std::string(1, '\'') + std::string(1, t) + std::string(1, '\''); } 
std::string ts(const char* t) { return std::string(1, '\"') + std::string(t) + std::string(1, '\"'); }    
std::string ts(const std::string& t) { return ts(t.c_str()); }                                            

struct A                                                                                                  
{                                                                                                         
	template<typename T, size_t N>                                                                        
		struct ArgStr                                                                                     
		{                                                                                                 
			static std::string str(const T& t)                                                            
			{                                                                                             
				std::string str;                                                                          
				if(N > 1) str += ArgStr<T, N-1>::str(t) + ", ";                                           
				return str + ts(std::get<N-1>(t));                                                        
			}                                                                                             
		};                                                                                                

	template<typename T>                                                                                  
		struct ArgStr<T, 0>                                                                               
		{                                                                                                 
			static std::string str(const T& t) { return std::string(""); }                                
		};                                                                                                

	template<size_t N, typename T, typename... Args>                                                      
		struct MakeTuple                                                                                  
		{                                                                                                 
			static constexpr std::tuple<T, Args...>                                                       
				make(const T& head, const Args&... tail)                                                      
				{                                                                                             
					return std::tuple_cat(std::tie(head), MakeTuple<N-1, Args...>::make(tail...));            
				}                                                                                             
		};                                                               

	template<typename T>                                                                                  
		struct MakeTuple<1, T>                                                                            
		{                                                                                                 
			static constexpr std::tuple<T>                                                                
				make(const T& head)                                                                           
				{                                                                                             
					return std::tuple_cat(std::tie(head));                                                    
				}                                                                                             
		};                                                                                                

	template<typename... A>                                                                               
		static inline std::string makeArgStr(A&&... a)                                                    
		{                                                                                                 
			auto t = MakeTuple<sizeof...(A), A...>::make(a...);                                           
			return ArgStr<decltype(t), sizeof...(A)>::str(t);                                             
		}                                                         
};

struct Base                                                                                               
{                                                                                                         
	virtual void test() = 0;                                                                              
};                                                                                                        

struct Derived : public Base                                                                              
{                                                                                                         
	virtual void test() {}                                                                                
};                                                                                                        

#define F(...) {    \
	cout << '(' << #__VA_ARGS__ << ") = (" << A::makeArgStr(__VA_ARGS__) << ')' << endl; \
}                                                                                                         

int main() {                                                                                              
	void (*f1_call)();                                                                                    

	char c = 'a';                                                                                         
	unsigned char uc = 'A';                                                                               
	char array[3] = "ar";                                                                                 

	int8_t  i8 = -8;                                                                                      
	int16_t i16= -16;                                                                                     
	int32_t i32 = -32;                                                                                    
	int64_t i64 = -64;                                                                                    
	long        l = -100;                                                                                 
	long long   ll = -1000;                                                                               

	uint8_t     ui8 = 8;                                                                                  
	uint16_t    ui16= 16;                                                                                 
	uint32_t    ui32 = 32;                                                                                
	uint64_t    ui64 = 64;                                                                                
	unsigned long       ul = 100;                                                                         
	unsigned long long  ull = 1000;                                                                       

	float   f32 = 32.0;                                                                                   
	double  d64 = 64.0;                                                                                   

	struct FunctionNested {} fn;                                                                          
	void *voidPtr = &ull;                                                                                 
	void *& voidPtrRef = voidPtr;                                                                         
	Base *basePtr = new Derived();                                                                        
	std::shared_ptr<Base> baseSharedPtr(new Derived());                                                   

	F(c, uc, 'y', "YYY", array, string("std::string"));                                                   
	F(i8, i16, i32, i64, -1, -10L, -100LL);                                                               
	F(ui8, ui16, ui32, ui64, 1, 10UL, 100ULL);                                                            
	F(l, ll, ul, ull, f1_call);                                                                                    
	F(f32, d64, fn, voidPtr, voidPtrRef, basePtr, baseSharedPtr);                                         

	return 0;                                                                                             
}                                                          

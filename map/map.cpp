#include <boost/unordered_map.hpp>
#include <boost/lexical_cast.hpp>
#include <sys/time.h>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <unordered_map>
#include <boost/config.hpp>

#include "tst.hpp"

using namespace std;
using namespace std;

class PrintElapsedTime
{
	public:
		PrintElapsedTime(const char *progname) 
			: progname_(progname)
		{
			gettimeofday(&start_, NULL);
		}
		virtual ~PrintElapsedTime()
		{
			struct timeval end;

			gettimeofday(&end, NULL);
			if(end.tv_sec >= start_.tv_sec) {
				printf("%s: Elapsed time: %f sec\n", progname_.c_str(), 
						end.tv_sec + double(end.tv_usec) / 1000000.0
						- (start_.tv_sec + double(start_.tv_usec) / 1000000.0));
			}
		}

	private:
		struct timeval 	start_;
		std::string		progname_;
};

template <typename MapType>
void checkTime()
{
	PrintElapsedTime pt(typeid(MapType).name());
	uint32_t max = 1 << 20;
	MapType x;

	for(uint32_t i = 0; i < max; i++) {
		x[boost::lexical_cast<string>(i)] = i;
	}
	for(uint32_t i = 0; i < max; i++) {
		uint32_t j = x[boost::lexical_cast<string>(i)];
		assert(j == i);
	}
}

//namespace __gnu_cxx
//{
//  template<> struct hash<string>
//  {
//    size_t operator()(const string& __s) const
//    { return __stl_hash_string(__s.c_str()); }
//  };
//}

int main(int argc, char *argv[])
{
	typedef boost::unordered_map<string, uint32_t> BoostMap;
	typedef std::map<string, uint32_t> StdMap;
	typedef std::unordered_map<string, uint32_t> HashMap;
	typedef TernarySearchTree<uint32_t> TSTMap;

	checkTime<BoostMap>();
	checkTime<StdMap>();
	checkTime<HashMap>();
	checkTime<TSTMap>();

	cout << BOOST_COMPILER << endl;

	return 0;
}

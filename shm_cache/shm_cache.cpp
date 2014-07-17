#include "xsi_shm.hpp"
#include "../sha1/sha1.hpp"
#include <fstream>

using namespace std;
using namespace boost::interprocess;

//const size_t MEMORY_LIMIT = 20ULL * 1024 * 1024 * 1024; 
const size_t MEMORY_LIMIT = 65535;

ostream& operator<<(ostream& out, const Sha1& sha1)
{
	auto flags = out.setf(ios::hex, ios::basefield); 
	out.setf(ios::uppercase);
	for(auto u : sha1) out << u;
	out.setf(flags);

	return out;
}

int main(int argc, char *argv[])
{
	try {
		xsi_key shmKey(argv[0], 1);

		SharedMemoryCache<Sha1> c(shmKey, MEMORY_LIMIT);

		if(argc > 1) {
			if((*argv[1]) == '1') {
				SharedMemoryCache<Sha1>::destroy(shmKey);
				PRINT << "remove " << argv[0] << endl;
				return 0;
			} else {
				c.printInfo();
				return 0;
			}
		}

		fork();
		fork();
		fork();
		fork();

		PRINT << c.getFreeMemory() << endl;

		srand(getpid());
		//Insert data in the map
		for(int i = 0; i < 10000; i++) {
			int seed = rand() % 1000;
			Sha1 key((const char*)&seed, sizeof(seed));
			std::string value;

			if(c.get(key, value)) {
				PRINT << "hit : [" << key << ']' << value << endl;
			} else {
				value.assign(seed, 'v');
				c.put(key, value);
				PRINT << "miss : [" << key << ']' << value << endl;
			}
		}
	} catch (std::exception& ex) {
		PRINT << ";;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;; " << ex.what() << endl;
		return 1;
	}

	return 0;
}

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <iostream>
#include <fstream>
#include <limits>
#include <vector>

#include "sha1.hpp"

using namespace std;

static inline int self_check() /*{{{*/
{
	ifstream msg("vectors/byte-messages.sha1");
	ifstream hash("vectors/byte-hashes.sha1");

	do {
		int z, b;
		msg >> z >> b;

		if(msg.eof() || msg.bad()) {
			break;
		} else if(msg.fail()) {
			msg.clear();
			msg.ignore(numeric_limits<streamsize>::max(), '\n');
			hash.ignore(numeric_limits<streamsize>::max(), '\n');
		} else {
			vector<unsigned char> msg_str;
			int s = 0;
			while(z--) {
				int t;
				msg >> t;
				while(t--) {
					if(s % 8 == 0) msg_str.push_back(0);
					if(b) {
						msg_str.back() += b << (7 - (s % 8));
					}
					s++;
				}
				b ^= 1;
			}
			Sha1 sha(msg_str.data(), msg_str.size());
			char r[21] = {0};
			sprintf(r, "%08X%08X%08X%08X%08X", sha[0], sha[1], sha[2], sha[3], sha[4]);

			string hash_str;
			hash >> hash_str;
			if(hash_str.compare(r) != 0) {
				msg_str.push_back(0);
				//				cout << "-------------------------------" << endl;
				//				cout << msg_str.data() << endl;
				//				cout << hash_str << endl;
				//				cout << r << endl;
				return 0;
			}
		}
	} while(1);

	cout << sizeof(Sha1) << endl;

	return 1;
}/*}}}*/

int main(int argc, char **argv) 
{
	if(!self_check()) {
		printf("Self-check failed\n");
		return 1;
	}
	printf("Self-check passed\n");

	// Benchmark speed
	uint32_t state[5];
	uint32_t block[16];
	const int N = 10000000;
	clock_t start_time = clock();
	for(int i = 0; i < N; i++)
		Sha1::compress(state, block);
	printf("Speed: %.1f MiB/s\n", (double)N * 64 / (clock() - start_time) * CLOCKS_PER_SEC / 1048576);

	return 0;
}


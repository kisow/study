/*
 *  shatest.cpp
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: shatest.cpp 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This file will exercise the SHA1 class and perform the three
 *      tests documented in FIPS PUB 180-1.
 *
 *  Portability Issues:
 *      None.
 *
 */

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cassert>
#include "sha1.h"

using namespace std;

/*
 *  Define patterns for testing
 */
#define TESTA   "abc"
#define TESTB   "abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq"

/*
 *  Function prototype
 */
void DisplayMessageDigest(unsigned *message_digest);

/*  
 *  main
 *
 *  Description:
 *      This is the entry point for the program
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *
 */
int main()
{
    SHA1        sha;
    unsigned    message_digest[5];

    /*
     *  Perform test A
     */
    cout << endl << "Test A: 'abc'" << endl;

    sha.Reset();
    sha << TESTA;

    if (!sha.Result(message_digest))
    {
        cerr << "ERROR-- could not compute message digest" << endl;
    }
    else
    {
        DisplayMessageDigest(message_digest);
        cout << "Should match:" << endl;
        cout << '\t' << "A9993E36 4706816A BA3E2571 7850C26C 9CD0D89D" << endl;
    }

    /*
     *  Perform test B
     */
    cout << endl << "Test B: " << TESTB << endl;

    sha.Reset();
    sha << TESTB;

    if (!sha.Result(message_digest))
    {
        cerr << "ERROR-- could not compute message digest" << endl;
    }
    else
    {
        DisplayMessageDigest(message_digest);
        cout << "Should match:" << endl;
        cout << '\t' << "84983E44 1C3BD26E BAAE4AA1 F95129E5 E54670F1" << endl;
    }

    /*
     *  Perform test C
     */
    cout << endl << "Test C: One million 'a' characters" << endl;

    sha.Reset();
    for(int i = 1; i <= 1000000; i++) sha.Input('a');

    if (!sha.Result(message_digest))
    {
        cerr << "ERROR-- could not compute message digest" << endl;
    }
    else
    {
        DisplayMessageDigest(message_digest);
        cout << "Should match:" << endl;
        cout << '\t' << "34AA973C D4C4DAA4 F61EEB2B DBAD2731 6534016F" << endl;
    }

//	{
//		sha.Reset();
//		const int N = 10000000;
//		clock_t start_time = clock();
//		int i;
//		for (i = 0; i < N; i++)
//			sha.ProcessMessageBlock();
//		printf("Speed: %.1f MiB/s\n", (double)N * 64 / (clock() - start_time) * CLOCKS_PER_SEC / 1048576);
//	}

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
			sha.Reset();
			sha.Input(msg_str.data(), msg_str.size());

			unsigned d[5];
			sha.Result(d);
			char r[21] = {0};
			sprintf(r, "%08X%08X%08X%08X%08X", d[0], d[1], d[2], d[3], d[4]);

			string hash_str;
			hash >> hash_str;
			if((hash_str.compare(r) != 0)) {
				msg_str.push_back(0);
				cout << "-------------------------------" << endl;
				cout << msg_str.data() << endl;
				cout << hash_str << endl;
				cout << r << endl;
			}
		}
	} while(1);

    return 0;
}

/*  
 *  DisplayMessageDigest
 *
 *  Description:
 *      Display Message Digest array
 *
 *  Parameters:
 *      None.
 *
 *  Returns:
 *      Nothing.
 *
 *  Comments:
 *
 */
void DisplayMessageDigest(unsigned *message_digest)
{
    ios::fmtflags   flags;

    cout << '\t';

    flags = cout.setf(ios::hex|ios::uppercase,ios::basefield);
    cout.setf(ios::uppercase);

    for(int i = 0; i < 5 ; i++)
    {
        cout << message_digest[i] << ' ';
    }

    cout << endl;

    cout.setf(flags);
}

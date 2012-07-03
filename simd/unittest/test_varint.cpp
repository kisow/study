#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>
#include "ir_test.hpp"

#include <sys/time.h>
#include <string>
#include <bitset>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/std/vector.hpp>
#include "varint.hpp"

using namespace std;
using namespace varint;
using namespace boost::assign;

template <typename Type>
ostream& operator<<(ostream& out, const vector<Type>& container)
{
	for(const Type& it : container) {
		out << (int)it << ' ';
	}
	return out;
}

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

class VarintTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(VarintTest);
	CPPUNIT_TEST(benchmarkCodec);
	CPPUNIT_TEST(testG8IUTable);
	CPPUNIT_TEST(testG8IUCodec<uint16_t>);
	CPPUNIT_TEST(testG8IUCodec<uint32_t>);
	CPPUNIT_TEST(testG8IUCodec<uint64_t>);
	CPPUNIT_TEST(testG8IUCodec<int16_t>);
	CPPUNIT_TEST(testG8IUCodec<int32_t>);
	CPPUNIT_TEST(testG8IUCodec<int64_t>);
	CPPUNIT_TEST_SUITE_END();

public:
	template <size_t INT_SIZE>
	void printG8IUTable(ostream& out)/*{{{*/
	{
		typedef G8IU::Table<INT_SIZE> Table;
		typedef G8IU::Entry<INT_SIZE> Entry;

		const Table& table = G8IU::getTable<INT_SIZE>();
		for(size_t d = 0; d < Table::DESC_MAX; d++) {
			const Entry& entry = table.getEntry(d);
			const int8_t *seq = (int8_t*)&(entry.shuffleSequence[0]);

			out << bitset<8>(d) << ' ';
			out.width(2);
			out << (int)entry.getInputOffset() << ' ';
			out.width(2);
			out << (int)entry.getOutputOffset() << ' ';
			for(size_t i = 0; i < entry.getOutputOffset(); i++) {
				if(i % INT_SIZE == 0) { 
					if(i > 0) out << ']';
					out << '[';
				}
				if(seq[i] == -1) {
					out << ' ';
				} else {
					out << (int)seq[i];
				}
			}
			if(entry.getOutputOffset() > 0) out << ']';
			out << endl;
		}
		out << "-------------------------------------" << endl;

		for(size_t d = 0; d < Table::DESC_MAX; d++) {
			out << bitset<8>(d) << ' ' << Entry::num(d) << '=';
			for(size_t i = 0; i < Entry::num(d); i++) {
				if(i > 0) out << '+';
				out << Entry::len(d, i);
			}
			out << endl;
		}
	}/*}}}*/

	void testG8IUTable()/*{{{*/
	{
		ofstream file;

		file.open("g8iutable_2.txt");
		printG8IUTable<2>(file);
		file.close();

		file.open("g8iutable_4.txt");
		printG8IUTable<4>(file);
		file.close();

		file.open("g8iutable_8.txt");
		printG8IUTable<8>(file);
		file.close();
	}/*}}}*/

#if 0
	void printBuf(const vector<uint8_t>& buf)/*{{{*/
	{
		for(size_t i = 0; i < buf.size(); i++) {
			if(i == 0) {
				cout << (int)buf[i];
			} else {
				if(i % 9 == 1) {
					cout << endl << bitset<8>(buf[i]);
				} else {
					cout << ' ' << (int)buf[i];
				}
			}
		}
		cout << endl;
	}/*}}}*/
#endif

	template <typename Codec, typename Type>
	void testCodec(const vector<Type>& src)/*{{{*/
	{
		vector<Type> dest;
		vector<uint8_t>	buf;

		buf.resize((sizeof(src[0]) + 1) * src.size());

		uint8_t* bufPtr = &buf[0];
		encodeWithCnt<Codec>(src, bufPtr);

//		cout << endl;
//		cout << "src: " << src << endl;
//		cout << "buf: " << endl;
//		printBuf(buf);

		bufPtr = &buf[0];
		decodeWithCnt<Codec>(bufPtr, dest);
		CPPUNIT_ASSERT(src == dest);
	}/*}}}*/

	template <typename Type>
	void testG8IUCodec()
	{
		vector<Type> values;
		values += 1,2,3,4,5,6,7,8,9,10;
		testCodec<G8IU>(values);
		values += 11,repeat(4,0xff),(0xff+1);
		values += 12,0x01020304,0x0506070809,0xff,0x0a0b0c0d;
		testCodec<G8IU>(values);
		values.clear();
		for(size_t i = 0; i < 10000000; i++) {
			values.push_back(i);
		}
		testCodec<G8IU>(values);
	}

	void benchmarkCodec()
	{
		cout << endl;
		benchmarkCodec<10000000,uint16_t,SU>();
		benchmarkCodec<10000000,uint16_t,G8IU>();
		benchmarkCodec<10000000,uint32_t,SU>();
		benchmarkCodec<10000000,uint32_t,G8IU>();
		benchmarkCodec<10000000,uint64_t,SU>();
		benchmarkCodec<10000000,uint64_t,G8IU>();
	}

	template <size_t SIZE, typename Type, typename Codec>
	void benchmarkCodec()/*{{{*/
	{
		vector<Type> values;
		vector<uint8_t>	buf;
		string title;

		values.resize(SIZE);
		for(size_t i = 0; i < SIZE; i++) {
			values[i] = i;
		}
		buf.resize((sizeof(values[0]) + 1) * values.size());

		uint8_t* bufPtr = &buf[0];
		title = typeid(Codec).name();
		title += ":";
		title += typeid(Type).name();
		title += ":encoding";
		{
			PrintElapsedTime guard(title.c_str());
			encodeWithCnt<Codec>(values, bufPtr);
		}

		if(typeid(Codec) == typeid(G8IU)
				|| typeid(Codec) == typeid(G8IU)) {
			
			title = typeid(Codec).name();
			title += ":";
			title += typeid(Type).name();
			title += ":constructTable";
			{
				PrintElapsedTime guard(title.c_str());
				//Codec::getTable<Type>();
			}
		}

		bufPtr = &buf[0];
		title = typeid(Codec).name();
		title += ":";
		title += typeid(Type).name();
		title += ":decoding";
		{
			PrintElapsedTime guard(title.c_str());
			decodeWithCnt<Codec>(bufPtr, values);
		}
	}/*}}}*/

private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(VarintTest);

// vim:sw=4 ts=4 smarttab autoindent

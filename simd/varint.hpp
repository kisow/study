#ifndef __VARINT_HPP__

#define __VARINT_HPP__

// author : kisow@naver.com

#include "x86simd.hpp"
#include <cstring>
#include <iostream>
#include <iterator>
#include <bitset>

namespace varint {

using namespace std;
using namespace x86simd;

template <typename Type, typename Iterator>
void encodeSU(Iterator& it, Type value)/*{{{*/
{
	do {
		uint8_t byteCode = (uint8_t)(value & 0x7f);
		// signed 음수 값에 대한 방어코드
		// 단, 음수 값에 대해서는 오히려 크기가 더 늘어나는 점에 유의
		value = static_cast<typename make_unsigned<Type>::type>(value) >> 7;
		if(value != 0) byteCode |= 0x80;
		*it = byteCode;
		++it;
	} while(value != 0);
}/*}}}*/

template <typename Type, typename Iterator>
void decodeSU(Iterator& it, Type& value)/*{{{*/
{
	value = 0;
	int shift = 0;
	Type byteCode;
	do {
		byteCode = *it;
		++it;
		value += ((byteCode & 0x7F) << shift);
		shift += 7;
	} while (byteCode & 0x80);
}/*}}}*/

struct SU/*{{{*/
{
	template <typename InIterator, typename OutIterator>
	static void encode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		while(first != last) {
			encodeSU(out, *first);
			++first;
		}
	}/*}}}*/

	template <typename InIterator, typename OutIterator>
	static void decode(InIterator& in, OutIterator first, OutIterator last)/*{{{*/
	{
		while(first != last) {
			decodeSU(in, *first);
			++first;
		}
	}/*}}}*/
};/*}}}*/

template <typename Input, typename Output, typename Entry>
void shuffleAndWrite(Input in[], Output out[], const Entry& entry)/*{{{*/
{
	const size_t MAX_SIZE_OF_8_INT = 64;
	uint8_t buf[MAX_SIZE_OF_8_INT];
	char*	ptr = (char*)&out[0];

	*((__v16qi*)&buf[0]) = __builtin_ia32_loaddqu((char const *)in);
	pshufb(&buf[0], entry.getShuffleSequencePtr(0));
	__builtin_ia32_storedqu(&ptr[0], *(__v16qi*)&buf[0]);
	if(sizeof(Output) >= sizeof(uint32_t)) {
		if(entry.getOutputOffset() > 16) {
			*((__v16qi*)&buf[16]) = __builtin_ia32_loaddqu((char const *)in);
			pshufb(&buf[16], entry.getShuffleSequencePtr(16));
			__builtin_ia32_storedqu(&ptr[16], *(__v16qi*)&buf[16]);
		}
	}
	if(sizeof(Output) >= sizeof(uint64_t)) {
		if(entry.getOutputOffset() > 32) {
			*((__v16qi*)&buf[32]) = __builtin_ia32_loaddqu((char const *)in);
			pshufb(&buf[32], entry.getShuffleSequencePtr(32));
			__builtin_ia32_storedqu(&ptr[32], *(__v16qi*)&buf[32]);
			if(entry.getOutputOffset() > 48) {
				*((__v16qi*)&buf[48]) = __builtin_ia32_loaddqu((char const *)in);
				pshufb(&buf[48], entry.getShuffleSequencePtr(48));
				__builtin_ia32_storedqu(&ptr[48], *(__v16qi*)&buf[48]);
			}
		}
	}
}/*}}}*/

template <typename Entry>
void constructShuffleSequence(Entry& entry, uint8_t desc, uint8_t state)/*{{{*/
{
	const size_t INT_SIZE = Entry::MAX_SHUFFLE_SEQ_SIZE / Entry::MAX_INTCNT_PER_BLOCK;
	size_t j = 0;
	size_t k = 0;
	size_t s = INT_SIZE - state;
	int8_t *seq = &entry.shuffleSequence[0];

	entry.outputOffset = INT_SIZE * Entry::num(desc) - state + Entry::rem(desc);
	for(size_t i = 0; i < Entry::num(desc); i++) {
		if(s < Entry::len(desc, i)) {
			fill(seq, seq + Entry::MAX_SHUFFLE_SEQ_SIZE, -1);
			entry.outputOffset = 0;
			return;
		}
		for(size_t n = 0; n < s; n++) {
			if(n < Entry::len(desc, i)) {
				seq[k++] = j++;
			} else {
				seq[k++] = -1;
			}
		}
		s = INT_SIZE;
	}
	for(size_t n = 0; n < Entry::rem(desc); n++) {
		seq[k++] = j++;
	}
}/*}}}*/

struct G8IU/*{{{*/
{
	template <typename InIterator, typename OutIterator>
	static void encode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		typedef typename std::iterator_traits<InIterator>::value_type Input;
		typedef typename std::iterator_traits<OutIterator>::value_type Output;
		const size_t BUF_MAX = 9;

		Output begin[BUF_MAX];
		Output* it = &begin[1];
		Output* end = &begin[BUF_MAX];
		Output	shift = 0;

		begin[0] = 0xff;
		while(first != last) {
			uint64_t org = static_cast<typename boost::make_unsigned<Input>::type>(*first);
			uint64_t value = org;
			bool uncomplete = false;

			do {
				if(it == end) {
					if(value < org) {
						uncomplete = true;
						break;
					}
					fill(it, end, 0);
					std::copy(begin, end, out);
					out += BUF_MAX;
					it = &begin[1];
					begin[0] = 0xff;
					shift = 0;
				}
				(*it++) = value & 0xff; 
				++shift;
			} while(value >>= 8);
			if(uncomplete) continue;
			begin[0] ^= (1 << (shift - 1));
			++first; 
		}
		std::copy(begin, it, out);
		out += (it - begin);
	}/*}}}*/

	template <typename InIterator, typename OutIterator>
	static void decode(InIterator& in, OutIterator first, OutIterator last)/*{{{*/
	{
		typedef typename std::iterator_traits<OutIterator>::value_type Output;
		const size_t INT_SIZE = sizeof(Output);
		const auto& table = getTable<INT_SIZE>();
		auto out = first;

		while(out != last) {
			const auto& entry = table.entries[*in];
			shuffleAndWrite(&(*++in), &(*out), entry);
			in += entry.getInputOffset();
			out += entry.getOutputOffset() / sizeof(Output);
		}
	}/*}}}*/

	template <size_t INT_SIZE>
	struct Entry/*{{{*/
	{
		static const size_t MAX_INTCNT_PER_BLOCK = 8;
		static const size_t MAX_SHUFFLE_SEQ_SIZE = MAX_INTCNT_PER_BLOCK * INT_SIZE;

		int8_t	shuffleSequence[MAX_SHUFFLE_SEQ_SIZE];
		uint8_t	outputOffset __attribute__ ((aligned(16)));	// 16byte align 되어야 한다.
		uint8_t getInputOffset() const { return 8; }
		uint8_t getOutputOffset() const { return outputOffset; }
		const int8_t* getShuffleSequencePtr(size_t idx) const { return &shuffleSequence[idx]; }

		static size_t len(uint8_t d, size_t i)/*{{{*/
		{
			while(i && d) {
				if(!(d & 1)) --i;
				d >>= 1;
			}

			size_t length = 1;
			while(d & 1) {
				++length;
				d >>= 1;
			}

			return length;
		}/*}}}*/
		static size_t num(uint8_t d) { return 8 - bitset<8>(d).count(); }
		static size_t rem(uint8_t d) { return 0; }
	};/*}}}*/

	template <size_t INT_SIZE>
	struct Table/*{{{*/
	{
		static const size_t DESC_MAX = 256;
		Entry<INT_SIZE>	entries[DESC_MAX];

		Table()
		{
			for(size_t d = 0; d < DESC_MAX; d++) {
				constructShuffleSequence(entries[d], d, 0);
			}
		}
	};/*}}}*/

	template<size_t INT_SIZE>
	static const Table<INT_SIZE>& getTable()/*{{{*/
	{
		static const Table<INT_SIZE> table;

		return table;
	}/*}}}*/
};/*}}}*/

#if 0
struct G8CU/*{{{*/
{
	template <typename InIterator, typename OutIterator>
	static void encode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		typedef typename std::iterator_traits<InIterator>::value_type Input;
		typedef typename std::iterator_traits<OutIterator>::value_type Output;
		const size_t BUF_MAX = 9;

		Output begin[BUF_MAX];
		Output* it = &begin[1];
		Output* end = &begin[BUF_MAX];
		Output	shift = 0;

		begin[0] = 0xff;
		while(first != last) {
			uint64_t org = static_cast<typename boost::make_unsigned<Input>::type>(*first);
			uint64_t value = org;

			do {
				if(it == end) {
					fill(it, end, 0);
					std::copy(begin, end, out);
					out += BUF_MAX;
					it = &begin[1];
					begin[0] = 0xff;
					shift = 0;
				}
				(*it++) = value & 0xff; 
				++shift;
			} while(value >>= 8);
			begin[0] ^= (1 << (shift - 1));
			++first; 
		}
		std::copy(begin, it, out);
		out += (it - begin);
	}/*}}}*/

	template <typename InIterator, typename OutIterator>
	static void decode(InIterator& in, OutIterator first, OutIterator last)/*{{{*/
	{
		typedef typename std::iterator_traits<OutIterator>::value_type Output;
		const size_t INT_SIZE = sizeof(Output);
		const auto& table = getTable<INT_SIZE>();
		auto out = first;
		uint8_t state = 0;

		while(out != last) {
			const auto& entry = table.entries[state][*in];
			shuffleAndWrite(&(*++in), &(*out), entry);
			in += entry.getInputOffset();
			out += entry.getOutputOffset();
			state = entry.getNextState();
		}
	}/*}}}*/

	template <size_t INT_SIZE>
	struct Entry/*{{{*/
	{
		static const size_t MAX_INTCNT_PER_BLOCK = 8;
		static const size_t MAX_SHUFFLE_SEQ_SIZE = MAX_INTCNT_PER_BLOCK * INT_SIZE;

		int8_t	shuffleSequence[MAX_SHUFFLE_SEQ_SIZE];	// 16byte align 되어야 한다.
		uint8_t	outputOffset __attribute__ ((aligned(8)));	
		uint8_t	nextState __attribute__ ((aligned(8)));	
		uint8_t getInputOffset() const { return 8; }
		uint8_t getOutputOffset() const { return outputOffset; }
		uint8_t getNextState() const { return nextState; }
		const int8_t* getShuffleSequencePtr(size_t idx) const { return &shuffleSequence[idx]; }

		static size_t len(uint8_t d, size_t i)/*{{{*/
		{
			while(i && d) {
				if(!(d & 1)) --i;
				d >>= 1;
			}

			size_t length = 1;
			while(d & 1) {
				++length;
				d >>= 1;
			}

			return length;
		}/*}}}*/
		static size_t num(uint8_t d) { return 8 - bitset<8>(d).count(); }
		static size_t rem(uint8_t d) 
		{ 
			size_t i = 1 << 7;
			while((d & i) && i) i >>= 1;
			return 8 - i; 
		}
	};/*}}}*/

	template <size_t INT_SIZE>
	struct Table/*{{{*/
	{
		static const size_t STATE_MAX = 8;
		static const size_t DESC_MAX = 256;
		Entry<INT_SIZE>	entries[STATE_MAX][DESC_MAX];

		Table()
		{
			for(size_t s = 0; s < STATE_MAX; s++) {
				for(size_t d = 0; d < DESC_MAX; d++) {
					constructShuffleSequence(entries[s][d], d, s);
					entries[s][d].nextState = Entry<INT_SIZE>::rem(d);
				}
			}
		}
	};/*}}}*/

	template <size_t INT_SIZE>
	static const Table<INT_SIZE>& getTable()/*{{{*/
	{
		static const Table<INT_SIZE> table;

		return table;
	}/*}}}*/
};/*}}}*/
#endif

template <typename Codec, typename InContainer, typename OutIterator>
void encodeWithCnt(InContainer& in, OutIterator& out)/*{{{*/
{
	encodeSU(out, in.size());
	Codec::encode(in.begin(), in.end(), out);
}/*}}}*/

template <typename Codec, typename Input, typename Output>
void decodeWithCnt(Input*& in, std::vector<Output>& out)/*{{{*/
{
	size_t cnt;
	decodeSU(in, cnt);
	out.resize(cnt + 64);
	Codec::decode(in, out.begin(), out.begin() + (out.size() - 64));
	out.resize(cnt);
}/*}}}*/

};

#endif // end of include guard: __VARINT_HPP__ 

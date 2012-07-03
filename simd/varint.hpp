#ifndef __VARINT_HPP__

#define __VARINT_HPP__

// author : kisow@naver.com

#include "x86simd.hpp"
#include <cstring>
#include <iostream>
#include <iterator>
#include <bitset>
#include <boost/type_traits.hpp>

namespace varint {

using namespace std;
using namespace x86simd;

template <typename Type, typename Iterator>
void encodeSU(Iterator& it, Type value)
{
	do {
		uint8_t byteCode = (uint8_t)(value & 0x7f);
		// signed 음수 값에 대한 방어코드
		// 단, 음수 값에 대해서는 오히려 크기가 더 늘어나는 점에 유의
		value = static_cast<typename boost::make_unsigned<Type>::type>(value) >> 7;
		if(value != 0) byteCode |= 0x80;
		*it = byteCode;
		++it;
	} while(value != 0);
}

template <typename Type, typename Iterator>
void decodeSU(Iterator& it, Type& value)
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
}

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

struct G8IU/*{{{*/
{
	template <typename InIterator, typename OutIterator>
	static void encode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		typedef typename std::iterator_traits<InIterator>::value_type InType;
		typedef typename std::iterator_traits<OutIterator>::value_type OutType;
		const size_t BUF_MAX = 9;

		OutType begin[BUF_MAX];
		OutType* it = &begin[1];
		OutType* end = &begin[BUF_MAX];
		OutType	shift = 0;

		begin[0] = 0xff;
		while(first != last) {
			uint64_t org = static_cast<typename boost::make_unsigned<InType>::type>(*first);
			uint64_t value = org;
			bool uncomplete = false;

			do {
				if(it == end) {
					if(value < org) {
						uncomplete = true;
						break;
					}
					fill(it, end, 0);
					copy(begin, end, out);
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
		copy(begin, it, out);
		out += (it - begin);
	}/*}}}*/

	template <typename InIterator, typename OutIterator>
	static void decode(InIterator& in, OutIterator first, OutIterator last)/*{{{*/
	{
		typedef typename std::iterator_traits<OutIterator>::value_type OutType;
		const size_t INT_SIZE = sizeof(OutType);
		const auto& table = getTable<INT_SIZE>();
		auto out = first;

		while(out != last) {
			const auto& entry = table.getEntry(*in);
			entry.shuffleAndWrite(++in, out);
			in += entry.getInputOffset();
			out += entry.getOutputOffset() / sizeof(OutType);
		}
	}/*}}}*/

	template <size_t INT_SIZE>
	struct Entry/*{{{*/
	{
		static const size_t MAX_INTCNT_PER_BLOCK = 8;
		static const size_t MAX_SHUFFLE_SEQ_SIZE = MAX_INTCNT_PER_BLOCK * INT_SIZE;

		M128	shuffleSequence[MAX_SHUFFLE_SEQ_SIZE / sizeof(M128)];
		uint8_t	outputOffset __attribute__ ((aligned(16)));	// 16byte align 되어야 한다.
		uint8_t getInputOffset() const { return 8; }
		uint8_t getOutputOffset() const { return outputOffset; }

		template <typename InIterator, typename OutIterator>
		void shuffleAndWrite(InIterator& in, OutIterator& out) const
		{
			typedef typename std::iterator_traits<OutIterator>::value_type OutType;
			const size_t MAX_SIZE_OF_8_INT = 64;

			uint8_t buf[MAX_SIZE_OF_8_INT];
			char*	ptr = (char*)&out[0];

			*((__v16qi*)&buf[0]) = __builtin_ia32_loaddqu((char const *)in);
			pshufb(&buf[0], &shuffleSequence[0]);
			__builtin_ia32_storedqu(&ptr[0], *(__v16qi*)&buf[0]);
			if(INT_SIZE >= sizeof(uint32_t)) {
				if(outputOffset > 16) {
					*((__v16qi*)&buf[16]) = __builtin_ia32_loaddqu((char const *)in);
					pshufb(&buf[16], &shuffleSequence[1]);
					__builtin_ia32_storedqu(&ptr[16], *(__v16qi*)&buf[16]);
				}
			}
			if(INT_SIZE >= sizeof(uint64_t)) {
				if(outputOffset > 32) {
					*((__v16qi*)&buf[32]) = __builtin_ia32_loaddqu((char const *)in);
					pshufb(&buf[32], &shuffleSequence[2]);
					__builtin_ia32_storedqu(&ptr[32], *(__v16qi*)&buf[32]);
					if(outputOffset > 48) {
						*((__v16qi*)&buf[48]) = __builtin_ia32_loaddqu((char const *)in);
						pshufb(&buf[48], &shuffleSequence[3]);
						__builtin_ia32_storedqu(&ptr[48], *(__v16qi*)&buf[48]);
					}
				}
			}
		}

		void constructShuffleSequence(uint8_t desc, uint8_t state)
		{
			size_t j = 0;
			size_t k = 0;
			size_t s = INT_SIZE - state;
			int8_t *seq = shuffleSequence[0].begin<int8_t>();

			outputOffset = INT_SIZE * num(desc) - state + rem(desc);
			for(size_t i = 0; i < num(desc); i++) {
				if(s < len(desc, i)) {
					fill(seq, seq + MAX_SHUFFLE_SEQ_SIZE, -1);
					outputOffset = 0;
					return;
				}
				for(size_t n = 0; n < s; n++) {
					if(n < len(desc, i)) {
						seq[k++] = j++;
					} else {
						seq[k++] = -1;
					}
				}
				s = INT_SIZE;
			}
			for(size_t n = 0; n < rem(desc); n++) {
				seq[k++] = j++;
			}
		}

		static size_t num(uint8_t d) { return 8 - bitset<8>(d).count(); }

		static size_t len(uint8_t d, size_t i)
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
		}

		static size_t rem(uint8_t d) { return 0; }
	};/*}}}*/

#if 0
	template <size_t INT_SIZE>
	struct G8CUEntry : public Entry<INT_SIZE>/*{{{*/
	{
		uint8_t nextState;
	};/*}}}*/
#endif

	template <size_t INT_SIZE>
	struct Table/*{{{*/
	{
		static const size_t DESC_MAX = 256;
		Entry<INT_SIZE>	entries[DESC_MAX];

		Table()
		{
			for(size_t d = 0; d < DESC_MAX; d++) {
				entries[d].constructShuffleSequence(d, 0);
			}
		}

		const Entry<INT_SIZE>& getEntry(uint8_t desc) const
		{
			return entries[desc];
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
	void encode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		typedef typename std::iterator_traits<InIterator>::value_type InType;
		typedef typename std::iterator_traits<OutIterator>::value_type OutType;
		const size_t BUF_MAX = 17;

		OutType buf[BUF_MAX];
		OutType* p = &buf[1];
		OutType* pend = &buf[BUF_MAX];

		buf[0] = 0xff;
		while(first != last) {
			InType value = (*first);
			OutType	shift = 0;

			do {
				if(p == pend) {
					copy(buf, buf + BUF_MAX, out);
					out += BUF_MAX;
					p = &buf[1];
					buf[0] = 0xff;
				}
				(*p) = value & 0xff; 
				++p;
				++shift;
			} while(value >>= 8);
			buf[0] ^= (1 << shift);
			shift = 0;
			++first; 
		}
		copy(buf, buf + BUF_MAX, out);
		out += BUF_MAX;
	}/*}}}*/

	template <typename InIterator, typename OutIterator>
	void decode(InIterator first, InIterator last, OutIterator& out)/*{{{*/
	{
		typedef typename std::iterator_traits<OutIterator>::value_type OutType;
		const size_t INT_SIZE = sizeof(OutType);
		const Table<INT_SIZE> & table = getTable<INT_SIZE>();

		M128	buf;
		uint8_t	state = 0;

		while(first != last) {
			const Entry& entry = table.entries[*first][state];
			memcpy(&buf, ++first, entry.getInputOffset());
			pshufb(buf, entry.shuffleSequence);
			std::copy(out, &buf, entry.outputOffset);
			first += entry.getInputOffset();
			out += entry.outputOffset;
		}
	}/*}}}*/

	template <typename InContainer, typename OutIterator>
	void encodeWithCnt(InContainer in, OutIterator& out)/*{{{*/
	{
		encodeSU(out, in.size());
		encode(in.begin(), in.end(), out);
	}/*}}}*/

	template <typename InIterator, typename OutContainer>
	void decodeWithCnt(InIterator in, OutContainer& out)/*{{{*/
	{
		size_t cnt;
		decodeSU(in, cnt);
		out.resize(cnt);
		decode(in.begin(), in.end(), out);
	}/*}}}*/
};/*}}}*/
#endif

template <typename Codec, typename InContainer, typename OutIterator>
void encodeWithCnt(InContainer& in, OutIterator& out)/*{{{*/
{
	encodeSU(out, in.size());
	Codec::encode(in.begin(), in.end(), out);
}/*}}}*/

template <typename Codec, typename InIterator, typename OutContainer>
void decodeWithCnt(InIterator& in, OutContainer& out)/*{{{*/
{
	size_t cnt;
	decodeSU(in, cnt);
	out.resize(cnt + 64);
	Codec::decode(in, out.begin(), out.begin() + (out.size() - 64));
	out.resize(cnt);
}/*}}}*/

};

#endif // end of include guard: __VARINT_HPP__ 

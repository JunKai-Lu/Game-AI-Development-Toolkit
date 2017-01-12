#include "stdafx.h"
#pragma once
#include <string>
#include "gadtlib.h"

namespace gadt
{

	//bit board.
	class BitBoard
	{
	private:
		long long _data;

	public:
		inline BitBoard() :
			_data(0)
		{

		}
		inline BitBoard(long long board) :
			_data(board)
		{
		}

		//equal to the appointed value.
		inline void operator=(long long board)
		{
			_data = board;
		}

		//return whether any bit is true.
		inline bool any() const
		{
			return _data != 0;
		}

		//return whether no any bit is true.
		inline bool none() const
		{
			return _data == 0;
		}

		//set appointed bit to true.
		inline void set(size_t index)
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			long long temp = 1;
			temp = temp << index;
			_data = _data | temp;
		}

		//reset appointed bit.
		inline void reset(size_t index)
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			long long temp = 1;
			temp = ~(temp << index);
			_data = _data & temp;
		}

		//reset all bits.
		inline void reset()
		{
			_data = 0;
		}

		//write value to appointed bit.
		inline void write(size_t index, int value)
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			if (value)
			{
				set(index);
			}
			else
			{
				reset(index);
			}
		}

		//get bit.
		inline bool operator[](size_t index) const
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			return ((_data >> index) & 0x1) == 1;
		}

		//get bit.
		inline bool get(size_t index) const
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			return ((_data >> index) & 0x1) == 1;
		}

		//get value
		inline BitBoard to_ullong() const
		{
			return _data;
		}

		//get string
		inline std::string to_string() const
		{
			char c[64];
			for (size_t i = 64 - 1; i >= 0 && i <= 64; i--)
			{
				if (get(i))
				{
					c[64 - 1 - i] = '1';
				}
				else
				{
					c[64 - 1 - i] = '0';
				}
			}
			return std::string(c);
		}

		inline bool operator<(const BitBoard& target)
		{
			return _data < target._data;
		}
		inline bool operator>(const BitBoard& target)
		{
			return _data > target._data;
		}
		inline bool operator<=(const BitBoard& target)
		{
			return _data <= target._data;
		}
		inline bool operator>=(const BitBoard& target)
		{
			return _data >= target._data;
		}
		inline bool operator==(const BitBoard& target)
		{
			return _data == target._data;
		}
	};

	//bit poker.
	class BitPoker
	{
	private:
		long long _data;
	public:
		inline BitPoker() :
			_data(0)
		{

		}
		inline BitPoker(long long board) :
			_data(board)
		{
		}
		//equal to the appointed value.
		inline void operator=(long long board)
		{
			_data = board;
		}

		//return whether any bit is true.
		inline bool any() const
		{
			return _data != 0;
		}

		//return whether no any bit is true.
		inline bool none() const
		{
			return _data == 0;
		}

		//set appointed bit to true.
		inline void set(size_t index,size_t value)
		{
			GADT_WARNING_CHECK(index >= 16, "out of range.");
			GADT_WARNING_CHECK(value >= 16, "out of value.");
			_data = (_data & (~((long long)0xF << index * 4))) | ((value & 0xF) << (index * 4));
		}

		//reset appointed bit.
		inline void reset(size_t index)
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			long long temp = 15;
			temp = ~(temp << index);
			_data = _data & temp;
		}

		//reset all bits.
		inline void reset()
		{
			_data = 0;
		}

		//get bit.
		inline bool operator[](size_t index) const
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			return (_data >> (index * 4)) & 0xF;
		}

		//get bit.
		inline bool get(size_t index) const
		{
			GADT_WARNING_CHECK(index >= 64, "out of range.");
			return (_data >> (index * 4)) & 0xF;
		}

		//get value
		inline BitBoard to_ullong() const
		{
			return _data;
		}

		//get string
		inline std::string to_string() const
		{
			char c[64];
			for (size_t i = 64 - 1; i >= 0 && i <= 64; i--)
			{
				if (get(i))
				{
					c[64 - 1 - i] = '1';
				}
				else
				{
					c[64 - 1 - i] = '0';
				}
			}
			return std::string(c);
		}

		inline bool operator<(const BitPoker& target)
		{
			return _data < target._data;
		}
		inline bool operator>(const BitPoker& target)
		{
			return _data > target._data;
		}
		inline bool operator<=(const BitPoker& target)
		{
			return _data <= target._data;
		}
		inline bool operator>=(const BitPoker& target)
		{
			return _data >= target._data;
		}
		inline bool operator==(const BitPoker& target)
		{
			return _data == target._data;
		}
	};


	namespace GROUP
	{
		//������ӡ�������Ƿ������
		inline BitGroup Plus(const BitGroup& fir, const BitGroup& sec)
		{
			BitGroup temp = 0;
			for (unsigned int i = 15; i > 0; i--)
			{
				temp |= ((Get(fir, i) + Get(sec, i)) & 0xF);
#ifdef BIT_WARNING
				if ((Get(fir, i) + Get(sec, i)) > 0xF)
				{
					std::cout << ">> WARNING:: function BITGROUP::Plus overflow." << std::endl;
				}
#endif

				temp = temp << 4;
			}
			temp |= ((Get(fir, 0) + Get(sec, 0)) & 0xF);


#ifdef BIT_ALLOW_OVERFLOW
			if ((Get(fir, 0) + Get(sec, 0)) > 0xF)
			{
				temp |= 0xF;
			}
#endif

#ifdef BIT_WARNING
			if ((Get(fir, 0) + Get(sec, 0)) > 0xF)
			{
				std::cout << ">> WARNING:: function BITGROUP::Plus overflow." << std::endl;
			}
#endif

			return temp;
		}

		//BitGroup�ļ���
		inline BitGroup Reduce(const BitGroup& fir, const BitGroup& sec)
		{
			BitGroup temp = 0;
			for (size_t i = 15; i > 0; i--)
			{
				temp |= ((Get(fir, i) - Get(sec, i)) & 0xF);
#ifdef BIT_WARNING
				if ((Get(fir, i) - Get(sec, i)) < 0)
				{
					std::cout << ">> WARNING:: function BITGROUP::Reduce overflow." << std::endl;
				}
#endif
				temp = temp << 4;
			}
			temp |= ((Get(fir, 0) - Get(sec, 0)) & 0xF);
#ifdef BIT_WARNING
			if ((Get(fir, 0) - Get(sec, 0)) < 0)
			{
				std::cout << ">> WARNING:: function BITGROUP::Reduce overflow." << std::endl;
			}
#endif
			return temp;
		}

		//ֱ�������һ�����ڴ������м�ȥ��ʽ
		inline BitGroup DirectReduce(BitGroup fir, BitGroup sec)
		{
#ifdef GADT_WARNING
			for (size_t i = 1; i < CARDS_RANGE; i++)
			{
				if (Get(sec, i) > Get(fir, i))
				{
					std::cout << ">>WARNING : Wrong DirectReduce, fir = " << fir << " ,sec = " << sec << std::endl;
				}
			}

			if (Get(fir, 0) != 0 || Get(sec, 0) != 0)
			{
				std::cout << ">>WARNING : Wrong Zero-Bit in DirectReduce, fir = " << fir << " ,sec = " << sec << std::endl;
			}
#endif
			return fir - sec;
		}

		//BitGroup��ĳһλ����
		inline void Increment(BitGroup& bit_group, size_t index)
		{
#ifdef GADT_WARNING
			if (index == 0)
			{
				std::cout << ">>WARNING: [ GROUP::Set use Zero-Bit ]" << std::endl;
			}
#endif

			BitGroup value = Get(bit_group, index);
#ifdef BIT_WARNING
			if (Get(bit_group, index) == 0xF)
			{
				std::cout << ">> WARNING:: function BITGROUP::Increment overflow." << std::endl;
			}
#endif
			Set(bit_group, index, value + 1);
		}

		//BitGroup��ĳһλ�Լ�
		inline void Decrement(BitGroup& bit_group, size_t index)
		{
#ifdef GADT_WARNING
			if (index == 0)
			{
				std::cout << ">>WARNING: [ GROUP::Set use Zero-Bit ]" << std::endl;
			}
#endif

			BitGroup value = Get(bit_group, index);
#ifdef BIT_WARNING
			if (Get(bit_group, index) == 0)
			{
				std::cout << ">> WARNING:: function BITGROUP::Decrement overflow." << std::endl;
			}
#endif
			Set(bit_group, index, value - 1);
		}

		//��ĳһλ�������λ����
		inline void Push(BitGroup& bit_group, size_t index)
		{
#ifdef GADT_WARNING
			if (index == 0)
			{
				std::cout << ">>WARNING: [ GROUP::Set use Zero-Bit ]" << std::endl;
			}
#endif

			BitGroup value = Get(bit_group, index);
			Set(bit_group, index, value + 1);
		}

		//�ж�һ��λ�����ǲ��ǿյ�
		inline bool IsEmpty(const BitGroup& bit_group)
		{
			return (bit_group == 0);
		}

		//�õ�һ��λ����ϼ�ʣ�µ���
		inline size_t Length(BitGroup bit_group)
		{
			int t = 0;
			bit_group = bit_group >> 4;
			for (size_t i = 1; i < 16; i++)
			{
				t += (bit_group & 0xF);
				bit_group = bit_group >> 4;
			}
			return t;
		}

		//��һ���������Ƴ���һ������
		inline void Remove(BitGroup& bit_group, const BitGroup& target)
		{
			bit_group = Reduce(bit_group, target);
		}

		//��λ�����ӡ�������������֣�
		inline void Print(BitGroup bit_group)
		{
			std::cout << "{ ";
			for (int i = 0; i < 64; i += 4)
			{
				std::cout << ((bit_group >> i) & 0xF) << " ";
			}
			std::cout << "}" << std::endl;
		}

		//��λ�������λ�����
		inline void BitPrint(BitGroup bit_group)
		{
			for (int j = 0; j < 16; j++)
			{
				for (int i = (j * 4) + 3; i >= j * 4; i--)
				{
					std::cout << ((bit_group >> i) & 1);
					//bit_group = bit_group >> 1;
				}
				std::cout << " ";
			}
			std::cout << std::endl;
		}

		//�������ڴ��еı�ʾ��ӡ����
		inline void BitMemoryPrint(BitGroup bit_group)
		{
			for (int i = 63; i >= 0; i--)
			{
				if ((i + 1) % 4 == 0)
				{
					std::cout << " ";
				}
				std::cout << ((bit_group >> i) & 1);



			}
			std::cout << std::endl;
		}

		//�õ��������ַ�����ʽ��û���κοո�
		std::string StringFormat(BitGroup bit_group);

		//ͨ���ַ����õ�һ���µ�λ����
		BitGroup Create(std::string cards_str);

		//�õ��������ַ�����ʾ
		//std::string GetBitGroupStr(BitGroup bit_group);

		//�õ��������ַ�����ʾ��������ʽ��
		//std::string GetBitGroupCardsStr(BitGroup bit_group);

		//ͨ������õ�һ���µ�λ����
		//BitGroup Create(CardVector cards);


	}
}

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"
#include "Runtime/Core/Public/GenericPlatform/GenericPlatformProperties.h"

namespace MemoryStreamConverter
{
	template <class T> 
	void swap(T& a, T& b)
	{
		T c(a); a = b; b = c;
	}

	template<size_t T>
	inline void convert(char *val)
	{
		swap(*val, *(val + T - 1));
		convert<T - 2>(val + 1);
	}

	template<> inline void convert<0>(char *) {}
	template<> inline void convert<1>(char *) {}            // ignore central byte

	template<typename T> inline void apply(T *val)
	{
		convert<sizeof(T)>((char *)(val));
	}

	inline void convert(char *val, size_t size)
	{
		if (size < 2)
			return;

		swap(*val, *(val + size - 1));
		convert(val + 1, size - 2);
	}
}

template<typename T> inline void EndianConvert(T& val) 
{ 
	if(!FGenericPlatformProperties::IsLittleEndian())
		MemoryStreamConverter::apply<T>(&val); 
}

template<typename T> inline void EndianConvertReverse(T&) 
{
	if (FGenericPlatformProperties::IsLittleEndian())
		MemoryStreamConverter::apply<T>(&val);
}

template<typename T> void EndianConvert(T*);         // will generate link error
template<typename T> void EndianConvertReverse(T*);  // will generate link error

inline void EndianConvert(uint8&) { }
inline void EndianConvert(int8&) { }
inline void EndianConvertReverse(uint8&) { }
inline void EndianConvertReverse(int8&) { }

/*
	������������ģ��
	�ܹ���һЩ�����������л�(writeXXX)�ɶ�������ͬʱҲ�ṩ�˷����л�(readXXX)�Ȳ���
*/
class KBENGINEPLUGINS_API MemoryStream
{
public:
	union PackFloatXType
	{
		float	fv;
		uint32	uv;
		int		iv;
	};

public:
	const static size_t DEFAULT_SIZE = 0x100;

	MemoryStream() :
		rpos_(0), 
		wpos_(0),
		data_()
	{
		data_resize(DEFAULT_SIZE);
	}

	~MemoryStream()
	{
		clear(false);
	}

	static MemoryStream* createObject();
	static void reclaimObject(MemoryStream* obj);

public:
	uint8* data() {
		return data_.GetData();
	}

	void clear(bool clearData)
	{
		if (clearData)
			data_.Empty();

		rpos_ = wpos_ = 0;
	}

	// array�Ĵ�С
	virtual uint32 size() const { return data_.Num(); }

	// array�Ƿ�Ϊ��
	virtual bool empty() const { return data_.Num() == 0; }

	// ����������д����֮��ĳ���
	virtual uint32 length() const { return rpos() >= wpos() ? 0 : wpos() - rpos(); }

	// ʣ������Ĵ�С
	virtual uint32 space() const { return wpos() >= size() ? 0 : size() - wpos(); }

	// ��������ǿ�����õ�д��������ʾ��������
	void done() { read_skip(length()); }

	void data_resize(uint32 newsize)
	{
		KBE_ASSERT(newsize <= 1310700);
		data_.SetNumUninitialized(newsize);
	}

	void resize(uint32 newsize)
	{
		KBE_ASSERT(newsize <= 1310700);
		data_.SetNumUninitialized(newsize);
		rpos_ = 0;
		wpos_ = size();
	}

	void reserve(uint32 ressize)
	{
		KBE_ASSERT(ressize <= 1310700);

		if (ressize > size())
			data_.Reserve(ressize);
	}

	uint32 rpos() const { return rpos_; }

	uint32 rpos(int rpos)
	{
		if (rpos < 0)
			rpos = 0;

		rpos_ = rpos;
		return rpos_;
	}

	uint32 wpos() const { return wpos_; }

	uint32 wpos(int wpos)
	{
		if (wpos < 0)
			wpos = 0;

		wpos_ = wpos;
		return wpos_;
	}

	uint8 operator[](uint32 pos)
	{
		return read<uint8>(pos);
	}

	MemoryStream &operator<<(uint8 value)
	{
		append<uint8>(value);
		return *this;
	}

	MemoryStream &operator<<(uint16 value)
	{
		append<uint16>(value);
		return *this;
	}

	MemoryStream &operator<<(uint32 value)
	{
		append<uint32>(value);
		return *this;
	}

	MemoryStream &operator<<(uint64 value)
	{
		append<uint64>(value);
		return *this;
	}

	MemoryStream &operator<<(int8 value)
	{
		append<int8>(value);
		return *this;
	}

	MemoryStream &operator<<(int16 value)
	{
		append<int16>(value);
		return *this;
	}

	MemoryStream &operator<<(int32 value)
	{
		append<int32>(value);
		return *this;
	}

	MemoryStream &operator<<(int64 value)
	{
		append<int64>(value);
		return *this;
	}

	MemoryStream &operator<<(float value)
	{
		append<float>(value);
		return *this;
	}

	MemoryStream &operator<<(double value)
	{
		append<double>(value);
		return *this;
	}

	MemoryStream &operator<<(const FString &value)
	{
		const TCHAR *serializedChar = value.GetCharArray().GetData();
		uint32 size = FCString::Strlen(serializedChar);

		append(((uint8*)TCHAR_TO_ANSI(serializedChar)), size);
		append((uint8)0);
		return *this;
	}

	MemoryStream &operator<<(const char *str)
	{
		append((uint8 const *)str, str ? strlen(str) : 0);
		append((uint8)0);
		return *this;
	}

	MemoryStream &operator<<(bool value)
	{
		append<int8>(value);
		return *this;
	}

	MemoryStream &operator>>(bool &value)
	{
		value = read<char>() > 0 ? true : false;
		return *this;
	}

	MemoryStream &operator>>(uint8 &value)
	{
		value = read<uint8>();
		return *this;
	}

	MemoryStream &operator>>(uint16 &value)
	{
		value = read<uint16>();
		return *this;
	}

	MemoryStream &operator>>(uint32 &value)
	{
		value = read<uint32>();
		return *this;
	}

	MemoryStream &operator>>(uint64 &value)
	{
		value = read<uint64>();
		return *this;
	}

	MemoryStream &operator>>(int8 &value)
	{
		value = read<int8>();
		return *this;
	}

	MemoryStream &operator>>(int16 &value)
	{
		value = read<int16>();
		return *this;
	}

	MemoryStream &operator>>(int32 &value)
	{
		value = read<int32>();
		return *this;
	}

	MemoryStream &operator>>(int64 &value)
	{
		value = read<int64>();
		return *this;
	}

	MemoryStream &operator>>(float &value)
	{
		value = read<float>();
		return *this;
	}

	MemoryStream &operator>>(double &value)
	{
		value = read<double>();
		return *this;
	}

	MemoryStream &operator>>(FString& value)
	{
		char cc[2];
		cc[0] = 0;
		cc[1] = 0;

		value = TEXT("");
		while (length() > 0)
		{
			cc[0] = read<char>();
			if (cc[0] == 0 || !isascii(cc[0]))
				break;

			value += ANSI_TO_TCHAR((const ANSICHAR*)&cc);
		}

		return *this;
	}

	MemoryStream &operator>>(char *value)
	{
		while (length() > 0)
		{
			char c = read<char>();
			if (c == 0 || !isascii(c))
				break;

			*(value++) = c;
		}

		*value = '\0';
		return *this;
	}

	template<typename T>
	void read_skip() { read_skip(sizeof(T)); }

	void read_skip(uint32 skip)
	{
		check(skip <= length());
		rpos_ += skip;
	}

	template <typename T> T read()
	{
		T r = read<T>(rpos_);
		rpos_ += sizeof(T);
		return r;
	}

	template <typename T> T read(uint32 pos) 
	{
		check(sizeof(T) <= length());

		T val = *((T *)(data() + pos));
		EndianConvert(val);
		return val;
	}

	void read(uint8 *dest, uint32 len)
	{
		check(len <= length());

		memcpy(dest, data() + rpos_, len);
		rpos_ += len;
	}

	uint32 readBlob(TArray<uint8>& datas)
	{
		if (length() <= 0)
			return 0;

		uint32 rsize = 0;
		(*this) >> rsize;
		if ((uint32)rsize > length())
			return 0;

		if (rsize > 0)
		{
			datas.SetNumUninitialized(rsize);
			memcpy(datas.GetData(), data() + rpos(), rsize);
			read_skip(rsize);
		}

		return rsize;
	}

	void readPackXYZ(float& x, float&y, float& z, float minf = -256.f)
	{
		uint32 packed = 0;
		(*this) >> packed;
		x = ((packed & 0x7FF) << 21 >> 21) * 0.25f;
		z = ((((packed >> 11) & 0x7FF) << 21) >> 21) * 0.25f;
		y = ((packed >> 22 << 22) >> 22) * 0.25f;

		x += minf;
		y += minf / 2.f;
		z += minf;
	}

	void readPackXZ(float& x, float& z)
	{
		PackFloatXType & xPackData = (PackFloatXType&)x;
		PackFloatXType & zPackData = (PackFloatXType&)z;

		// 0x40000000 = 1000000000000000000000000000000.
		xPackData.uv = 0x40000000;
		zPackData.uv = 0x40000000;

		uint8 tv;
		uint32 data = 0;

		(*this) >> tv;
		data |= (tv << 16);

		(*this) >> tv;
		data |= (tv << 8);

		(*this) >> tv;
		data |= tv;

		// ����ָ����β��
		xPackData.uv |= (data & 0x7ff000) << 3;
		zPackData.uv |= (data & 0x0007ff) << 15;

		xPackData.fv -= 2.0f;
		zPackData.fv -= 2.0f;

		// ���ñ��λ
		xPackData.uv |= (data & 0x800000) << 8;
		zPackData.uv |= (data & 0x000800) << 20;
	}

	void readPackY(float& y)
	{
		PackFloatXType yPackData;
		yPackData.uv = 0x40000000;

		uint16 data = 0;
		(*this) >> data;
		yPackData.uv |= (data & 0x7fff) << 12;
		yPackData.fv -= 2.f;
		yPackData.uv |= (data & 0x8000) << 16;
		y = yPackData.fv;
	}

	template <typename T> void append(T value)
	{
		EndianConvert(value);
		append((uint8 *)&value, sizeof(value));
	}

	template<class T> void append(const T *src, uint32 cnt)
	{
		return append((const uint8 *)src, cnt * sizeof(T));
	}

	void append(const uint8 *src, uint32 cnt)
	{
		if (!cnt)
			return;

		KBE_ASSERT(size() < 10000000);

		if (size() < wpos_ + cnt)
			data_resize(wpos_ + cnt);

		memcpy((void*)&data()[wpos_], src, cnt);
		wpos_ += cnt;
	}

	void append(const uint8* datas, uint32 offset, uint32 size)
	{
		append(datas + offset, size);
	}

	void appendBlob(const TArray<uint8>& datas)
	{
		uint32 len = (uint32)datas.Num();
		(*this) << len;

		if (len > 0)
			append(datas.GetData(), len);
	}

	void appendPackAnyXYZ(float x, float y, float z, const float epsilon = 0.5f)
	{
		if (epsilon > 0.f)
		{
			x = floorf(x + epsilon);
			y = floorf(y + epsilon);
			z = floorf(z + epsilon);
		}

		*this << x << y << z;
	}

	void appendPackAnyXZ(float x, float z, const float epsilon = 0.5f)
	{
		if (epsilon > 0.f)
		{
			x = floorf(x + epsilon);
			z = floorf(z + epsilon);
		}

		*this << x << z;
	}

	void appendPackXYZ(float x, float y, float z, float minf = -256.f)
	{
		x -= minf;
		y -= minf / 2.f;
		z -= minf;

		// ���ֵ��Ҫ����-256~256
		// y ��Ҫ����-128~128
		uint32 packed = 0;
		packed |= ((int)(x / 0.25f) & 0x7FF);
		packed |= ((int)(z / 0.25f) & 0x7FF) << 11;
		packed |= ((int)(y / 0.25f) & 0x3FF) << 22;
		*this << packed;
	}

	void appendPackXZ(float x, float z)
	{
		PackFloatXType xPackData;
		xPackData.fv = x;

		PackFloatXType zPackData;
		zPackData.fv = z;

		// 0-7λ���β��, 8-10λ���ָ��, 11λ��ű�־
		// ����ʹ����24λ���洢2��float�� ����Ҫ���ܹ��ﵽ-512~512֮�����
		// 8λβ��ֻ�ܷ����ֵ256, ָ��ֻ��3λ(�������������ֵΪ2^(2^3)=256) 
		// ������ȥ��һλʹ��Χ�ﵽ(-512~-2), (2~512)֮��
		// ����������Ǳ�֤��С��Ϊ-2.f����2.f
		xPackData.fv += xPackData.iv < 0 ? -2.f : 2.f;
		zPackData.fv += zPackData.iv < 0 ? -2.f : 2.f;

		uint32 data = 0;

		// 0x7ff000 = 11111111111000000000000
		// 0x0007ff = 00000000000011111111111
		const uint32 xCeilingValues[] = { 0, 0x7ff000 };
		const uint32 zCeilingValues[] = { 0, 0x0007ff };

		// ��������������������������ø�����Ϊ�����
		// ��������ָ����4λ�ͱ��λ�� �������λ��Ϊ0��϶������ �����4λ��8λβ����Ϊ0�����
		// 0x7c000000 = 1111100000000000000000000000000
		// 0x40000000 = 1000000000000000000000000000000
		// 0x3ffc000  = 0000011111111111100000000000000
		data |= xCeilingValues[((xPackData.uv & 0x7c000000) != 0x40000000) || ((xPackData.uv & 0x3ffc000) == 0x3ffc000)];
		data |= zCeilingValues[((zPackData.uv & 0x7c000000) != 0x40000000) || ((zPackData.uv & 0x3ffc000) == 0x3ffc000)];

		// ����8λβ����3λָ���� ���������ʣ��β�����λ��1��+1��������, ���Ҵ�ŵ�data��
		// 0x7ff000 = 11111111111000000000000
		// 0x0007ff = 00000000000011111111111
		// 0x4000	= 00000000100000000000000
		data |= ((xPackData.uv >> 3) & 0x7ff000) + ((xPackData.uv & 0x4000) >> 2);
		data |= ((zPackData.uv >> 15) & 0x0007ff) + ((zPackData.uv & 0x4000) >> 14);

		// ȷ��ֵ�ڷ�Χ��
		// 0x7ff7ff = 11111111111011111111111
		data &= 0x7ff7ff;

		// ���Ʊ��λ
		// 0x800000 = 100000000000000000000000
		// 0x000800 = 000000000000100000000000
		data |= (xPackData.uv >> 8) & 0x800000;
		data |= (zPackData.uv >> 20) & 0x000800;

		uint8 packs[3];
		packs[0] = (uint8)(data >> 16);
		packs[1] = (uint8)(data >> 8);
		packs[2] = (uint8)data;
		(*this).append(packs, 3);
	}

	void appendPackY(float y)
	{
		PackFloatXType yPackData;
		yPackData.fv = y;

		yPackData.fv += yPackData.iv < 0 ? -2.f : 2.f;
		uint16 data = 0;
		data = (yPackData.uv >> 12) & 0x7fff;
		data |= ((yPackData.uv >> 16) & 0x8000);

		(*this) << data;
	}

	/** ��������� */
	void print_storage();

protected:
	uint32 rpos_;
	uint32 wpos_;
	TArray<uint8> data_;
};

template<>
inline void MemoryStream::read_skip<char*>()
{
	uint8 temp = 1;
	while (temp != 0)
		temp = read<uint8>();
}

template<>
inline void MemoryStream::read_skip<char const*>()
{
	read_skip<char*>();
}

template<>
inline void MemoryStream::read_skip<FString>()
{
	read_skip<char*>();
}

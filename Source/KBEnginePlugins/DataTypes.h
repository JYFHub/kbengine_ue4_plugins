// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class MemoryStream;

/*
	entitydef��֧�ֵĻ�����������
	��ģ���е������������е�֧�����Ͳ��ṩ����Щ���͵��������л��ɶ����������뷴���л���������Ҫ��������ͨѶ�Ĵ��������
*/
class KBENGINEPLUGINS_API KBEDATATYPE_BASE
{
public:
		virtual void bind()
		{
		}

		/*
		static bool isNumeric(object v)
		{
		return v is sbyte || v is byte ||
		v is short || v is ushort ||
		v is int || v is uint ||
		v is long || v is ulong ||
		v is char || v is decimal || v is float ||
		v is double || v is Int16 || v is Int64 ||
		v is UInt16 || v is UInt64 ||
		v is Boolean || v is bool;
		}

		virtual object createFromStream(MemoryStream stream)
		{
			return null;
		}
		
		virtual void addToStream(Bundle stream, object v)
		{
		}
		
		virtual object parseDefaultValStr(string v)
		{
			return null;
		}
		
		virtual bool isSameType(object v)
		{
			return v == null;
		}
		*/
protected:

};

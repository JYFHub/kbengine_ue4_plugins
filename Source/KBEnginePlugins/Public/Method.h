// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once
#include "KBVar.h"
#include "KBECommon.h"

class KBEDATATYPE_BASE;

/*
	ʵ�嶨��ķ���ģ��
	�����һ��def�ļ��ж���ķ�������ģ�����ṩ�˸÷��������������Ϣ
	���磺�����Ĳ�����������id��������Ӧ�ű���handler
*/
class KBENGINEPLUGINS_API Method
{
public:
	Method();
	virtual ~Method();

public:
	FString name;
	uint16 methodUtype;
	int16 aliasID;

	TArray<KBEDATATYPE_BASE*> args;
	// System.Reflection.MethodInfo handler = null;
};

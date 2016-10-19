// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

/*
��Ϣģ��
�ͻ��������˽���������ϢͨѶ�� �κ�һ����Ϊһ��ָ�����һ����Ϣ��������
*/
class KBENGINEPLUGINS_API Message
{
public:
	Message();
	virtual ~Message();

public:
	

public:
	MessageID id;
	FString name;
	int16 msglen;
	//public System.Reflection.MethodInfo handler = null;
	//public KBEDATATYPE_BASE[] argtypes = null;
	uint8 argsType;
};

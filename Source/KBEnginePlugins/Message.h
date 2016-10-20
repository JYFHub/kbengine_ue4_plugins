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
	Message(MessageID mid, const FString& mname, int16 mmsglen, uint8 margsType);
	virtual ~Message();

public:
	static Message* getMessage(const FString& messageName);
	static void setMessage(const FString& messageName, Message* pMessage);

	static void clear();
	static void bindFixedMessage();

public:
	MessageID id;
	FString name;
	int16 msglen;
	//public System.Reflection.MethodInfo handler = null;
	//public KBEDATATYPE_BASE[] argtypes = null;
	uint8 argsType;

	static TMap<MessageID, Message*> loginappMessages;
	static TMap<MessageID, Message*> baseappMessages;
	static TMap<MessageID, Message*> clientMessages;

	static TMap<FString, Message*> messages;
};

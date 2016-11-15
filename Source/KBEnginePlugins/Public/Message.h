// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class Channel;
class MemoryStream;

/** һ����Ϣ�Ĳ��������� */
class KBENGINEPLUGINS_API MessageArgs
{
public:
	enum MESSAGE_ARGS_TYPE
	{
		MESSAGE_ARGS_TYPE_VARIABLE = -1,		// �ɱ��������
		MESSAGE_ARGS_TYPE_FIXED = 0				// �̶���������
	};

	MessageArgs() :strArgsTypes() {};
	virtual ~MessageArgs() {};
	virtual void createFromStream(MemoryStream& s) = 0;
	virtual void addToStream(MemoryStream& s) = 0;
	virtual int32 dataSize(void) = 0;
	virtual MessageArgs::MESSAGE_ARGS_TYPE type(void) { return MESSAGE_ARGS_TYPE_FIXED; }

	TArray<FString> strArgsTypes;
};

/*
��Ϣģ��
�ͻ��������˽���������ϢͨѶ�� �κ�һ����Ϊһ��ָ�����һ����Ϣ��������
*/
class KBENGINEPLUGINS_API Message
{
public:
	Message();
	Message(MessageID mid, const FString& mname, int16 mmsglen);
	virtual ~Message();

public:

	FString c_str();

	virtual void handle(MemoryStream& s);

public:
	MessageID id;
	FString name;
	int16 msglen;
};

class KBENGINEPLUGINS_API Messages
{
public:
	Messages();
	virtual ~Messages();

	static Messages& getSingleton();

public:
	void clear();
	void bindFixedMessage();


	Message* add(Message* pMessage, MessageID mid, const FString& mname, int16 mmsglen);
	Message* findClientMessage(MessageID mid);
	Message* findMessage(const FString& mname);

public:
	TMap<MessageID, Message*> loginappMessages;
	TMap<MessageID, Message*> baseappMessages;
	TMap<int, Message*> clientMessages;

	TMap<FString, Message*> messages;

	TMap<FString, MessageID> fixedMessageID;
};
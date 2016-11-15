// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"


/*
	��Ϣ�Ķ�ģ��
	�����ݰ����з��������е���Ϣ�������佻����Ӧ����Ϣ������
*/
class KBENGINEPLUGINS_API MessageReader
{
public:
	MessageReader();
	virtual ~MessageReader();

	enum READ_STATE
	{
		// ��ϢID
		READ_STATE_MSGID = 0,

		// ��Ϣ�ĳ���65535����
		READ_STATE_MSGLEN = 1,

		// ���������Ϣ���ȶ��޷�����Ҫ��ʱʹ����չ����
		// uint32
		READ_STATE_MSGLEN_EX = 2,

		// ��Ϣ������
		READ_STATE_BODY = 3
	};

public:
	void process(const uint8* datas, MessageLengthEx offset, MessageLengthEx length);

protected:
	MessageID msgid_;
	MessageLength msglen_;
	MessageLengthEx expectSize_;
	READ_STATE state_;
	MemoryStream* pMemoryStream_;

};

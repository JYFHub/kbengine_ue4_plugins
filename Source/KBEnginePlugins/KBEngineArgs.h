// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"


/*
��ʼ��KBEngine�Ĳ�����
*/
class KBENGINEPLUGINS_API KBEngineArgs
{
public:
	KBEngineArgs();
	virtual ~KBEngineArgs();

	int getRecvBufferSize();
	int getSendBufferSize();

public:
	FString ip;
	int port;

	// �ͻ�������
	// Reference: http://www.kbengine.org/docs/programming/clientsdkprogramming.html, client types
	EKCLIENT_TYPE clientType;

	bool syncPlayer;
	bool useAliasEntityID;
	bool isOnInitCallPropertysSetMethods;

	// ���ͻ����С
	MessageLengthEx SEND_BUFFER_MAX;

	// ���ջ�������С
	MessageLengthEx RECV_BUFFER_MAX;
};

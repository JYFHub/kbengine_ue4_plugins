// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class MemoryStream;
class NetworkInterface;
class Message;

/*
	���ģ�齫������ݰ�������һ��
	����ÿ�����ݰ�����������ޣ� ��Bundle��д��������ݽ������ڲ��������MemoryStream
	��sendʱ��ȫ�����ͳ�ȥ
*/
class KBENGINEPLUGINS_API Bundle
{
public:
	Bundle();
	virtual ~Bundle();

public:
	static Bundle* createObject();
	static void reclaimObject(Bundle* obj);

	void newMessage(Message* pMsg);
	void fini(bool issend);

	void writeMsgLength();
	void send(NetworkInterface* pNetworkInterface);

	void checkStream(uint32 v);

public:

protected:
	MemoryStream* pCurrStream_;
	TArray<MemoryStream*> streams_;

	int numMessage_;
	int messageLength_;
	Message* pMsgtype_;
	int curMsgStreamIndex_;
};

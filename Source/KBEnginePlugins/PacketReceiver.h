// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class NetworkInterface;
class MessageReader;

/*
	������ģ��(���������粿�ֵ����ƶ�Ӧ)
	�����������ݵĽ���
*/
class KBENGINEPLUGINS_API PacketReceiver
{
public:
	PacketReceiver(NetworkInterface* pNetworkInterface);
	virtual ~PacketReceiver();

public:
	void process();

protected:
	NetworkInterface* pNetworkInterface_;
	MessageReader* pMessageReader_;
	MemoryStream* pBuffer_;
};

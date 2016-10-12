// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class NetworkInterface;

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
};

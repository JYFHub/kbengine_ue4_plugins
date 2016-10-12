// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class NetworkInterface;

/*
	������ģ��(���������粿�ֵ����ƶ�Ӧ)
	�����������ݵķ���
*/
class KBENGINEPLUGINS_API PacketSender
{
public:
	PacketSender(NetworkInterface* pNetworkInterface);
	virtual ~PacketSender();

public:
	bool send(FString datas);

protected:
	NetworkInterface* pNetworkInterface_;
};
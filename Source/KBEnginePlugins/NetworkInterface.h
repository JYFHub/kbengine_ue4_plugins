// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

#include "Runtime/Sockets/Public/SocketSubsystem.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"
#include "Runtime/Networking/Public/Networking.h"
#include "Runtime/Sockets/Public/Sockets.h"

class PacketSender;
class PacketReceiver;
class MemoryStream;

/*
	����ģ��
	�������ӡ��շ�����
*/
class KBENGINEPLUGINS_API NetworkInterface
{
public:
	NetworkInterface();
	virtual ~NetworkInterface();

public:
	FSocket* socket() {
		return socket_;
	}

	void process();

	void reset();
	void close();
	bool valid();

	bool connectTo(FString ip, uint16 port);
	bool send(MemoryStream* pMemoryStream);

private:
	bool TickConnecting(float InDeltaTime);

protected:
	FSocket* socket_;
	PacketSender* pPacketSender_;
	PacketReceiver* pPacketReceiver_;

	FDelegateHandle TickConnectingDelegateHandle;
};

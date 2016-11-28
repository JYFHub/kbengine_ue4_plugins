// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

/*
�ӿ�ģ��
����������ʵ��ĳЩ�ص�ͳһ�ӿ�
*/
class KBENGINEPLUGINS_API InterfaceConnect
{
public:

	virtual void onConnectCallback(FString ip, uint16 port, bool success, int userdata) = 0;

};


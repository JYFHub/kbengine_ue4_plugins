// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"


/*
����KBEngine����ĺ���ģ��
�������紴�����־û�Э�顢entities�Ĺ����Լ��������ɵ��ýӿڡ�

һЩ���Բο��ĵط�:
http://www.kbengine.org/docs/programming/clientsdkprogramming.html
http://www.kbengine.org/docs/programming/kbe_message_format.html

http://www.kbengine.org/cn/docs/programming/clientsdkprogramming.html
http://www.kbengine.org/cn/docs/programming/kbe_message_format.html
*/
class KBEngineApp
{
public:
	KBEngineApp();
	virtual ~KBEngineApp();

public:
	FString username;
	FString password;

	// ����˷����baseapp��ַ
	FString baseappIP;
	uint16 baseappPort;

	// �������ͻ��˵İ汾���Լ�Э��MD5
	FString serverVersion;
	FString clientVersion;
	FString serverScriptVersion;
	FString clientScriptVersion;
	FString serverProtocolMD5;
	FString serverEntitydefMD5;

	// ��ǰ��ҵ�ʵ��id��ʵ�����
	uint64 entity_uuid;
	int32 entity_id;
	FString entity_type;

	// ��ҵ�ǰ���ڿռ��id�� �Լ��ռ��Ӧ����Դ
	uint32 spaceID;
	FString spaceResPath;
	bool isLoadedGeometry;

	// ���ձ�׼��ÿ���ͻ��˲��ֶ�Ӧ�ð����������
	FString component;

protected:

};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class KBEngineArgs;
class Entity;
class NetworkInterface;
class MemoryStream;

/*
����KBEngine����ĺ���ģ��
�������紴�����־û�Э�顢entities�Ĺ����Լ��������ɵ��ýӿڡ�

һЩ���Բο��ĵط�:
http://www.kbengine.org/docs/programming/clientsdkprogramming.html
http://www.kbengine.org/docs/programming/kbe_message_format.html

http://www.kbengine.org/cn/docs/programming/clientsdkprogramming.html
http://www.kbengine.org/cn/docs/programming/kbe_message_format.html
*/
class KBENGINEPLUGINS_API KBEngineApp : public InterfaceLogin
{
public:
	KBEngineApp();
	KBEngineApp(KBEngineArgs* pArgs);
	virtual ~KBEngineApp();
	
public:
	bool initialize(KBEngineArgs* pArgs);
	void destroy();
	void reset();

	NetworkInterface* pNetworkInterface() const {
		return pNetworkInterface_;
	}

	KBEngineArgs* getInitArgs() const
	{
		return pArgs_;
	}

	void resetMessages();

	/**
		�������ѭ��������
	*/
	void process();

	/*
		�����˷��������Լ�ͬ����ɫ��Ϣ�������
	*/
	void sendTick();

	/**
		��¼������ˣ������¼���loginapp������(baseapp)����¼���̲������
	*/
	bool login(FString& username, FString& password, TArray<uint8>& datas);
	virtual void onLoginCallback(FString ip, uint16 port, bool success, int userdata) override;

private:
	bool initNetwork();

	void hello();
	void Client_onHelloCB(MemoryStream& stream);

	void login_loginapp(bool noconnect);
	void onConnectTo_loginapp_callback(FString ip, uint16 port, bool success);
	void onLogin_loginapp();

	void login_baseapp(bool noconnect);
	void onConnectTo_baseapp_callback(FString ip, uint16 port, bool success);
	void onLogin_baseapp();


	void clearSpace(bool isall);
	void clearEntities(bool isall);


	void updatePlayerToServer();

	void onServerDigest();

protected:
	KBEngineArgs* pArgs_;
	NetworkInterface* pNetworkInterface_;

	FString username_;
	FString password_;

	// �Ƿ����ڼ��ر�����ϢЭ��
	static bool loadingLocalMessages_;

	// ��ϢЭ���Ƿ��Ѿ�������
	static bool loginappMessageImported_;
	static bool baseappMessageImported_;
	static bool entitydefImported_;
	static bool isImportServerErrorsDescr_;

	// ����˷����baseapp��ַ
	FString baseappIP_;
	uint16 baseappPort_;

	// ��ǰ״̬
	FString currserver_;
	FString currstate_;

	// ����������Լ��ͻ����������ڵ�¼ʱ������˺Ű󶨵Ķ�������Ϣ
	// ����Ϣ���û��Լ�������չ
	TArray<uint8> serverdatas_;
	TArray<uint8> clientdatas_;

	// ͨ��Э����ܣ�blowfishЭ��
	TArray<uint8> encryptedKey_;

	// �������ͻ��˵İ汾���Լ�Э��MD5
	FString serverVersion_;
	FString clientVersion_;
	FString serverScriptVersion_;
	FString clientScriptVersion_;
	FString serverProtocolMD5_;
	FString serverEntitydefMD5_;

	// ��ǰ��ҵ�ʵ��id��ʵ�����
	uint64 entity_uuid_;
	int32 entity_id_;
	FString entity_type_;

	// space�����ݣ����忴API�ֲ����spaceData
	// https://github.com/kbengine/kbengine/tree/master/docs/api
	TMap<FString, FString> spacedatas_;
	
	// ����ʵ�嶼��������� ��ο�API�ֲ����entities����
	// https://github.com/kbengine/kbengine/tree/master/docs/api
	TMap<int32, Entity*> entities_;

	// ���з���˴������Ӧ�Ĵ�������
	static TMap<uint16, FKServerErr> serverErrs_;

	double lastTickTime_;
	double lastTickCBTime_;
	double lastUpdateToServerTime_;

	// ��ҵ�ǰ���ڿռ��id�� �Լ��ռ��Ӧ����Դ
	uint32 spaceID_;
	FString spaceResPath_;
	bool isLoadedGeometry_;

	// ���ձ�׼��ÿ���ͻ��˲��ֶ�Ӧ�ð����������
	FString component_;

};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class KBEngineArgs;
class Entity;
class NetworkInterface;
class MemoryStream;
class PersistentInfos;

/*
	����KBEngine����ĺ���ģ��
	�������紴�����־û�Э�顢entities�Ĺ����Լ��������ɵ��ýӿڡ�

	һЩ���Բο��ĵط�:
	http://www.kbengine.org/docs/programming/clientsdkprogramming.html
	http://www.kbengine.org/docs/programming/kbe_message_format.html
*/
class KBENGINEPLUGINS_API KBEngineApp : public InterfaceLogin
{
public:
	KBEngineApp();
	KBEngineApp(KBEngineArgs* pArgs);
	virtual ~KBEngineApp();
	
public:
	static KBEngineApp& getSingleton();

public:
	bool isInitialized() const {
		return pArgs_ != NULL;
	}

	bool initialize(KBEngineArgs* pArgs);
	void destroy();
	void reset();

	NetworkInterface* pNetworkInterface() const {
		return pNetworkInterface_;
	}

	const TArray<uint8>& serverdatas() const
	{
		return serverdatas_;
	}

	KBEngineArgs* getInitArgs() const
	{
		return pArgs_;
	}

	void installEvents();

	void resetMessages();

	static bool validEmail(FString strEmail);

	/*
		ͨ������id�õ���������
	*/
	FString serverErr(uint16 id);

	Entity* player();
	Entity* findEntity(int32 entityID);

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

	/*
	�ص�¼������(baseapp)
	һЩ�ƶ���Ӧ�����׵��ߣ�����ʹ�øù��ܿ��ٵ����������˽���ͨ��
	*/
	void reLoginBaseapp();

	/*
		��¼loginappʧ����
	*/
	void Client_onLoginFailed(MemoryStream& stream);

	/*
		��¼loginapp�ɹ���
	*/
	void Client_onLoginSuccessfully(MemoryStream& stream);

	/*
		��¼baseappʧ����
	*/
	void Client_onLoginBaseappFailed(uint16 failedcode);

	/*
		�ص�¼baseappʧ����
	*/
	void Client_onReLoginBaseappFailed(uint16 failedcode);

	/*
		��¼baseapp�ɹ���
	*/
	void Client_onReLoginBaseappSuccessfully(MemoryStream& stream);

	void hello();
	void Client_onHelloCB(MemoryStream& stream);

	void Client_onVersionNotMatch(MemoryStream& stream);
	void Client_onScriptVersionNotMatch(MemoryStream& stream);

	/*
		��������߳�
	*/
	void Client_onKicked(uint16 failedcode);

	/*
	�ӷ���˷��صĶ�����������ͻ�����ϢЭ��
	*/
	void Client_onImportClientMessages(MemoryStream& stream);

	/*
		����˴�������������
	*/
	void Client_onImportServerErrorsDescr(MemoryStream& stream);

	void Client_onImportClientEntityDef(MemoryStream& stream);

	/*
		�����������ص�
	*/
	void Client_onAppActiveTickCB();

	/*
		�����֪ͨ����һ����ɫ
	*/
	void Client_onCreatedProxies(uint64 rndUUID, int32 eid, FString& entityType);

	/*
		�����֪ͨǿ������һ��ʵ��
	*/
	void Client_onEntityDestroyed(int32 eid);

	/*
		�����ʹ���Ż��ķ�ʽ����ʵ����������
	*/
	void Client_onUpdatePropertysOptimized(MemoryStream& stream);

	/*
		����˸���ʵ����������
	*/
	void Client_onUpdatePropertys(MemoryStream& stream);

	/*
		����˸���ʵ����������
	*/
	void onUpdatePropertys_(ENTITY_ID eid, MemoryStream& stream);

private:
	bool initNetwork();

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

	/*
		�Ӷ�������������ϢЭ�������
	*/
	void onImportClientMessagesCompleted();
	void onImportEntityDefCompleted();
	void onImportClientMessages(MemoryStream& stream);
	void onImportServerErrorsDescr(MemoryStream& stream);

	void createDataTypeFromStreams(MemoryStream& stream, bool canprint);
	void createDataTypeFromStream(MemoryStream& stream, bool canprint);
	void onImportClientEntityDef(MemoryStream& stream);

	void resetpassword_loginapp(bool noconnect);

	void createAccount_loginapp(bool noconnect);

	/*
		ͨ�������ݻ��AOIʵ���ID
	*/
	ENTITY_ID getAoiEntityIDFromStream(MemoryStream& stream);

public:
	ENTITY_ID entity_id() const {
		return entity_id_;
	}

	uint64 entity_uuid() const {
		return entity_uuid_;
	}

	const FString& entity_type() const {
		return entity_type_;
	}



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

	// �־û������Ϣ�� ���磺�ӷ���˵����Э����Գ־û������أ��´ε�¼�汾�������ı�
	// ����ֱ�Ӵӱ��ؼ������ṩ��¼�ٶ�
	PersistentInfos* persistentInfos_;

	// ��ǰ��ҵ�ʵ��id��ʵ�����
	uint64 entity_uuid_;
	ENTITY_ID entity_id_;
	FString entity_type_;

	// space�����ݣ����忴API�ֲ����spaceData
	// https://github.com/kbengine/kbengine/tree/master/docs/api
	TMap<FString, FString> spacedatas_;
	
	// ����ʵ�嶼��������� ��ο�API�ֲ����entities����
	// https://github.com/kbengine/kbengine/tree/master/docs/api
	TMap<ENTITY_ID, Entity*> entities_;

	// �����AOI��ΧС��256��ʵ��ʱ���ǿ���ͨ��һ�ֽ��������ҵ�entity
	TArray<ENTITY_ID> entityIDAliasIDList_;
	TMap<ENTITY_ID, MemoryStream*> bufferedCreateEntityMessage_;

	// ���з���˴������Ӧ�Ĵ�������
	static TMap<uint16, FKServerErr> serverErrs_;

	double lastTickTime_;
	double lastTickCBTime_;
	double lastUpdateToServerTime_;

	// ��ҵ�ǰ���ڿռ��id�� �Լ��ռ��Ӧ����Դ
	SPACE_ID spaceID_;
	FString spaceResPath_;
	bool isLoadedGeometry_;

	// ���ձ�׼��ÿ���ͻ��˲��ֶ�Ӧ�ð����������
	FString component_;

};


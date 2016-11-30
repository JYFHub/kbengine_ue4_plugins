// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"
#include "Interfaces.h"

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
class KBENGINEPLUGINS_API KBEngineApp : public InterfaceConnect
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

	void _closeNetwork();

	const TArray<uint8>& serverdatas() const
	{
		return serverdatas_;
	}

	void entityServerPos(const FVector& pos)
	{
		entityServerPos_ = pos;
	}

	KBEngineArgs* getInitArgs() const
	{
		return pArgs_;
	}

	void installEvents();

	void resetMessages();

	static bool validEmail(const FString& strEmail);

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
	bool login(const FString& username, const FString& password, const TArray<uint8>& datas);
	virtual void onConnectCallback(FString ip, uint16 port, bool success, int userdata) override;

	/*
		�˺Ŵ������ؽ��
	*/
	void Client_onCreateAccountResult(MemoryStream& stream);

	/*
		�����˺�
	*/
	bool createAccount(const FString& username, const FString& password, const TArray<uint8>& datas);

	/*
		��������, ͨ��loginapp
	*/
	void resetPassword(const FString& username);
	void onOpenLoginapp_resetpassword();
	void onConnectTo_resetpassword_callback(FString ip, uint16 port, bool success);
	void Client_onReqAccountResetPasswordCB(uint16 failcode);

	/*
		��Email��ͨ��baseapp
	*/
	void bindAccountEmail(const FString& emailAddress);
	void Client_onReqAccountBindEmailCB(uint16 failcode);

	/*
		���������룬ͨ��baseapp�� ������ҵ�¼���߲���������baseapp��
	*/
	void newPassword(const FString& old_password, const FString& new_password);
	void Client_onReqAccountNewPasswordCB(uint16 failcode);

	/*
	�ص�¼������(baseapp)
	һЩ�ƶ���Ӧ�����׵��ߣ�����ʹ�øù��ܿ��ٵ����������˽���ͨ��
	*/
	void reLoginBaseapp();
	void onReloginTo_baseapp_callback(FString ip, uint16 port, bool success);

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
		�����ʹ���Ż��ķ�ʽ����ʵ�巽��
	*/
	void Client_onRemoteMethodCallOptimized(MemoryStream& stream);

	/*
		����˵���ʵ�巽��
	*/
	void Client_onRemoteMethodCall(MemoryStream& stream);

	/*
		���߿ͻ��ˣ��㵱ǰ���𣨻�ȡ��������˭��λ��ͬ��
	*/
	void Client_onControlEntity(ENTITY_ID eid, int8 isControlled);

	/*
		����˳�ʼ���ͻ��˵�spacedata�� spacedata��ο�API
	*/
	void Client_initSpaceData(MemoryStream& stream);
	FString getSpaceData(const FString& key);

	/*
		����˳�ʼ���ͻ��˵�spacedata�� spacedata��ο�API
	*/
	void Client_setSpaceData(uint32 spaceID, const FString& key, const FString& value);

	/*
		�����ɾ���ͻ��˵�spacedata�� spacedata��ο�API
	*/
	void Client_delSpaceData(uint32 spaceID, const FString& key);

	/*
		�����֪ͨ���������ؿ�ʼ
		��ο�API�ֲ����onStreamDataStarted
	*/
	void Client_onStreamDataStarted(int16 id, uint32 datasize, FString descr);
	void Client_onStreamDataRecv(MemoryStream& stream);
	void Client_onStreamDataCompleted(int16 id);

	/*
		�����֪ͨһ��ʵ�����������(���ʵ���ǵ�ǰ�������ҵ�һ����һ��space�д����ˣ� ���������ʵ����������ʵ���������ҵ�AOI)
	*/
	void Client_onEntityEnterWorld(MemoryStream& stream);

	/*
		�����ʹ���Ż��ķ�ʽ֪ͨһ��ʵ���뿪������(���ʵ���ǵ�ǰ���������뿪��space�� ���������ʵ����������ʵ���뿪����ҵ�AOI)
	*/
	void Client_onEntityLeaveWorldOptimized(MemoryStream& stream);

	/*
		�����֪ͨһ��ʵ���뿪������(���ʵ���ǵ�ǰ���������뿪��space�� ���������ʵ����������ʵ���뿪����ҵ�AOI)
	*/
	void Client_onEntityLeaveWorld(ENTITY_ID eid);

	/*
		�����֪ͨ��ǰ��ҽ�����һ���µ�space
	*/
	void Client_onEntityEnterSpace(MemoryStream& stream);

	/*
		�����֪ͨ��ǰ����뿪��space
	*/
	void Client_onEntityLeaveSpace(ENTITY_ID eid);

	/*
		����˸�����ҵĻ���λ�ã� �ͻ������������λ�ü��ϱ���ֵ����������Χʵ�������
	*/
	void Client_onUpdateBasePos(float x, float y, float z);
	void Client_onUpdateBasePosXZ(float x, float z);
	void Client_onUpdateBaseDir(MemoryStream& stream);
	void Client_onUpdateData(MemoryStream& stream);

	/*
		�����ǿ����������ҵ�����
		���磺�ڷ����ʹ��avatar.position=(0,0,0), �������λ�����ٶ��쳣ʱ��ǿ�����ص�һ��λ��
	*/
	void Client_onSetEntityPosAndDir(MemoryStream& stream);

	void Client_onUpdateData_ypr(MemoryStream& stream);
	void Client_onUpdateData_yp(MemoryStream& stream);
	void Client_onUpdateData_yr(MemoryStream& stream);
	void Client_onUpdateData_pr(MemoryStream& stream);
	void Client_onUpdateData_y(MemoryStream& stream);
	void Client_onUpdateData_p(MemoryStream& stream);
	void Client_onUpdateData_r(MemoryStream& stream);
	void Client_onUpdateData_xz(MemoryStream& stream);
	void Client_onUpdateData_xz_ypr(MemoryStream& stream);
	void Client_onUpdateData_xz_yp(MemoryStream& stream);
	void Client_onUpdateData_xz_yr(MemoryStream& stream);
	void Client_onUpdateData_xz_pr(MemoryStream& stream);
	void Client_onUpdateData_xz_y(MemoryStream& stream);
	void Client_onUpdateData_xz_p(MemoryStream& stream);
	void Client_onUpdateData_xz_r(MemoryStream& stream);
	void Client_onUpdateData_xyz(MemoryStream& stream);
	void Client_onUpdateData_xyz_ypr(MemoryStream& stream);
	void Client_onUpdateData_xyz_yp(MemoryStream& stream);
	void Client_onUpdateData_xyz_yr(MemoryStream& stream);
	void Client_onUpdateData_xyz_pr(MemoryStream& stream);
	void Client_onUpdateData_xyz_y(MemoryStream& stream);
	void Client_onUpdateData_xyz_p(MemoryStream& stream);
	void Client_onUpdateData_xyz_r(MemoryStream& stream);

private:
	void _updateVolatileData(ENTITY_ID entityID, float x, float y, float z, float yaw, float pitch, float roll, int8 isOnGround);

	bool initNetwork();

	void login_loginapp(bool noconnect);
	void onConnectTo_loginapp_login_callback(FString ip, uint16 port, bool success);
	void onLogin_loginapp();

	void login_baseapp(bool noconnect);
	void onConnectTo_baseapp_callback(FString ip, uint16 port, bool success);
	void onLogin_baseapp();


	void clearSpace(bool isall);
	void clearEntities(bool isall);


	void updatePlayerToServer();

	void onServerDigest();

	void addSpaceGeometryMapping(uint32 uspaceID, const FString& respath);

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
	void onOpenLoginapp_createAccount();
	void onConnectTo_loginapp_create_callback(FString ip, uint16 port, bool success);

	/*
		ͨ�������ݻ��AOIʵ���ID
	*/
	ENTITY_ID getAoiEntityIDFromStream(MemoryStream& stream);

	/*
	����˸���ʵ����������
	*/
	void onUpdatePropertys_(ENTITY_ID eid, MemoryStream& stream);

	void onRemoteMethodCall_(ENTITY_ID eid, MemoryStream& stream);

public:
	SPACE_ID spaceID() const {
		return spaceID_;
	}

	ENTITY_ID entity_id() const {
		return entity_id_;
	}

	uint64 entity_uuid() const {
		return entity_uuid_;
	}

	const FString& entity_type() const {
		return entity_type_;
	}

	const FString& serverVersion() const {
		return serverVersion_;
	}

	const FString& clientVersion() const {
		return clientVersion_;
	}

	const FString& serverScriptVersion() const {
		return serverScriptVersion_;
	}

	const FString& clientScriptVersion() const {
		return clientScriptVersion_;
	}

	const FString& component() const {
		return component_;
	}

	const FString& currserver() const {
		return currserver_;
	}

	const FString& currstate() const {
		return currstate_;
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

	TArray<Entity*> controlledEntities_;

	// ��ǰ��������һ��ͬ�����������λ��
	FVector entityServerPos_;

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


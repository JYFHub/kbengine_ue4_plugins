// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Object.h"
#include "KBECommon.h"
#include "KBEvent.generated.h"


/*
�¼�ģ��
�¼������ݻ����࣬ ��ͬ�¼���Ҫʵ�ֲ�ͬ��������
*/
UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData : public UObject
{
	GENERATED_BODY()

public:
	// �¼����ƣ������ڶ��¼����ͽ����жϣ����������¼�����ʱ�¼�ϵͳ�������
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString eventName;
};

/*
	�¼�ģ��
	KBEngine�������Unity3D���ֲ�ͨ���¼�������
*/
class KBENGINEPLUGINS_API KBEvent
{
public:
	KBEvent();
	virtual ~KBEvent();
	
public:
		static bool registerEvent(const FString& eventName, const FString& funcName, TFunction<void(const UKBEventData*)> func, void* objPtr);
		static bool deregister(void* objPtr, const FString& eventName, const FString& funcName);
		static bool deregister(void* objPtr);

		static void fire(const FString& eventName, UKBEventData* pEventData);

		static void clear();
		static void clearFiredEvents();

		static void processInEvents() {}
		static void processOutEvents() {}

protected:
	struct EventObj
	{
		TFunction<void(const UKBEventData*)> method;
		FString funcName;
		void* objPtr;
	};

	static TMap<FString, TArray<EventObj>> events_;
};


// ע���¼�
#define KBENGINE_REGISTER_EVENT(EVENT_NAME, EVENT_FUNC) \
	KBEvent::registerEvent(EVENT_NAME, #EVENT_FUNC, [this](const UKBEventData* pEventData) {	EVENT_FUNC(pEventData);	}, (void*)this);

// ע���¼�������д�¼�����
#define KBENGINE_REGISTER_EVENT_OVERRIDE_FUNC(EVENT_NAME, FUNC_NAME, EVENT_FUNC) \
	KBEvent::registerEvent(EVENT_NAME, FUNC_NAME, EVENT_FUNC, (void*)this);

// ע���������ĳ���¼�
#define KBENGINE_DEREGISTER_EVENT(EVENT_NAME) KBEvent::deregister((void*)this, EVENT_NAME, FUNC_NAME);

// ע������������е��¼�
#define KBENGINE_DEREGISTER_ALL_EVENT()	KBEvent::deregister((void*)this);

// fire event
#define KBENGINE_EVENT_FIRE(EVENT_NAME, EVENT_DATA) KBEvent::fire(EVENT_NAME, EVENT_DATA);

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_Baseapp_importClientMessages : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onKicked : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int32 failedcode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString errorStr;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_createAccount : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	TArray<uint8> datas;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_login : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString username;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	TArray<uint8> datas;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLoginFailed : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int32 failedcode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString errorStr;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLoginBaseapp : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLoginSuccessfully : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, /*BlueprintReadWrite, No support*/ Category = KBEngine)
	uint64  entity_uuid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int32 entity_id;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onReLoginBaseapp : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLoginBaseappFailed : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int32 failedcode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString errorStr;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onReLoginBaseappFailed : public UKBEventData
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int32 failedcode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString errorStr;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onReLoginBaseappSuccessfully : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onVersionNotMatch : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString clientVersion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString serverVersion;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onScriptVersionNotMatch : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString clientScriptVersion;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString serverScriptVersion;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_Loginapp_importClientMessages : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_Baseapp_importClientEntityDef : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onControlled : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int entityID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool isControlled;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLoseControlledEntity : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int entityID;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_updatePosition : public UKBEventData
{
	GENERATED_BODY()

public:
	FVector position;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_set_position : public UKBEventData
{
	GENERATED_BODY()

public:
	FVector position;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_set_direction : public UKBEventData
{
	GENERATED_BODY()

public:
	// roll, pitch, yaw
	FVector direction;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onCreateAccountResult : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(Category = KBEngine, BlueprintReadWrite, EditAnywhere)
	int errorCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString errorStr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	TArray<uint8> datas;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_addSpaceGeometryMapping : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString spaceResPath;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onSetSpaceData : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int spaceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString key;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString value;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onDelSpaceData : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int spaceID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString key;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onDisableConnect : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onConnectStatus : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool success;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString address;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onEnterWorld : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLeaveWorld : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onEnterSpace : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_onLeaveSpace : public UKBEventData
{
	GENERATED_BODY()

public:
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_resetPassword : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString username;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_bindAccountEmail : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString email;
};

UCLASS(Blueprintable, BlueprintType)
class KBENGINEPLUGINS_API UKBEventData_newPassword : public UKBEventData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString old_password;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString new_password;
};

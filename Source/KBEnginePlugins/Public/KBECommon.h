// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "KBVar.h"
#include "GameFramework/Actor.h"
#include "KBECommon.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogKBEngine, Log, All);

#define KBE_ASSERT check

typedef uint16 MessageID;
typedef uint16 MessageLength;
typedef uint32 MessageLengthEx;
typedef int32 ENTITY_ID;
typedef uint32 SPACE_ID;
typedef uint64 DBID;
typedef TArray<uint8> ByteArray;
typedef TMap<FString, KBVar> KB_FIXED_DICT;
typedef TArray<KBVar> KB_ARRAY;

#define KBE_FLT_MAX FLT_MAX

/** ��ȫ���ͷ�һ��ָ���ڴ� */
#define SAFE_RELEASE(i)										\
	if (i)													\
		{													\
			delete i;										\
			i = NULL;										\
		}

/** ��ȫ���ͷ�һ��ָ�������ڴ� */
#define SAFE_RELEASE_ARRAY(i)								\
	if (i)													\
		{													\
			delete[] i;										\
			i = NULL;										\
		}

USTRUCT(BlueprintType)
struct FKServerErr
{
	GENERATED_USTRUCT_BODY()

	FKServerErr()
	: name()
	, descr()
	, id(0)
	{
	}

	UPROPERTY(Category = ServerErr, BlueprintReadWrite, EditAnywhere)
	FString name;

	UPROPERTY(Category = ServerErr, BlueprintReadWrite, EditAnywhere)
	FString descr;

	UPROPERTY(Category = ServerErr, BlueprintReadWrite, EditAnywhere)
	int32 id;
};

// �ͻ��˵����
// http://www.kbengine.org/docs/programming/clientsdkprogramming.html
// http://www.kbengine.org/cn/docs/programming/clientsdkprogramming.html
UENUM(BlueprintType)
enum class EKCLIENT_TYPE : uint8
{
	CLIENT_TYPE_UNKNOWN		UMETA(DisplayName = "unknown"),
	CLIENT_TYPE_MOBILE		UMETA(DisplayName = "Mobile"),
	CLIENT_TYPE_WIN			UMETA(DisplayName = "Windows"),
	CLIENT_TYPE_LINUX		UMETA(DisplayName = "Linux"),
	CLIENT_TYPE_MAC			UMETA(DisplayName = "Mac"),
	CLIENT_TYPE_BROWSER		UMETA(DisplayName = "Browser"),
	CLIENT_TYPE_BOTS		UMETA(DisplayName = "Bots"),
	CLIENT_TYPE_MINI		UMETA(DisplayName = "Mini"),
};

// ������Ϣ���
enum NETWORK_MESSAGE_TYPE
{
	NETWORK_MESSAGE_TYPE_COMPONENT = 0,		// �����Ϣ
	NETWORK_MESSAGE_TYPE_ENTITY = 1,		// entity��Ϣ
};

enum ProtocolType
{
	PROTOCOL_TCP = 0,
	PROTOCOL_UDP = 1,
};

enum EntityDataFlags
{
	ED_FLAG_UNKOWN = 0x00000000,			// δ����
	ED_FLAG_CELL_PUBLIC = 0x00000001,		// �������cell�㲥
	ED_FLAG_CELL_PRIVATE = 0x00000002,		// ��ǰcell
	ED_FLAG_ALL_CLIENTS = 0x00000004,		// cell�㲥�����пͻ���
	ED_FLAG_CELL_PUBLIC_AND_OWN = 0x00000008, // cell�㲥���Լ��Ŀͻ���
	ED_FLAG_OWN_CLIENT = 0x00000010,		// ��ǰcell�Ϳͻ���
	ED_FLAG_BASE_AND_CLIENT = 0x00000020,	// base�Ϳͻ���
	ED_FLAG_BASE = 0x00000040,				// ��ǰbase
	ED_FLAG_OTHER_CLIENTS = 0x00000080,		// cell�㲥�������ͻ���
};

// ���ܶ���洢����Ϣռ���ֽ�(����+���)
#define ENCRYPTTION_WASTAGE_SIZE			(1 + 7)

#define PACKET_MAX_SIZE						1500
#ifndef PACKET_MAX_SIZE_TCP
#define PACKET_MAX_SIZE_TCP					1460
#endif
#define PACKET_MAX_SIZE_UDP					1472

typedef uint16								PacketLength;				// ���65535
#define PACKET_LENGTH_SIZE					sizeof(PacketLength)

#define NETWORK_MESSAGE_ID_SIZE				sizeof(MessageID)
#define NETWORK_MESSAGE_LENGTH_SIZE			sizeof(MessageLength)
#define NETWORK_MESSAGE_LENGTH1_SIZE		sizeof(MessageLengthEx)
#define NETWORK_MESSAGE_MAX_SIZE			65535
#define NETWORK_MESSAGE_MAX_SIZE1			4294967295

// ��Ϸ���ݿ��ð���С
#define GAME_PACKET_MAX_SIZE_TCP			PACKET_MAX_SIZE_TCP - NETWORK_MESSAGE_ID_SIZE - \
											NETWORK_MESSAGE_LENGTH_SIZE - ENCRYPTTION_WASTAGE_SIZE

/*
	������Ϣ���ͣ� �������߱䳤��
	�����Ҫ�Զ��峤������NETWORK_INTERFACE_DECLARE_BEGIN������ʱ���볤�ȼ��ɡ�
*/
#ifndef NETWORK_FIXED_MESSAGE
#define NETWORK_FIXED_MESSAGE 0
#endif

#ifndef NETWORK_VARIABLE_MESSAGE
#define NETWORK_VARIABLE_MESSAGE -1
#endif

/*
	����MTU���ֵ
*/

#define TCP_PACKET_MAX 1460

float getTimeSeconds();

inline float int82angle(int8 angle, bool half)
{
	float halfv = 128.f;
	if (half == true)
		halfv = 254.f;

	halfv = ((float)angle) * ((float)PI / halfv);
	return halfv;
}

inline bool almostEqual(float f1, float f2, float epsilon)
{
	return FMath::Abs(f1 - f2) < epsilon;
}

// UE4�ĳ߶ȵ�λת��Ϊ��
#define UE4_SCALE_UNIT_TO_METER 100.f

// ��KBE����ϵ��position(Vector3)ת��ΪUE4����ϵ��λ��
inline void KBPos2UE4Pos(FVector& UE4_POSITION, const FVector& KBE_POSITION)
{	
	// UE4���굥λΪ���ף� KBE��λΪ�ף� ���ת����Ҫ����
	UE4_POSITION.Y = KBE_POSITION.X * UE4_SCALE_UNIT_TO_METER;
	UE4_POSITION.Z = KBE_POSITION.Y * UE4_SCALE_UNIT_TO_METER;
	UE4_POSITION.X = KBE_POSITION.Z * UE4_SCALE_UNIT_TO_METER;
}	

// ��UE4����ϵ��position(Vector3)ת��ΪKBE����ϵ��λ��
inline void UE4Pos2KBPos(FVector& KBE_POSITION, const FVector& UE4_POSITION)
{
	// UE4���굥λΪ���ף� KBE��λΪ�ף� ���ת����Ҫ����
	KBE_POSITION.X = UE4_POSITION.Y / UE4_SCALE_UNIT_TO_METER;
	KBE_POSITION.Y = UE4_POSITION.Z / UE4_SCALE_UNIT_TO_METER;
	KBE_POSITION.Z = UE4_POSITION.X / UE4_SCALE_UNIT_TO_METER;
}

UCLASS()
class KBENGINEPLUGINS_API AKBECommon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKBECommon();
	
};



// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "KBECommon.h"
#include "Event.h"
#include "Components/ActorComponent.h"
#include "KBEMain.generated.h"

/*
�������Ϊ��������ģ��
���������а�װ����Ҫ�������¼�(installEvents)��ͬʱ��ʼ��KBEngine(initKBEngine)
*/
class KBEngineApp;


UCLASS(ClassGroup = "KBEngine", blueprintable, editinlinenew, hidecategories = (Object, LOD, Lighting, TextureStreaming), meta = (DisplayName = "KBEngine Main", BlueprintSpawnableComponent))
class KBENGINEPLUGINS_API UKBEMain : public UActorComponent
{
	GENERATED_UCLASS_BODY()

public:	
	// Sets default values for this component's properties
	UKBEMain();

	/**
	* Initializes the component.  Occurs at level startup. This is before BeginPlay (Actor or Component).
	* All Components in the level will be Initialized on load before any Actor/Component gets BeginPlay
	* Requires component to be registered, and bWantsInitializeComponent to be true.
	*/
	virtual void InitializeComponent() override;

	// Called when the game starts
	virtual void BeginPlay() override;

	/**
	* Ends gameplay for this component.
	* Called from AActor::EndPlay only if bHasBegunPlay is true
	*/
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/**
	* Handle this component being Uninitialized.
	* Called from AActor::EndPlay only if bHasBeenInitialized is true
	*/
	virtual void UninitializeComponent() override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	FString getClientVersion();

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	FString getClientScriptVersion();

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	FString getServerVersion();

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	FString getServerScriptVersion();

	/*
		�ͻ�������KBE����е�һ����������������ȡ���̶�����client
	*/
	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	FString getComponentName();

	/**
		�ڳ���ر�ʱ��Ҫ��������, ��������KBEngine
	*/
	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	bool destroyKBEngine();

	UFUNCTION(BlueprintCallable, Category = "KBEngine")
	bool login(FString username, FString password, TArray<uint8> datas);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString ip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int port;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	EKCLIENT_TYPE clientType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool syncPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool useAliasEntityID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	bool isOnInitCallPropertysSetMethods;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int SEND_BUFFER_MAX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	int RECV_BUFFER_MAX;

	// �־û������Ϣ�� ���磺�ӷ���˵����Э����Գ־û������أ��´ε�¼�汾�������ı�
	// ����ֱ�Ӵӱ��ؼ������ṩ��¼�ٶ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = KBEngine)
	FString persistentDataPath;
};

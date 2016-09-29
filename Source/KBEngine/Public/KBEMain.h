// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "KBEMain.generated.h"

/*
�������Ϊ��������ģ��
���������а�װ����Ҫ�������¼�(installEvents)��ͬʱ��ʼ��KBEngine(initKBEngine)
*/

UCLASS(ClassGroup = "KBEngine", blueprintable, editinlinenew, hidecategories = (Object, LOD, Lighting, TextureStreaming), meta = (DisplayName = "KBEngine Main", BlueprintSpawnableComponent))
class KBENGINE_API UKBEMain : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UKBEMain();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

		
	UPROPERTY(EditAnywhere, Category = KBEngine)
	FString ip;

	UPROPERTY(EditAnywhere, Category = KBEngine)
	int port;

	UPROPERTY(EditAnywhere, Category = KBEngine)
	bool syncPlayer;

	UPROPERTY(EditAnywhere, Category = KBEngine)
	bool useAliasEntityID;

	UPROPERTY(EditAnywhere, Category = KBEngine)
	bool isOnInitCallPropertysSetMethods;

};

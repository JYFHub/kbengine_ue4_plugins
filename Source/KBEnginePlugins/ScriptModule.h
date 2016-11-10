// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

class Property;
class Method;

/*
	һ��entitydef�ж���Ľű�ģ���������
	������ĳ��entity����������뷽���Լ���entity�ű�ģ���������ģ��ID
*/
class KBENGINEPLUGINS_API ScriptModule
{
public:
	ScriptModule(const FString& moduleName);
	virtual ~ScriptModule();

public:
	FString name;
	bool usePropertyDescrAlias;
	bool useMethodDescrAlias;

	TMap<FString, Property*> propertys;
	TMap<uint16, Property*> idpropertys;

	TMap<FString, Method*> methods;
	TMap<FString, Method*> base_methods;
	TMap<FString, Method*> cell_methods;

	TMap<uint16, Method*> idmethods;
	TMap<uint16, Method*> idbase_methods;
	TMap<uint16, Method*> idcell_methods;

	// Type script = null;
};

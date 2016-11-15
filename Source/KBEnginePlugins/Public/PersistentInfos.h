// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

/*
	�־û�����Э�飬�ڼ�⵽Э��汾�����ı�ʱ������Э��
*/
class KBENGINEPLUGINS_API PersistentInfos
{
public:
	PersistentInfos();
	PersistentInfos(const FString& path);
	virtual ~PersistentInfos();

public:
	bool isGood() const
	{
		return _isGood;
	}
	
	void installEvents();

	bool loadAll();

	void onImportClientMessages(const FString& currserver, const TArray<uint8>& stream);
	void onImportServerErrorsDescr(const TArray<uint8>& stream);
	void onImportClientEntityDef(const TArray<uint8>& stream);
	void onVersionNotMatch(const FString& verInfo, const FString& serVerInfo);
	void onScriptVersionNotMatch(const FString& verInfo, const FString& serVerInfo);
	void onServerDigest(const FString& currserver, const FString& serverProtocolMD5, const FString& serverEntitydefMD5);

	void clearMessageFiles();

private:
	FString _persistentDataPath;
	bool _isGood;
	FString _digest;
};

// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "KBECommon.h"

/*
	�¼�ģ��
	KBEngine�������Unity3D���ֲ�ͨ���¼�������
*/
class KBENGINEPLUGINS_API Event
{
public:
	Event();
	virtual ~Event();

public:
		/*
			ע�������kbe����׳����¼���(out = kbe->render)
			ͨ������Ⱦ���ֲ���ע��, ���磺������ɫѪ�����Եı仯�� ���UI��ע������¼���
			�¼�������Ϳ��Ը����¼��������ĵ�ǰѪ��ֵ���ı��ɫͷ����Ѫ��ֵ��
		*/
		static bool registerOut(FString eventname, FString funcname)
		{
			return _register(eventname, funcname);
		}

		/*
			ע���������Ⱦ���ֲ��׳����¼�(in = render->kbe)
			ͨ����kbe�������ע�ᣬ ���磺UI������¼�� ��ʱ��Ҫ����һ���¼���kbe�������������˽����Ĵ���
		*/
		static bool registerIn(FString eventname, FString funcname)
		{
			return _register(eventname, funcname);
		}

		static bool _register(FString eventname, FString funcname);
		
protected:
};

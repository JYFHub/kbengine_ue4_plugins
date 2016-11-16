// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.



#pragma once

#include "KBVar.h"
#include "KBECommon.h"

class Mailbox;


/*
	KBEngine�߼����ʵ�������
	������չ������Ϸʵ�嶼Ӧ�ü̳��ڸ�ģ��
*/
class KBENGINEPLUGINS_API Entity
{
public:
	Entity();
	virtual ~Entity();

	static void clear();

public:
	const FString& className() const {
		return className_;
	}

	void className(const FString& v) {
		className_ = v;
	}

	bool inWorld() const {
		return inWorld_;
	}

	void inWorld(bool v) {
		inWorld_ = v;
	}

	bool isControlled() const {
		return isControlled_;
	}

	void isControlled(bool v) {
		isControlled_ = v;
	}

	bool inited() const {
		return inited_;
	}

protected:
	int32 id_;
	FString className_;

	FVector position_;
	FVector direction_;

	bool isOnGround_;

	Mailbox* base_;
	Mailbox* cell_;

	// enterworld֮������Ϊtrue
	bool inWorld_;

	// �������������˵������ʾ�Ƿ��Լ���������ҿ����ˣ�
	// ��������entity��˵����ʾ�ұ����Ƿ���������entity
	bool isControlled_;

	bool inited_;
};

/*
	��������Ҫ����ĳ��ʵ��ʱ������ײ���ô�����
	ÿ���һ���µ�ʵ�������Դ�����м���� KBENGINE_ENTITY_CLASS_REGISTER(XEntity)���Ա�ײ�ע��һ����ص�EntityCreator
*/
class KBENGINEPLUGINS_API EntityCreator
{
public:
	EntityCreator(const FString& scriptName);

	virtual ~EntityCreator();

	virtual Entity* create() = 0;
};

/*
	ʵ�崴��������
*/
class KBENGINEPLUGINS_API EntityFactory
{
public:
	static Entity* create(const FString& scriptName);
	EntityCreator* addEntityCreator(const FString& scriptName, EntityCreator* pEntityCreator);

	static EntityCreator* findCreator(const FString& scriptName);

	TMap<FString, EntityCreator*> creators;
};

/*
	��װʵ���defined����
*/
class KBENGINEPLUGINS_API EntityDefMethodHandle
{
public:
	EntityDefMethodHandle(const FString& scriptName, const FString& defMethodName);

	virtual ~EntityDefMethodHandle();

	virtual void callMethod(Entity* pEntity, KBVar* oldVal) = 0;
};

class KBENGINEPLUGINS_API EntityDefMethodHandles
{
public:
	EntityDefMethodHandle* add(const FString& scriptName, const FString& defMethodName, EntityDefMethodHandle* pEntityDefMethodHandle);
	static EntityDefMethodHandle* find(const FString& scriptName, const FString& defMethodName);

	TMap<FString, TMap<FString, EntityDefMethodHandle*>> defMethodHandles;
};

/*
	��װʵ���defined����
*/
class KBENGINEPLUGINS_API EntityDefPropertyHandle
{
public:
	EntityDefPropertyHandle(const FString& scriptName, const FString& defPropertyName);

	virtual ~EntityDefPropertyHandle();

	virtual void setPropertyValue(Entity* pEntity, KBVar* oldVal) = 0;
	virtual KBVar* getPropertyValue(Entity* pEntity) = 0;
};

class KBENGINEPLUGINS_API EntityDefPropertyHandles
{
public:
	EntityDefPropertyHandle* add(const FString& scriptName, const FString& defPropertyName, EntityDefPropertyHandle* pEntityDefPropertyHandle);
	static EntityDefPropertyHandle* find(const FString& scriptName, const FString& defPropertyName);

	TMap<FString, TMap<FString, EntityDefPropertyHandle*>> defPropertyHandles;
};


// ע�ᶨ���ʵ����
#define ENTITYDEF_CLASS_REGISTER(ENTITY_SCRIPTMODULE_NAME)	\
	class ENTITY_SCRIPTMODULE_NAME##Creator : public EntityCreator {	\
		public:	\
			ENTITY_SCRIPTMODULE_NAME##Creator(const FString& scriptName):	\
			EntityCreator(scriptName)	\
			{	\
			}	\
			virtual ~ENTITY_SCRIPTMODULE_NAME##Creator()	\
			{	\
			}	\
			virtual Entity* create() override	\
			{	\
				return new ENTITY_SCRIPTMODULE_NAME();	\
			}	\
	};\
	ENTITY_SCRIPTMODULE_NAME##Creator g_ENTITY_SCRIPTMODULE_NAME##Creator(FString(TEXT(#ENTITY_SCRIPTMODULE_NAME)));	\


// ע�ᶨ��ķ���
#define ENTITYDEF_METHOD_REGISTER(ENTITY_SCRIPTMODULE_NAME, DEF_METHOD_NAME)	\
	class ENTITY_SCRIPTMODULE_NAME##DefMethodHandle : public EntityDefMethodHandle {	\
		public:	\
			ENTITY_SCRIPTMODULE_NAME##DefMethodHandle(const FString& scriptName, const FString& defMethodName):	\
			EntityDefMethodHandle(scriptName, defMethodName)	\
			{	\
			}	\
			virtual ~ENTITY_SCRIPTMODULE_NAME##DefMethodHandle()	\
			{	\
			}	\
			virtual void callMethod(Entity* pEntity, KBVar* oldVal) override	\
			{	\
				static_cast<ENTITY_SCRIPTMODULE_NAME*>(pEntity)->DEF_METHOD_NAME(*oldVal);	\
			}	\
	};\
	ENTITY_SCRIPTMODULE_NAME##DefMethodHandle g_ENTITY_SCRIPTMODULE_NAME##DefMethodHandle(FString(TEXT(#ENTITY_SCRIPTMODULE_NAME)), FString(TEXT(#DEF_METHOD_NAME)));	\


// ע�ᶨ�������
#define ENTITYDEF_PROPERTY_REGISTER(ENTITY_SCRIPTMODULE_NAME, DEF_PROPERTY_NAME)	\
	class ENTITY_SCRIPTMODULE_NAME##DefPropertyHandle : public EntityDefPropertyHandle {	\
		public:	\
			ENTITY_SCRIPTMODULE_NAME##DefPropertyHandle(const FString& scriptName, const FString& defPropertyName):	\
			EntityDefPropertyHandle(scriptName, defPropertyName)	\
			{	\
			}	\
			virtual ~ENTITY_SCRIPTMODULE_NAME##DefPropertyHandle()	\
			{	\
			}	\
			virtual void setPropertyValue(Entity* pEntity, KBVar* oldVal) override	\
			{	\
				static_cast<ENTITY_SCRIPTMODULE_NAME*>(pEntity)->DEF_PROPERTY_NAME = *oldVal;	\
			}	\
			virtual KBVar* getPropertyValue(Entity* pEntity) override	\
			{	\
				return new KBVar(static_cast<ENTITY_SCRIPTMODULE_NAME*>(pEntity)->DEF_PROPERTY_NAME);	\
			}	\
	};\
	ENTITY_SCRIPTMODULE_NAME##DefPropertyHandle g_ENTITY_SCRIPTMODULE_NAME##DefPropertyHandle(FString(TEXT(#ENTITY_SCRIPTMODULE_NAME)), FString(TEXT(#DEF_PROPERTY_NAME)));	\


// ע�ᶨ������ԣ��������԰���һ��set_*���������Ա����º�ķ���������
#define ENTITYDEF_PROPERTY_AND_SETMETHOD_REGISTER(ENTITY_SCRIPTMODULE_NAME, DEF_PROPERTY_NAME)	\
	ENTITYDEF_PROPERTY_REGISTER(ENTITY_SCRIPTMODULE_NAME, DEF_PROPERTY_NAME)	\
	ENTITYDEF_METHOD_REGISTER(ENTITY_SCRIPTMODULE_NAME, set_DEF_PROPERTY_NAME)	\

	







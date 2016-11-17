
#include "KBEnginePluginsPrivatePCH.h"
#include "KBEngine.h"
#include "KBEngineArgs.h"
#include "Entity.h"
#include "NetworkInterface.h"
#include "Bundle.h"
#include "MemoryStream.h"
#include "PersistentInfos.h"
#include "DataTypes.h"
#include "ScriptModule.h"
#include "Property.h"
#include "Method.h"
#include "Mailbox.h"

TMap<uint16, FKServerErr> KBEngineApp::serverErrs_;

bool KBEngineApp::loadingLocalMessages_ = false;


bool KBEngineApp::loginappMessageImported_ = false;
bool KBEngineApp::baseappMessageImported_ = false;
bool KBEngineApp::entitydefImported_ = false;
bool KBEngineApp::isImportServerErrorsDescr_ = false;

KBEngineApp::KBEngineApp() :
	pArgs_(NULL),
	pNetworkInterface_(NULL),
	username_(TEXT("")),
	password_(TEXT("")),
	baseappIP_(TEXT("")),
	baseappPort_(0),
	currserver_(TEXT("")),
	currstate_(TEXT("")),
	serverdatas_(),
	clientdatas_(),
	encryptedKey_(),
	serverVersion_(TEXT("")),
	clientVersion_(TEXT("")),
	serverScriptVersion_(TEXT("")),
	clientScriptVersion_(TEXT("")),
	serverProtocolMD5_(TEXT("")),
	serverEntitydefMD5_(TEXT("")),
	persistentInfos_(NULL),
	entity_uuid_(0),
	entity_id_(0),
	entity_type_(TEXT("")),
	spacedatas_(),
	entities_(),
	entityIDAliasIDList_(),
	bufferedCreateEntityMessage_(),
	lastTickTime_(0.0),
	lastTickCBTime_(0.0),
	lastUpdateToServerTime_(0.0),
	spaceID_(0),
	spaceResPath_(TEXT("")),
	isLoadedGeometry_(false),
	component_(TEXT("client"))
{
	INFO_MSG("hello!");
}

KBEngineApp::KBEngineApp(KBEngineArgs* pArgs):
	pArgs_(NULL),
	pNetworkInterface_(NULL),
	username_(TEXT("")),
	password_(TEXT("")),
	baseappIP_(TEXT("")),
	baseappPort_(0),
	currserver_(TEXT("")),
	currstate_(TEXT("")),
	serverdatas_(),
	clientdatas_(),
	encryptedKey_(),
	serverVersion_(TEXT("")),
	clientVersion_(TEXT("")),
	serverScriptVersion_(TEXT("")),
	clientScriptVersion_(TEXT("")),
	serverProtocolMD5_(TEXT("")),
	serverEntitydefMD5_(TEXT("")),
	persistentInfos_(NULL),
	entity_uuid_(0),
	entity_id_(0),
	entity_type_(TEXT("")),
	spacedatas_(),
	entities_(),
	entityIDAliasIDList_(),
	bufferedCreateEntityMessage_(),
	lastTickTime_(0.0),
	lastTickCBTime_(0.0),
	lastUpdateToServerTime_(0.0),
	spaceID_(0),
	spaceResPath_(TEXT("")),
	isLoadedGeometry_(false),
	component_(TEXT("client"))
{
	INFO_MSG("hello!");
	initialize(pArgs);
}

KBEngineApp::~KBEngineApp()
{
	destroy();
	INFO_MSG("destructed!");
}

KBEngineApp& KBEngineApp::getSingleton() 
{
	static KBEngineApp* pKBEngineApp = NULL;

	if (!pKBEngineApp)
		pKBEngineApp = new KBEngineApp();

	return *pKBEngineApp;
}

bool KBEngineApp::initialize(KBEngineArgs* pArgs)
{
	if (isInitialized())
		return false;

	EntityDef::initialize();

	// ����־û�KBE(����:Э�飬entitydef��)
	if (pArgs->persistentDataPath != TEXT(""))
	{
		SAFE_RELEASE(persistentInfos_);
		persistentInfos_ = new PersistentInfos(pArgs->persistentDataPath);
	}

	// ע���¼�
	installEvents();

	pArgs_ = pArgs;
	reset();
	return true;
}

void KBEngineApp::installEvents()
{
	/*
	Event.registerIn("createAccount", this, "createAccount");
	Event.registerIn("login", this, "login");
	Event.registerIn("reLoginBaseapp", this, "reLoginBaseapp");
	Event.registerIn("resetPassword", this, "resetPassword");
	Event.registerIn("bindAccountEmail", this, "bindAccountEmail");
	Event.registerIn("newPassword", this, "newPassword");

	// �ڲ��¼�
	Event.registerIn("_closeNetwork", this, "_closeNetwork");
	*/
}

void KBEngineApp::destroy()
{
	reset();
	resetMessages();

	SAFE_RELEASE(pArgs_);
	SAFE_RELEASE(pNetworkInterface_);
	SAFE_RELEASE(persistentInfos_);
}

void KBEngineApp::resetMessages()
{
	loadingLocalMessages_ = false;
	loginappMessageImported_ = false;
	baseappMessageImported_ = false;
	entitydefImported_ = false;
	isImportServerErrorsDescr_ = false;
	serverErrs_.Empty();
	Messages::getSingleton().clear();
	EntityDef::clear();
	Entity::clear();
	INFO_MSG("done!");
}

void KBEngineApp::reset()
{
	clearEntities(true);

	currserver_ = TEXT("");
	currstate_ = TEXT("");

	username_ = TEXT("kbengine");
	password_ = TEXT("123456");

	baseappIP_ = TEXT("");
	baseappPort_ = 0;

	serverdatas_.Empty();

	serverVersion_ = TEXT("");
	clientVersion_ = TEXT("0.9.0");
	serverScriptVersion_ = TEXT("");
	clientScriptVersion_ = TEXT("0.1.0");
	serverProtocolMD5_ = TEXT("");
	serverEntitydefMD5_ = TEXT("");

	entity_uuid_ = 0;
	entity_id_ = 0;
	entity_type_ = TEXT("");

	entityIDAliasIDList_.Empty();
	bufferedCreateEntityMessage_.Empty();

	lastTickTime_ = getTimeSeconds();
	lastTickCBTime_ = getTimeSeconds();
	lastUpdateToServerTime_ = getTimeSeconds();

	spacedatas_.Empty();

	spaceID_ = 0;
	spaceResPath_ = TEXT("");
	isLoadedGeometry_ = false;

	component_ = TEXT("client");
	

	initNetwork();
}

bool KBEngineApp::initNetwork()
{
	if (pNetworkInterface_)
		delete pNetworkInterface_;

	pNetworkInterface_ = new NetworkInterface();
	return true;
}

bool KBEngineApp::validEmail(FString strEmail)
{
	/*
	return Regex.IsMatch(strEmail, @"^([\w-\.]+)@((\[[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\.)
	|(([\w-]+\.)+))([a-zA-Z]{2,4}|[0-9]{1,3})(\]?)$");
	*/
	return false;
}

void KBEngineApp::process()
{
	// ��������
	if (pNetworkInterface_)
		pNetworkInterface_->process();

	// �����˷��������Լ�ͬ����ɫ��Ϣ�������
	sendTick();
}

void KBEngineApp::sendTick()
{
	if (!pNetworkInterface_ || !pNetworkInterface_->valid())
		return;

	if (!loginappMessageImported_ && !baseappMessageImported_)
		return;

	double span = getTimeSeconds() - lastTickTime_;

	// ������ҵ�λ���볯�򵽷����
	updatePlayerToServer();

	if (span > 15)
	{
		span = lastTickCBTime_ - lastTickTime_;

		// ��������ص�����ʱ��С����������ʱ�䣬˵��û���յ��ص�
		// ��ʱӦ��֪ͨ�ͻ��˵�����
		if (span < 0)
		{
			ERROR_MSG("Receive appTick timeout!");
			pNetworkInterface_->close();
			return;
		}

		lastTickTime_ = getTimeSeconds();
	}
}

Entity* KBEngineApp::player()
{
	return findEntity(entity_id_);
}

Entity* KBEngineApp::findEntity(int32 entityID)
{
	Entity** pEntity = entities_.Find(entityID);
	if (pEntity == nullptr)
		return NULL;

	return *pEntity;
}

FString KBEngineApp::serverErr(uint16 id)
{
	FKServerErr e = serverErrs_.FindRef(id);
	return FString::Printf(TEXT("%s[%s]"), *e.name, *e.descr);
}

void KBEngineApp::updatePlayerToServer()
{
	if (!pArgs_->syncPlayer || spaceID_ == 0)
		return;

	float tnow = getTimeSeconds();
	double span = tnow - lastUpdateToServerTime_;

	if (span < 0.1)
		return;


}

void KBEngineApp::Client_onAppActiveTickCB()
{
	lastTickCBTime_ = getTimeSeconds();
}

void KBEngineApp::hello()
{
	Bundle* pBundle = Bundle::createObject();
	if (currserver_ == "loginapp")
		pBundle->newMessage(Messages::getSingleton().messages["Loginapp_hello"]);
	else
		pBundle->newMessage(Messages::getSingleton().messages["Baseapp_hello"]);

	(*pBundle) << clientVersion_;
	(*pBundle) << clientScriptVersion_;
	pBundle->appendBlob(encryptedKey_);
	pBundle->send(pNetworkInterface_);
}

void KBEngineApp::Client_onHelloCB(MemoryStream& stream)
{
	stream >> serverVersion_;
	stream >> serverScriptVersion_;
	stream >> serverProtocolMD5_;
	stream >> serverEntitydefMD5_;

	int32 ctype;
	stream >> ctype;

	INFO_MSG("verInfo(%s), scriptVersion(%s), srvProtocolMD5(%s), srvEntitydefMD5(%s), ctype(%d)!", 
		*serverVersion_, *serverScriptVersion_, *serverProtocolMD5_, *serverEntitydefMD5_, ctype);

	onServerDigest();

	if (currserver_ == "baseapp")
	{
		onLogin_baseapp();
	}
	else
	{
		onLogin_loginapp();
	}
}

void KBEngineApp::Client_onVersionNotMatch(MemoryStream& stream)
{
	stream >> serverVersion_;

	ERROR_MSG("verInfo=%s(server: %s)", *clientVersion_, *serverVersion_);
	//Event.fireAll("onVersionNotMatch", new object[]{ clientVersion_, serverVersion_ });

	if (persistentInfos_)
		persistentInfos_->onVersionNotMatch(clientVersion_, serverVersion_);
}

void KBEngineApp::Client_onScriptVersionNotMatch(MemoryStream& stream)
{
	stream >> serverScriptVersion_;

	ERROR_MSG("verInfo=%s(server: %s)", *clientScriptVersion_, *serverScriptVersion_);
	//Event.fireAll("onScriptVersionNotMatch", new object[]{ clientScriptVersion, serverScriptVersion });

	if (persistentInfos_)
		persistentInfos_->onScriptVersionNotMatch(clientScriptVersion_, serverScriptVersion_);
}

void KBEngineApp::Client_onKicked(uint16 failedcode)
{
	DEBUG_MSG("failedcode=%d, %s", failedcode, *serverErr(failedcode));
	//Event.fireAll("onKicked", new object[]{ failedcode });
}

void KBEngineApp::Client_onImportServerErrorsDescr(MemoryStream& stream)
{
	TArray<uint8> datas;
	datas.SetNumUninitialized(stream.length());
	memcpy(datas.GetData(), stream.data() + stream.rpos(), stream.length());

	onImportServerErrorsDescr(stream);

	if (persistentInfos_)
		persistentInfos_->onImportServerErrorsDescr(datas);
}

void KBEngineApp::onImportServerErrorsDescr(MemoryStream& stream)
{
	uint16 size = 0;
	stream >> size;

	while (size > 0)
	{
		size -= 1;

		FKServerErr e;
		stream >> e.id;
		stream.readUTF8String(e.name);
		stream.readUTF8String(e.descr);

		serverErrs_.Add(e.id, e);

		DEBUG_MSG("id=%d, name=%s, descr=%s", e.id, *e.name, *e.descr);
	}
}

void KBEngineApp::onServerDigest()
{
	if (persistentInfos_)
		persistentInfos_->onServerDigest(currserver_, serverProtocolMD5_, serverEntitydefMD5_);
}

bool KBEngineApp::login(FString& username, FString& password, TArray<uint8>& datas)
{
	username_ = username;
	password_ = password;
	clientdatas_ = datas;

	login_loginapp(true);
	return true;
}

void KBEngineApp::onLoginCallback(FString ip, uint16 port, bool success, int userdata)
{
	if (userdata == 0)
	{
		onConnectTo_loginapp_callback(ip, port, success);
	}
	else
	{
		onConnectTo_baseapp_callback(ip, port, success);
	}
}

void KBEngineApp::login_loginapp(bool noconnect)
{
	if (noconnect)
	{
		reset();
		pNetworkInterface_->connectTo(pArgs_->ip, pArgs_->port, this, 0);
	}
	else
	{
		INFO_MSG("send login! username=%s", *username_);
		Bundle* pBundle = Bundle::createObject();
		pBundle->newMessage(Messages::getSingleton().messages[TEXT("Loginapp_login"]));
		(*pBundle) << (uint8)pArgs_->clientType;
		pBundle->appendBlob(clientdatas_);
		(*pBundle) << username_;
		(*pBundle) << password_;
		pBundle->send(pNetworkInterface_);
	}
}

void KBEngineApp::onConnectTo_loginapp_callback(FString ip, uint16 port, bool success)
{
	if (!success)
	{
		ERROR_MSG("connect %s:%d is error!", *ip, port);
		return;
	}

	currserver_ = TEXT("loginapp");
	currstate_ = TEXT("login");

	INFO_MSG("connect %s:%d is success!", *ip, port);

	hello();
}

void KBEngineApp::onLogin_loginapp()
{
	lastTickCBTime_ = getTimeSeconds();

	if (!loginappMessageImported_)
	{
		Bundle* pBundle = Bundle::createObject();
		pBundle->newMessage(Messages::getSingleton().messages[TEXT("Loginapp_importClientMessages"]));
		pBundle->send(pNetworkInterface_);

		DEBUG_MSG("send importClientMessages ...");
	//	Event.fireOut("Loginapp_importClientMessages", new object[]{});
	}
	else
	{
		onImportClientMessagesCompleted();
	}
}

void KBEngineApp::Client_onLoginFailed(MemoryStream& stream)
{
	uint16 failedcode = 0;
	stream >> failedcode;
	stream.readBlob(serverdatas_);
	ERROR_MSG("failedcode(%d:%s), datas(%d)!", failedcode, *serverErr(failedcode), serverdatas_.Num());
	//Event.fireAll("onLoginFailed", new object[]{ failedcode });
}

void KBEngineApp::Client_onLoginSuccessfully(MemoryStream& stream)
{
	FString accountName;
	stream >> accountName;
	username_ = accountName;
	stream >> baseappIP_;
	stream >> baseappPort_;

	DEBUG_MSG("accountName(%s), addr("
		 "%s:%d), datas(%d)!", *accountName, *baseappIP_, baseappPort_, serverdatas_.Num());

	stream.readBlob(serverdatas_);
	login_baseapp(true);
}

void KBEngineApp::login_baseapp(bool noconnect)
{
	if (noconnect)
	{
		//Event.fireOut("onLoginBaseapp", new object[]{});

		pNetworkInterface_->destroy();
		pNetworkInterface_ = NULL;
		initNetwork();
		pNetworkInterface_->connectTo(baseappIP_, baseappPort_, this, 1);
	}
	else
	{
		Bundle* pBundle = Bundle::createObject();
		pBundle->newMessage(Messages::getSingleton().messages[TEXT("Baseapp_loginBaseapp"]));
		(*pBundle) << username_;
		(*pBundle) << password_;
		pBundle->send(pNetworkInterface_);
	}
}

void KBEngineApp::onConnectTo_baseapp_callback(FString ip, uint16 port, bool success)
{
	lastTickCBTime_ = getTimeSeconds();

	if (!success)
	{
		ERROR_MSG("connect %s:%d is error!", *ip, port);
		return;
	}

	currserver_ = TEXT("baseapp");
	currstate_ = TEXT("");

	DEBUG_MSG("connect %s:%d is successfully!", *ip, port);

	hello();
}

void KBEngineApp::onLogin_baseapp()
{
	lastTickCBTime_ = getTimeSeconds();

	if (!baseappMessageImported_)
	{
		Bundle* pBundle = Bundle::createObject();
		pBundle->newMessage(Messages::getSingleton().messages[TEXT("Baseapp_importClientMessages"]));
		pBundle->send(pNetworkInterface_);
		DEBUG_MSG("send importClientMessages ...");
	//	Event.fireOut("Baseapp_importClientMessages", new object[]{});
	}
	else
	{
		onImportClientMessagesCompleted();
	}
}

void KBEngineApp::reLoginBaseapp()
{
	//Event.fireAll("onReLoginBaseapp", new object[]{});
	pNetworkInterface_->connectTo(baseappIP_, baseappPort_, this, 1);
}

void KBEngineApp::Client_onLoginBaseappFailed(uint16 failedcode)
{
	ERROR_MSG("failedcode(%d:%s)!", failedcode, *serverErr(failedcode));
	//Event.fireAll("onLoginBaseappFailed", new object[]{ failedcode });
}

void KBEngineApp::Client_onReLoginBaseappFailed(uint16 failedcode)
{
	ERROR_MSG("failedcode(%d:%s)!", failedcode, *serverErr(failedcode));
	//Event.fireAll("onReLoginBaseappFailed", new object[]{ failedcode });
}

void KBEngineApp::Client_onReLoginBaseappSuccessfully(MemoryStream& stream)
{
	stream >> entity_uuid_;
	ERROR_MSG("name(%s)!", *username_);
	//Event.fireAll("onReLoginBaseappSuccessfully", new object[]{});
}

void KBEngineApp::Client_onCreatedProxies(uint64 rndUUID, int32 eid, FString& entityType)
{
	DEBUG_MSG("eid(%d), entityType(%s)!", eid, *entityType);

	if (entities_.Contains(eid))
	{
		// WARNING_MSG("eid(%d) has exist!", eid);
		Client_onEntityDestroyed(eid);
	}

	entity_uuid_ = rndUUID;
	entity_id_ = eid;
	entity_type_ = entityType;

	ScriptModule** pModuleFind = EntityDef::moduledefs.Find(entityType);
	if (!pModuleFind)
	{
		ERROR_MSG("not found module(%s)!", *entityType);
		return;
	}

	ScriptModule* pModule = *pModuleFind;

	EntityCreator* pEntityCreator = pModule->pEntityCreator;
	if (!pEntityCreator)
		return;

	Entity* pEntity = pEntityCreator->create();
	pEntity->id(eid);
	pEntity->className(entityType);

	Mailbox* baseMB = new Mailbox();
	pEntity->base(baseMB);
	baseMB->id = eid;
	baseMB->className = entityType;
	baseMB->type = Mailbox::MAILBOX_TYPE_BASE;

	entities_.Add(eid, pEntity);

	MemoryStream** entityMessageFind = bufferedCreateEntityMessage_.Find(eid);
	if (entityMessageFind)
	{
		MemoryStream* entityMessage = *entityMessageFind;
		Client_onUpdatePropertys(*entityMessage);
		bufferedCreateEntityMessage_.Remove(eid);
		MemoryStream::reclaimObject(entityMessage);
	}

	pEntity->__init__();
	pEntity->inited(true);

	if (pArgs_->isOnInitCallPropertysSetMethods)
		pEntity->callPropertysSetMethods();
}

ENTITY_ID KBEngineApp::getAoiEntityIDFromStream(MemoryStream& stream)
{
	ENTITY_ID id = 0;

	if (!pArgs_->useAliasEntityID)
	{
		stream >> id;
		return id;
	}

	if (entityIDAliasIDList_.Num()> 255)
	{
		stream >> id;
	}
	else
	{
		uint8 aliasID = 0;
		stream >> aliasID;

		// ���Ϊ0�ҿͻ�����һ�����ص�½���������������ҷ����entity�ڶ����ڼ�һֱ��������״̬
		// ����Ժ����������, ��Ϊcellapp����һֱ����baseapp����ͬ����Ϣ�� ���ͻ���������ʱδ��
		// ����˳�ʼ�����迪ʼ���յ�ͬ����Ϣ, ��ʱ����ͻ����
		if (entityIDAliasIDList_.Num() <= aliasID)
			return 0;

		id = entityIDAliasIDList_[aliasID];
	}

	return id;
}

void KBEngineApp::Client_onUpdatePropertysOptimized(MemoryStream& stream)
{
	ENTITY_ID eid = getAoiEntityIDFromStream(stream);
	onUpdatePropertys_(eid, stream);
}

void KBEngineApp::Client_onUpdatePropertys(MemoryStream& stream)
{
	ENTITY_ID eid;
	stream >> eid;
	onUpdatePropertys_(eid, stream);
}

void KBEngineApp::onUpdatePropertys_(ENTITY_ID eid, MemoryStream& stream)
{
	Entity** pEntityFind = entities_.Find(eid);

	if (!pEntityFind)
	{
		MemoryStream** entityMessageFind = bufferedCreateEntityMessage_.Find(eid);
		if (entityMessageFind)
		{
			ERROR_MSG("entity(%d) not found!", eid);
			return;
		}

		MemoryStream* stream1 = MemoryStream::createObject();
		stream1->append(stream);
		stream1->rpos(stream.rpos() - 4);
		bufferedCreateEntityMessage_.Add(eid, stream1);
		return;
	}
	
	Entity* pEntity = *pEntityFind;

	ScriptModule* sm = EntityDef::moduledefs[pEntity->className()];
	TMap<uint16, Property*>& pdatas = sm->idpropertys;

	while (stream.length() > 0)
	{
		uint16 utype = 0;

		if (sm->usePropertyDescrAlias)
		{
			utype = stream.read<uint8>();
		}
		else
		{
			utype = stream.read<uint16>();
		}

		Property* propertydata = pdatas[utype];
		utype = propertydata->properUtype;
		EntityDefMethodHandle* pSetMethod = propertydata->pSetMethod;

		KBVar* val = propertydata->pUtype->createFromStream(stream);
		// DEBUG_MSG("entity.className + "(id=" + eid  + " " + 
		// propertydata.name + "=" + val->c_str() + "), hasSetMethod=" + setmethod + "!");

		EntityDefPropertyHandle* pEntityDefPropertyHandle = EntityDefPropertyHandles::find(pEntity->className(), propertydata->name);
		if (!pEntityDefPropertyHandle)
		{
			ERROR_MSG("%s not found property(%s), update error!", *pEntity->className(), *propertydata->name);
			delete val;
			continue;
		}

		KBVar* oldval = pEntityDefPropertyHandle->getPropertyValue(pEntity);
		pEntityDefPropertyHandle->setPropertyValue(pEntity, val);

		if (pSetMethod)
		{
			if (propertydata->isBase())
			{
				if (pEntity->inited())
					pSetMethod->callMethod(pEntity, oldval);
			}
			else
			{
				if (pEntity->inWorld())
					pSetMethod->callMethod(pEntity, oldval);
			}
		}

		delete oldval;
		delete val;
	}
}

void KBEngineApp::Client_onEntityDestroyed(int32 eid)
{

}

void KBEngineApp::clearSpace(bool isall)
{

}

void KBEngineApp::clearEntities(bool isall)
{

}

void KBEngineApp::onImportClientMessagesCompleted()
{
	DEBUG_MSG("successfully! currserver=%s, currstate=%s", *currserver_, *currstate_);

	if (currserver_ == TEXT("loginapp"))
	{
		if (!isImportServerErrorsDescr_ && !loadingLocalMessages_)
		{
			DEBUG_MSG("send importServerErrorsDescr!");
			isImportServerErrorsDescr_ = true;
			Bundle* pBundle = Bundle::createObject();
			pBundle->newMessage(Messages::getSingleton().messages[TEXT("Loginapp_importServerErrorsDescr"]));
			pBundle->send(pNetworkInterface_);
		}

		if (currstate_ == TEXT("login"))
		{
			login_loginapp(false);
		}
		else if (currstate_ == TEXT("autoimport"))
		{
		}
		else if (currstate_ == TEXT("resetpassword"))
		{
			resetpassword_loginapp(false);
		}
		else if (currstate_ == TEXT("createAccount"))
		{
			createAccount_loginapp(false);
		}
		else {
		}

		loginappMessageImported_ = true;
	}
	else
	{
		baseappMessageImported_ = true;

		if (!entitydefImported_ && !loadingLocalMessages_)
		{
			DEBUG_MSG("send importEntityDef(%d) ...", entitydefImported_);
			Bundle* pBundle = Bundle::createObject();
			pBundle->newMessage(Messages::getSingleton().messages[TEXT("Baseapp_importClientEntityDef"]));
			pBundle->send(pNetworkInterface_);
			//Event.fireOut("Baseapp_importClientEntityDef", new object[]{});
		}
		else
		{
			onImportEntityDefCompleted();
		}
	}
}

void KBEngineApp::createDataTypeFromStreams(MemoryStream& stream, bool canprint)
{
	uint16 aliassize = 0;
	stream >> aliassize;

	DEBUG_MSG("importAlias(size=%d)!", aliassize);

	while (aliassize > 0)
	{
		aliassize--;
		createDataTypeFromStream(stream, canprint);
	};

	for (auto& item : EntityDef::datatypes)
	{
		if (item.Value)
		{
			item.Value->bind();
		}
	}
}

void KBEngineApp::createDataTypeFromStream(MemoryStream& stream, bool canprint)
{
	uint16 utype = 0;
	stream >> utype;

	FString name;
	stream >> name;

	FString valname;
	stream >> valname;

	/* ��һЩ�������ͣ�������Ҫ�ṩһ��Ψһ���Ʒŵ�datatypes��
		�磺
		<onRemoveAvatar>
		<Arg>	ARRAY <of> INT8 </of>		</Arg>
		</onRemoveAvatar>
	*/
	if (valname.Len() == 0)
		valname = FString::Printf(TEXT("Null_%d"), (int)utype);

	if (canprint)
		DEBUG_MSG("importAlias(%s:%s:%d)!", *name, *valname, utype);

	if (name == TEXT("FIXED_DICT"))
	{
		KBEDATATYPE_FIXED_DICT* datatype = new KBEDATATYPE_FIXED_DICT();
		uint8 keysize;
		stream >> keysize;

		stream >> datatype->implementedBy;

		while (keysize > 0)
		{
			keysize--;

			FString keyname;
			stream >> keyname;

			uint16 keyutype;
			stream >> keyutype;

			datatype->dicttype_map.Add(keyname, keyutype);
		};

		EntityDef::datatypes.Add(valname, datatype);
	}
	else if (name == TEXT("ARRAY"))
	{
		uint16 uitemtype;
		stream >> uitemtype;

		KBEDATATYPE_ARRAY* datatype = new KBEDATATYPE_ARRAY();
		datatype->tmpset_uitemtype = (int)uitemtype;
		EntityDef::datatypes.Add(valname, datatype);
	}
	else
	{
		// ���ܻ��ظ���map��ӻ������ͣ� ��ʱ��Ҫ���˵�
		if (EntityDef::datatypes.Contains(valname))
			return;

		KBEDATATYPE_BASE* val = NULL;
		if (EntityDef::datatypes.Contains(name))
			val = EntityDef::datatypes[name];

		EntityDef::datatypes.Add(valname, val);
	}

	EntityDef::id2datatypes.Add(utype, EntityDef::datatypes[valname]);

	// ���û��Զ�������Ͳ��䵽ӳ�����
	EntityDef::datatype2id.Add(valname, utype);

}

void KBEngineApp::Client_onImportClientEntityDef(MemoryStream& stream)
{
	TArray<uint8> datas;
	datas.SetNumUninitialized(stream.length());
	memcpy(datas.GetData(), stream.data() + stream.rpos(), stream.length());

	onImportClientEntityDef(stream);

	if (persistentInfos_)
		persistentInfos_->onImportClientEntityDef(datas);
}

void KBEngineApp::onImportClientEntityDef(MemoryStream& stream)
{
	createDataTypeFromStreams(stream, true);

	while (stream.length() > 0)
	{
		FString scriptmodule_name;
		stream >> scriptmodule_name;

		uint16 scriptUtype;
		stream >> scriptUtype;

		uint16 propertysize;
		stream >> propertysize;

		uint16 methodsize;
		stream >> methodsize;

		uint16 base_methodsize;
		stream >> base_methodsize;

		uint16 cell_methodsize;
		stream >> cell_methodsize;

		DEBUG_MSG("import(%s), propertys(%d), "
			"clientMethods(%d), baseMethods(%d), cellMethods(%d)!", *scriptmodule_name,
			propertysize, methodsize, base_methodsize, cell_methodsize);

		ScriptModule* module = new ScriptModule(scriptmodule_name);
		EntityDef::moduledefs.Add(scriptmodule_name, module);
		EntityDef::idmoduledefs.Add(scriptUtype, module);

		EntityCreator* pEntityCreator = module->pEntityCreator;

		while (propertysize > 0)
		{
			propertysize--;

			uint16 properUtype;
			stream >> properUtype;

			uint32 properFlags;
			stream >> properFlags;

			int16 paliasID;
			stream >> paliasID;

			FString pname;
			stream >> pname;

			FString defaultValStr;
			stream >> defaultValStr;

			uint16 iutype;
			stream >> iutype;
			KBEDATATYPE_BASE* utype = EntityDef::id2datatypes[iutype];

			EntityDefMethodHandle* pEntityDefMethodHandle = NULL;

			if (pEntityCreator)
				pEntityDefMethodHandle = EntityDefMethodHandles::find(scriptmodule_name, FString::Printf(TEXT("set_%s"), *pname));

			Property* savedata = new Property();
			savedata->name = pname;
			savedata->pUtype = utype;
			savedata->properUtype = properUtype;
			savedata->properFlags = properFlags;
			savedata->aliasID = paliasID;
			savedata->defaultValStr = defaultValStr;
			savedata->pSetMethod = pEntityDefMethodHandle;
			savedata->pdefaultVal = savedata->pUtype->parseDefaultValStr(savedata->defaultValStr);

			module->propertys.Add(pname, savedata);

			if (paliasID >= 0)
			{
				module->usePropertyDescrAlias = true;
				module->idpropertys.Add(paliasID, savedata);
			}
			else
			{
				module->usePropertyDescrAlias = false;
				module->idpropertys.Add(properUtype, savedata);
			}

			DEBUG_MSG("add(%s), property(%s/%d), hasSetMethod=%s.", *scriptmodule_name, *pname, 
				properUtype, (savedata->pSetMethod ? TEXT("true") : TEXT("false")));
		};

		while (methodsize > 0)
		{
			methodsize--;

			uint16 methodUtype;
			stream >> methodUtype;

			int16 ialiasID;
			stream >> ialiasID;

			FString name;
			stream >> name;

			uint8 argssize;
			stream >> argssize;

			TArray<KBEDATATYPE_BASE*> args;

			while (argssize > 0)
			{
				argssize--;
				uint16 tmpType;
				stream >> tmpType;
				args.Add(EntityDef::id2datatypes[tmpType]);
			};

			Method* savedata = new Method();
			savedata->name = name;
			savedata->methodUtype = methodUtype;
			savedata->aliasID = ialiasID;
			savedata->args = args;

			if (pEntityCreator)
				savedata->pEntityDefMethodHandle = EntityDefMethodHandles::find(scriptmodule_name, name);

			module->methods.Add(name, savedata);

			if (ialiasID >= 0)
			{
				module->useMethodDescrAlias = true;
				module->idmethods.Add(ialiasID, savedata);
			}
			else
			{
				module->useMethodDescrAlias = false;
				module->idmethods.Add(methodUtype, savedata);
			}

			DEBUG_MSG("add(%s), method(%s).", *scriptmodule_name, *name);
		};

		while (base_methodsize > 0)
		{
			base_methodsize--;

			uint16 methodUtype;
			stream >> methodUtype;

			int16 ialiasID;
			stream >> ialiasID;

			FString name;
			stream >> name;

			uint8 argssize;
			stream >> argssize;

			TArray<KBEDATATYPE_BASE*> args;

			while (argssize > 0)
			{
				argssize--;
				uint16 tmpType;
				stream >> tmpType;
				args.Add(EntityDef::id2datatypes[tmpType]);
			};

			Method* savedata = new Method();
			savedata->name = name;
			savedata->methodUtype = methodUtype;
			savedata->aliasID = ialiasID;
			savedata->args = args;

			module->base_methods.Add(name, savedata);
			module->idbase_methods.Add(methodUtype, savedata);

			DEBUG_MSG("add(%s), base_method(%s).", *scriptmodule_name, *name);
		};

		while (cell_methodsize > 0)
		{
			cell_methodsize--;

			uint16 methodUtype;
			stream >> methodUtype;

			int16 ialiasID;
			stream >> ialiasID;

			FString name;
			stream >> name;

			uint8 argssize;
			stream >> argssize;

			TArray<KBEDATATYPE_BASE*> args;

			while (argssize > 0)
			{
				argssize--;
				uint16 tmpType;
				stream >> tmpType;
				args.Add(EntityDef::id2datatypes[tmpType]);
			};

			Method* savedata = new Method();
			savedata->name = name;
			savedata->methodUtype = methodUtype;
			savedata->aliasID = ialiasID;
			savedata->args = args;

			module->cell_methods.Add(name, savedata);
			module->idcell_methods.Add(methodUtype, savedata);

			DEBUG_MSG("add(%s), cell_method(%s).", *scriptmodule_name, *name);
		};

		if (!pEntityCreator)
		{
			ERROR_MSG("module(%s) not found!", *scriptmodule_name);
		}

		for(auto& e : module->methods)
		{
			FString name = e.Key;
			
			if (pEntityCreator && !EntityDefMethodHandles::find(scriptmodule_name, name))
			{
				WARNING_MSG("%s:: method(%s) no implement!", *scriptmodule_name, *name);
			}
		};
	};

	onImportEntityDefCompleted();
}

void KBEngineApp::onImportEntityDefCompleted()
{
	DEBUG_MSG("successfully!");
	entitydefImported_ = true;

	if (!loadingLocalMessages_)
		login_baseapp(false);
}

void KBEngineApp::Client_onImportClientMessages(MemoryStream& stream)
{
	TArray<uint8> datas;
	datas.SetNumUninitialized(stream.length());
	memcpy(datas.GetData(), stream.data() + stream.rpos(), stream.length());

	onImportClientMessages(stream);

	if (persistentInfos_)
		persistentInfos_->onImportClientMessages(currserver_, datas);
}

void KBEngineApp::onImportClientMessages(MemoryStream& stream)
{
	uint16 msgcount = 0;
	stream >> msgcount;

	DEBUG_MSG("start currserver=%s(msgsize=%d)...", *currserver_, msgcount);

	while (msgcount > 0)
	{
		msgcount--;

		MessageID msgid = 0;
		stream >> msgid;

		int16 msglen = 0;
		stream >> msglen;

		FString msgname;
		stream >> msgname;
		
		int8 argstype = 0;
		stream >> argstype;

		uint8 argsize = 0;
		stream >> argsize;

		TArray<uint8> argstypes;

		for (uint8 i = 0; i<argsize; i++)
		{
			uint8 v = 0;
			stream >> v;
			argstypes.Add(v);
		}

		Message* handler = Messages::getSingleton().findMessage(msgname);
		bool isClientMethod = msgname.Contains(TEXT("Client_"), ESearchCase::CaseSensitive, ESearchDir::FromEnd);

		if (isClientMethod)
		{
			if (handler == NULL)
			{
				WARNING_MSG("currserver[%s]: interface(%s/%d/%d) no implement!",
					*currserver_, *msgname, msgid, msglen);
			}
			else
			{
				DEBUG_MSG("imported(%s/%d/%d) successfully!", 
					*msgname, msgid, msglen);
			}
		}

		if (handler)
		{
			FString old_cstr = handler->c_str();
			handler->id = msgid;
			handler->msglen = msglen;

			// ��Ϊ������IDһ��ʼ��ʱ����Ϊ������ ������Ҫ��������ȷ��ID��ӵ��б�
			if (isClientMethod)
				Messages::getSingleton().add(handler, msgid, msgname, msglen);

			if (handler->id <= 0)
			{
				DEBUG_MSG("currserver[%s]: refreshed(%s => %s)!",
					*currserver_, *old_cstr, *handler->c_str());
			}
		}
		else
		{
			if (msgname != TEXT(""))
			{
				Messages::getSingleton().messages.Add(msgname, new Message(msgid, msgname, msglen));

				if(!isClientMethod)
					DEBUG_MSG("currserver[%s]: imported(%s/%d/%d) successfully!",
						*currserver_, *msgname, msgid, msglen);

				if (isClientMethod)
				{
					Messages::getSingleton().clientMessages.Add(msgid, Messages::getSingleton().messages[msgname]);
				}
				else
				{
					if (currserver_ == TEXT("loginapp"))
						Messages::getSingleton().loginappMessages.Add(msgid, Messages::getSingleton().messages[msgname]);
					else
						Messages::getSingleton().baseappMessages.Add(msgid, Messages::getSingleton().messages[msgname]);
				}
			}
			else
			{
				Message* msg = new Message(msgid, msgname, msglen);

				if(!isClientMethod)
					DEBUG_MSG("currserver[%s]: imported(%s/%d/%d) successfully!",
						*currserver_, *msgname, msgid, msglen);

				if (currserver_ == TEXT("loginapp"))
					Messages::getSingleton().loginappMessages.Add(msgid, msg);
				else
					Messages::getSingleton().baseappMessages.Add(msgid, msg);
			}
		}
	};

	onImportClientMessagesCompleted();
}

void KBEngineApp::resetpassword_loginapp(bool noconnect)
{

}

void KBEngineApp::createAccount_loginapp(bool noconnect)
{

}

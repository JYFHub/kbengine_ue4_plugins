
#include "KBEnginePluginsPrivatePCH.h"
#include "Bundle.h"
#include "MemoryStream.h"
#include "Message.h"
#include "NetworkInterface.h"

Bundle::Bundle():
	pCurrPacket_(NULL),
	streams_(),
	numMessage_(0),
	messageLength_(0),
	pMsgtype_(NULL),
	curMsgStreamIndex_(0)
{
	pCurrPacket_ = MemoryStream::createObject();
}

Bundle::~Bundle()
{
	MemoryStream::reclaimObject(pCurrPacket_);
	pCurrPacket_ = NULL;
}

Bundle* Bundle::createObject()
{
	return new Bundle();
}

void Bundle::reclaimObject(Bundle* obj)
{
	delete obj;
}

void Bundle::newMessage(Message* pMsg)
{
	fini(false);

	pMsgtype_ = pMsg;
	numMessage_ += 1;

	(*this) << pMsgtype_->id;

	if (pMsgtype_->msglen == -1)
	{
		uint16 lengseat = 0;
		(*this) << lengseat;
		messageLength_ = 0;
	}

	curMsgStreamIndex_ = 0;
}

void Bundle::fini(bool issend)
{
	if (numMessage_ > 0)
	{
		writeMsgLength();

		streams_.Add(pCurrPacket_);
		pCurrPacket_ = MemoryStream::createObject();
	}

	if (issend)
	{
		numMessage_ = 0;
		pMsgtype_ = NULL;
	}

	curMsgStreamIndex_ = 0;
}

void Bundle::send(NetworkInterface* pNetworkInterface)
{
	fini(true);

	if (pNetworkInterface->valid())
	{
		for (int i = 0; i<streams_.Num(); ++i)
		{
			MemoryStream* stream = streams_[i];
			pNetworkInterface->send(stream);
		}
	}
	else
	{
		ERROR_MSG("networkInterface invalid!");
	}

	// �Ѳ��õ�MemoryStream�Żػ���أ��Լ����������յ�����
	for (int i = 0; i < streams_.Num(); ++i)
	{
		MemoryStream::reclaimObject(streams_[i]);
	}

	streams_.Empty();

	if(pCurrPacket_)
		pCurrPacket_->clear(true);

	// ������Ϊ��������ɣ�����Ϊ���bundle����ʹ���ˣ�
	// �������ǻ�����Żض���أ��Լ����������մ��������ģ�
	// �����Ҫ����ʹ�ã�Ӧ������Bundle.createObject()��
	// ������治����createObject()��ֱ��ʹ�ã��Ϳ��ܻ����Ī�������⣬
	// ���Դ˱�ע����ʾʹ���ߡ�
	Bundle::reclaimObject(this);
}

void Bundle::writeMsgLength()
{
	if (pMsgtype_->msglen != -1)
		return;

	if (curMsgStreamIndex_ > 0)
	{
		pCurrPacket_ = streams_[streams_.Num() - curMsgStreamIndex_];
	}

	uint8* data = pCurrPacket_->data();
	data[2] = (uint8)(messageLength_ & 0xff);
	data[3] = (uint8)(messageLength_ >> 8 & 0xff);
}

void Bundle::checkStream(uint32 v)
{
	if (v > pCurrPacket_->space())
	{
		streams_.Add(pCurrPacket_);
		pCurrPacket_ = MemoryStream::createObject();
		++curMsgStreamIndex_;
	}

	messageLength_ += v;
}

Bundle &Bundle::operator<<(uint8 value)
{
	checkStream(sizeof(uint8));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(uint16 value)
{
	checkStream(sizeof(uint16));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(uint32 value)
{
	checkStream(sizeof(uint32));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(uint64 value)
{
	checkStream(sizeof(uint64));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(int8 value)
{
	checkStream(sizeof(int8));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(int16 value)
{
	checkStream(sizeof(int16));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(int32 value)
{
	checkStream(sizeof(int32));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(int64 value)
{
	checkStream(sizeof(int64));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(float value)
{
	checkStream(sizeof(float));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(double value)
{
	checkStream(sizeof(double));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(bool value)
{
	checkStream(sizeof(int8));
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(const FString &value)
{
	const TCHAR *serializedChar = value.GetCharArray().GetData();
	uint32 len = FCString::Strlen(serializedChar);

	// +1Ϊ�ַ���β����0λ��
	checkStream(len + 1);
	(*pCurrPacket_) << value;
	return *this;
}

Bundle &Bundle::operator<<(const char *str)
{
	// +1Ϊ�ַ���β����0λ��
	uint32 len = (uint32)strlen(str) + 1; 

	checkStream(len);
	(*pCurrPacket_) << str;
	return *this;
}

void Bundle::appendBlob(const TArray<uint8>& datas)
{
	uint32 len = (uint32)datas.Num() + 4/*len size*/;

	checkStream(len);

	(*pCurrPacket_).appendBlob(datas);
}
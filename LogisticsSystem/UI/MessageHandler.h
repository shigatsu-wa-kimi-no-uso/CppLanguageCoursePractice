#pragma once
#ifndef EventHandler_h__
#define EventHandler_h__
#include "../Config/typedef.h"
#include "Message.h"

/*
* �¼�������
* ���ڴ������������¼�����Ϣ, ������Esc���˳���Ϣ��
*/

class MessageHandler
{
public:
	void handleMessage(Message* pMsg);
};

#endif // EventHandler_h__
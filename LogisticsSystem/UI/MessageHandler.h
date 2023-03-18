#pragma once
#ifndef EventHandler_h__
#define EventHandler_h__
#include "../Config/typedef.h"
#include "Message.h"

/*
* 事件处理类
* 用于处理程序的所有事件和消息, 包括如Esc键退出消息等
*/

class MessageHandler
{
public:
	void handleMessage(Message* pMsg);
};

#endif // EventHandler_h__
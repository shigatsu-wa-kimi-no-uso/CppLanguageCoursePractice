#pragma once
#ifndef UserInterface_h__
#define UserInterface_h__
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include <conio.h>
#include "Message.h"

#define BEGIN_KEYBOARD_EVENT_MAP(clazz) Message clazz::processKeyboardEvents(){\
		do {\
			unsigned char c = _getch();\
			KeyEvent e = (KeyEvent)c;\
			switch (e){

#define BIND_RESPONSE_FUNC(keyEvent,responseFunc) case keyEvent: responseFunc(); break

#define BIND_NEEDRETURN_RESPONSE_FUNC(keyEvent,responseFunc) case keyEvent: return responseFunc()

#define END_KEYBOARD_EVENT_MAP }} while (1);}

#define SWITCH_UI(_targetUI) UserInterface::targetUI=_targetUI; return {Message::SWITCH_INTERFACE}

namespace UI
{
	
	/*
	* 交互界面类, 其他界面为该界面的派生类
	* 为实现多态, 定义了相关虚函数
	* void paint() : 打印界面的函数, 将当前UI需要的字符打印在控制台上
	* void processKeyboardEvents() : 处理键盘消息的函数, 负责处理用户的键盘敲击信息
	*/

	class UserInterface	
	{
	protected:
		virtual void _printHeader(const char* title);
		virtual void _printFooter() = 0;
	
	public:

		static int targetUI;
		static UserInterface* pCurrentUI;
		virtual void paint() = 0;
		virtual Message processKeyboardEvents() = 0;
		virtual ~UserInterface();
		enum
		{
			MAIN_MENU,
			ITEM_MANAGER,
			ADD_ITEM,
			ITEM_BATCH_INFO,
			PURCHASE_ITEM,
			SALE_ITEM,
			PURCHASE_RECORD,
			SALES_RECORD,
			ITEM_INFO,
			STOCK_CHECK
		};

	};

}

#endif // UserInterface_h__

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
	* ����������, ��������Ϊ�ý����������
	* Ϊʵ�ֶ�̬, ����������麯��
	* void paint() : ��ӡ����ĺ���, ����ǰUI��Ҫ���ַ���ӡ�ڿ���̨��
	* void processKeyboardEvents() : ���������Ϣ�ĺ���, �������û��ļ����û���Ϣ
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

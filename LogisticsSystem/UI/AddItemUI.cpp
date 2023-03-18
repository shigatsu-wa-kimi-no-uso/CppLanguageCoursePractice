#include "AddItemUI.h"
#include "../Utility/ConsoleUtil.h"
#include "../Config/enumeration.h"
#include "MessageBoxUI.h"
#include <conio.h>
using namespace Util;

namespace UI
{
	AddItemUI::AddItemUI() : newItemInfo({0}) ,savedItemInfo({0}),saved(false) {}

	BEGIN_KEYBOARD_EVENT_MAP(AddItemUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
	END_KEYBOARD_EVENT_MAP

	BEGIN_TEXTEDIT_OPTION_MAP(AddItemUI, 64)
		BIND_TEXTEDIT_OPTION(AddItemOption::ITEM_NAME,newItemInfo.itemName,24);
		BIND_TEXTEDIT_OPTION(AddItemOption::TYPE, newItemInfo.typeName,16);
		BIND_TEXTEDIT_OPTION(AddItemOption::PRODUCER,  newItemInfo.producer,24);
		BIND_TEXTEDIT_OPTION(AddItemOption::SPEC,  newItemInfo.spec,28);
	END_TEXTEDIT_OPTION_MAP

	BEGIN_OPTION_STRING_MAP(AddItemUI, AddItemOption::ENUMCNT)
		BIND_OPTION_STRINGS(AddItemOption::ITEM_NAME, "��Ʒ����", newItemInfo.itemName);
		BIND_OPTION_STRINGS(AddItemOption::TYPE, "����", newItemInfo.typeName);
		BIND_OPTION_STRINGS(AddItemOption::PRODUCER, "������", newItemInfo.producer);
		BIND_OPTION_STRINGS(AddItemOption::SPEC, "���", newItemInfo.spec);
	END_OPTION_STRING_MAP



	AddItemOption AddItemUI::getSelectableUpperBound() {
		return AddItemOption::ENUMCNT;
	}

	AddItemOption AddItemUI::getSelectableLowerBound() {
		return AddItemOption::ITEM_NAME;
	}


	const char* AddItemUI::getTitle(){
		return "��д��Ʒ��Ϣ";
	}

	const char* AddItemUI::getFooterFormatWhenEditing() {
		return "%10s|";
	};
	const char* AddItemUI::getFooterFormatWhenSelecting() {
		return "%14s|%18s|%12s|%12s|";
	}
	const void** AddItemUI::getFooterPrintArgsWhenEditing() {
		static const void* args[] = { "ENTER ȷ��"};
		return args;
	}

	const void** AddItemUI::getFooterPrintArgsWhenSelecting() {
		static const void* args[] = { "���� �л�ѡ��","ESC ������һҳ��","ENTER �༭","F1 ���" };
		return args;
	}

	Message AddItemUI::onKeyEsc() {
		SWITCH_UI(UserInterface::ITEM_MANAGER);
	}

	Message AddItemUI::onKeyF1() {
		QWORD itemUID;
		bool isDuplicate;
		if (strlen(newItemInfo.itemName) == 0 
			|| strlen(newItemInfo.producer) == 0
			|| strlen(newItemInfo.typeName) == 0
			|| strlen(newItemInfo.spec) == 0 ){
			MessageBoxUI::pop("����������д��Ϣ", 1500);
			return { Message::PAINT_INTERFACE };
		}
		else if (itemInfoSvc.addItemInfo(newItemInfo,itemUID,isDuplicate)) {
			char msg[256];
			sprintf_s(msg, "%s %llu", "��ӳɹ�, ��ƷUIDΪ:", itemUID);
			MessageBoxUI::pop(msg, 1500);
			return { Message::PAINT_INTERFACE };
		}

		if (isDuplicate) {
			MessageBoxUI::pop("���ʧ��,��Ʒ�Ѿ�����", 1500);
		} else {
			MessageBoxUI::pop("���ʧ��", 1500);
		}
		return { Message::PAINT_INTERFACE};
	}

}
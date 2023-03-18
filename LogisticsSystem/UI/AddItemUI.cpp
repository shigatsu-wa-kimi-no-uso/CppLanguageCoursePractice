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
		BIND_OPTION_STRINGS(AddItemOption::ITEM_NAME, "商品名称", newItemInfo.itemName);
		BIND_OPTION_STRINGS(AddItemOption::TYPE, "类型", newItemInfo.typeName);
		BIND_OPTION_STRINGS(AddItemOption::PRODUCER, "生产商", newItemInfo.producer);
		BIND_OPTION_STRINGS(AddItemOption::SPEC, "规格", newItemInfo.spec);
	END_OPTION_STRING_MAP



	AddItemOption AddItemUI::getSelectableUpperBound() {
		return AddItemOption::ENUMCNT;
	}

	AddItemOption AddItemUI::getSelectableLowerBound() {
		return AddItemOption::ITEM_NAME;
	}


	const char* AddItemUI::getTitle(){
		return "填写商品信息";
	}

	const char* AddItemUI::getFooterFormatWhenEditing() {
		return "%10s|";
	};
	const char* AddItemUI::getFooterFormatWhenSelecting() {
		return "%14s|%18s|%12s|%12s|";
	}
	const void** AddItemUI::getFooterPrintArgsWhenEditing() {
		static const void* args[] = { "ENTER 确定"};
		return args;
	}

	const void** AddItemUI::getFooterPrintArgsWhenSelecting() {
		static const void* args[] = { "↑↓ 切换选项","ESC 返回上一页面","ENTER 编辑","F1 添加" };
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
			MessageBoxUI::pop("请完整地填写信息", 1500);
			return { Message::PAINT_INTERFACE };
		}
		else if (itemInfoSvc.addItemInfo(newItemInfo,itemUID,isDuplicate)) {
			char msg[256];
			sprintf_s(msg, "%s %llu", "添加成功, 商品UID为:", itemUID);
			MessageBoxUI::pop(msg, 1500);
			return { Message::PAINT_INTERFACE };
		}

		if (isDuplicate) {
			MessageBoxUI::pop("添加失败,商品已经存在", 1500);
		} else {
			MessageBoxUI::pop("添加失败", 1500);
		}
		return { Message::PAINT_INTERFACE};
	}

}
#include "ItemInfoUI.h"
using namespace Util;

namespace UI
{
	StringifiedItemInfo ItemInfoUI::itemInfo;
	int ItemInfoUI::lastUI;
	QWORD ItemInfoUI::itemUID;

	BEGIN_KEYBOARD_EVENT_MAP(ItemInfoUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
	END_KEYBOARD_EVENT_MAP

	BEGIN_TEXTEDIT_OPTION_MAP(ItemInfoUI, 1)
	END_TEXTEDIT_OPTION_MAP

	BEGIN_OPTION_STRING_MAP(ItemInfoUI, ItemInfoEntry::ENUMCNT)
		BIND_OPTION_STRINGS(ItemInfoEntry::ITEM_NAME, "商品名称", itemInfo.itemName);
		BIND_OPTION_STRINGS(ItemInfoEntry::TYPE_NAME, "类型", itemInfo.typeName);
		BIND_OPTION_STRINGS(ItemInfoEntry::PRODUCER, "生产商", itemInfo.producer);
		BIND_OPTION_STRINGS(ItemInfoEntry::SPEC, "规格", itemInfo.spec);
		BIND_OPTION_STRINGS(ItemInfoEntry::ITEM_UID, "商品UID", [this] {
			static char buf[24]; 
		sprintf_s(buf, "%llu", itemUID); 
		return buf; }()
			);
	END_OPTION_STRING_MAP

	void ItemInfoUI::setItemInfo(StringifiedItemInfo& info,QWORD itemUID) {
		ItemInfoUI::itemInfo = info;
		ItemInfoUI::itemUID = itemUID;
	}

	void ItemInfoUI::setLastUI(int ui) {
		lastUI = ui;
	}

	ItemInfoEntry ItemInfoUI::getSelectableUpperBound() {
		return ItemInfoEntry::ENUMCNT;
	}

	ItemInfoEntry ItemInfoUI::getSelectableLowerBound() {
		return ItemInfoEntry::ITEM_NAME;
	}


	const char* ItemInfoUI::getTitle() {
		return "商品信息";
	}

	const char* ItemInfoUI::getFooterFormatWhenEditing() {
		return "";
	};
	const char* ItemInfoUI::getFooterFormatWhenSelecting() {
		return "%14s";
	}
	const void** ItemInfoUI::getFooterPrintArgsWhenEditing() {
		static const void* args[] = { "" };
		return args;
	}

	const void** ItemInfoUI::getFooterPrintArgsWhenSelecting() {
		static const void* args[] = { "ESC 返回上一页面" };
		return args;
	}

	Message ItemInfoUI::onKeyEsc() {
		SWITCH_UI(lastUI);
	}

}
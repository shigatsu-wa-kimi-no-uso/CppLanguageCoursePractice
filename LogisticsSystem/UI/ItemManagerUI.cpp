#include "ItemManagerUI.h"
#include "ItemBatchInfoUI.h"
#include "../Utility/ConsoleUtil.h"
#include "EditBoxUI.h"

using namespace Util;

namespace UI
{
	char ItemManagerUI::filterStr[256];
	bool ItemManagerUI::onFilter;

	ItemManagerUI::ItemManagerUI() {
		_assignPageIter();
	}
	
	ItemManagerUI::~ItemManagerUI() {
	}

	BEGIN_KEYBOARD_EVENT_MAP(ItemManagerUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F4, onKeyF4);
	END_KEYBOARD_EVENT_MAP

	const char* ItemManagerUI::getTitle() {
		return "商品浏览";
	}

	const char* ItemManagerUI::getHeaderFormat() {
		return "%4s|%24s|%16s|%28s|%24s|%8s|%12s|";
	}

	const void** ItemManagerUI::getHeaderPrintArgs() {
		static const void* headerStr[] = { "序号","商品名","类型","规格","生产商","库存","状态" };
		return headerStr;
	}

	const char* ItemManagerUI::getFooterFormat(){
		return "%14s|%18s|%12s|%9s|%9s|";
	}

	const void** ItemManagerUI::getFooterPrintArgs() {
		static const void* footerStr[] = { "↑↓ 选择商品","ESC 返回上一页面","ENTER 查看库存详情","F1 添加商品",""};
		footerStr[4] = onFilter ? "F4 取消筛选" : "F4 模糊筛选";
		return footerStr;
	}

	const char* ItemManagerUI::getEntryFormat() {
		return "%4d|%24s|%16s|%28s|%24s|%8s|%12s|";
	}

	const void** ItemManagerUI::getEntryPrintArgs(const ItemInfoList::Iterator& iter, const DWORD order) {
		static StringifiedItemInfo info;
		itemInfoService.getStringifiedItemInfo(*iter, info);
		static const void* entryStr[7];
		entryStr[0] = (void*)order;
		entryStr[1] = info.itemName;
		entryStr[2] = info.typeName;
		entryStr[3] = info.spec;
		entryStr[4] = info.producer;
		entryStr[5] = info.remainingCount;
		entryStr[6] = info.status;
		return entryStr;
	}
	
	const DWORD ItemManagerUI::getPageElemCntLimit() {
		return PageElemCntLimit;
	}

	const ItemInfoList::Iterator ItemManagerUI::getBeginIterator() {
		if (onFilter) {
			return itemInfoService.getItemInfoListIteratorFilteredBySubstring(filterStr);
		} else {
			return itemInfoService.getItemInfoListIterator();
		}
	}

	const ItemInfoList::Iterator ItemManagerUI::getEndIterator() {
		if (onFilter) {
			return itemInfoService.getItemInfoListEndIteratorFiltered();
		} else {
			return itemInfoService.getItemInfoListEndIterator();
		}
	}

	Message ItemManagerUI::onKeyEsc() {
		refreshPageIter();
		SWITCH_UI(UserInterface::MAIN_MENU);
	}


	Message ItemManagerUI::onKeyF1() {
		SWITCH_UI(UserInterface::ADD_ITEM);
	}

	Message ItemManagerUI::onKeyEnter() {
		ItemBatchInfoUI::setTargetItem(pageIter.getFocusedIterator());
		ItemBatchInfoUI::refreshPageIter();
		SWITCH_UI(UserInterface::ITEM_BATCH_INFO);
	}

	Message ItemManagerUI::onKeyF4() {
		if (!onFilter) {
			EditBoxUI::pop("请输入要查找的字符串", filterStr, 256);
			if (strlen(filterStr) >= 1) {
				onFilter = true;
			} else {
				onFilter = false;
			}
		} else {
			onFilter = false;
		}
		refreshPageIter();
		_assignPageIter();
		return { Message::PAINT_INTERFACE };
	}
}
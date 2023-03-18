#include "SalesRecordUI.h"
#include "ItemBatchInfoUI.h"
#include "../Utility/ConsoleUtil.h"
#include "ItemInfoUI.h"
#include "EditBoxUI.h"

using namespace Util;

namespace UI
{

	time_t SalesRecordUI::timeUB;
	time_t SalesRecordUI::timeLB;
	char SalesRecordUI::filterStr[256];
	bool SalesRecordUI::stringFilterOn;
	bool SalesRecordUI::timeFilterOn;


	SalesRecordUI::SalesRecordUI(){
		_assignPageIter();
	}

	SalesRecordUI::~SalesRecordUI() {}

	BEGIN_KEYBOARD_EVENT_MAP(SalesRecordUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F2, onKeyF2);
	END_KEYBOARD_EVENT_MAP


	const char* SalesRecordUI::getTitle() {
		return "销售记录";
	}

	const char* SalesRecordUI::getHeaderFormat() {
		return "%10s|%32s|%10s|%12s|%10s|%10s|%10s|%18s";
	}

	const void** SalesRecordUI::getHeaderPrintArgs() {
		static const void* headerStr[] = { "记录号","商品缩略信息","批次UID","售出量","售出单价","净利润","售出后库存","时间"};
		return headerStr;
	}

	const char* SalesRecordUI::getFooterFormat() {
		return "%14s|%27s|%12s|%9s|%9s";
	}

	const void** SalesRecordUI::getFooterPrintArgs() {
		static const void* footerStr[] = { "↑↓ 选择","ESC 返回上一页面" ,"ENTER 查看商品完整信息",nullptr,nullptr };
		footerStr[3] = timeFilterOn ? "F1 关闭时间筛选" : "F1 时间段筛选";
		footerStr[4] = stringFilterOn ? "F2 关闭信息筛选" : "F2 模糊查找商品信息";
		return footerStr;
	}

	const char* SalesRecordUI::getEntryFormat() {
		return "%10d|%32s|%10llu|%12d|%10.2f|%10.2f|%10d|%18s";
	}

	const void** SalesRecordUI::getEntryPrintArgs(const SalesRecordList::Iterator& iter, const DWORD order) {
		static union {
			double real;
			long long integer;
			char* string;
		}entryStr[9];
		static char timeStr[24];
		ConsoleUtil::getTimeString(iter->recordTime, timeStr);
		StringifiedItemInfo strInfo;
		ItemInfo info;
		info.info.itemUID = iter->itemUID;
		itemInfoService.getStringifiedItemInfo(info, strInfo);
		static char itemStr[128];
		sprintf_s(itemStr, "%s-%s-%s", strInfo.itemName, strInfo.spec, strInfo.producer);
		ConsoleUtil::truncateStringWithDots(itemStr, 32);
		entryStr[0].integer = iter->recordUID;
		entryStr[1].string = itemStr;
		entryStr[2].integer = iter->batchID;
		entryStr[3].integer = iter->soldCount;
		entryStr[4].real = iter->unitPrice;
		entryStr[5].real = iter->netProfit;
		entryStr[6].integer = iter->remainingCount;
		entryStr[7].string = timeStr;
		return (const void**)entryStr;
	}

	const DWORD SalesRecordUI::getPageElemCntLimit() {
		return PageElemCntLimit;
	}

	const SalesRecordList::Iterator SalesRecordUI::getBeginIterator() {
		if (stringFilterOn) {
			return salesService.getSalesRecordListIteratorFilteredBySubstring(filterStr, timeLB, timeUB);
		}
		return salesService.getSalesRecordListIterator(timeLB, timeUB);
	}

	const SalesRecordList::Iterator SalesRecordUI::getEndIterator() {
		if (stringFilterOn) {
			return salesService.getSalesRecordListEndIteratorFiltered();
		}
		return salesService.getSalesRecordListEndIterator(timeLB, timeUB);
	}

	Message SalesRecordUI::onKeyEsc() {
		refreshPageIter();
		SWITCH_UI(UserInterface::MAIN_MENU);
	}

	Message SalesRecordUI::onKeyEnter() {
		ItemInfo infoEntry;
		infoEntry.info.itemUID = pageIter.getFocusedIterator()->itemUID;
		StringifiedItemInfo infoStr;
		itemInfoService.getStringifiedItemInfo(infoEntry, infoStr);
		ItemInfoUI::setItemInfo(infoStr,infoEntry.info.itemUID);
		ItemInfoUI::setLastUI(UserInterface::SALES_RECORD);
		SWITCH_UI(UserInterface::ITEM_INFO);
	}
	Message SalesRecordUI::onKeyF1() {
		if (!timeFilterOn) {
			char temp[256];
			EditBoxUI::pop("请输入时间段(格式如:2021-03-15 18:00:12 - 2021-03-25 18:25:12):", temp, 256);
			this->timeLB = ConsoleUtil::getTimestamp(temp);
			this->timeUB = ConsoleUtil::getTimestamp(temp + 22);
			timeFilterOn = true;
		} else {
			timeFilterOn = false;
			this->timeLB = -1;
			this->timeUB = -1;
		}
		refreshPageIter();
		_assignPageIter();
		return { Message::PAINT_INTERFACE };
	}

	Message SalesRecordUI::onKeyF2() {
		if (!stringFilterOn) {
			EditBoxUI::pop("请输入要查找的字符串", filterStr, 256);
			if (strlen(filterStr) >= 1) {
				stringFilterOn = true;
			} else {
				stringFilterOn = false;
			}
		} else {
			stringFilterOn = false;
		}
		refreshPageIter();
		_assignPageIter();
		return { Message::PAINT_INTERFACE };
	}


}
#include "PurchaseRecordUI.h"
#include "ItemBatchInfoUI.h"
#include "ItemInfoUI.h"
#include "EditBoxUI.h"
#include "MessageBoxUI.h"
#include "../Utility/ConsoleUtil.h"

using namespace Util;

namespace UI
{
	time_t PurchaseRecordUI::timeLB;
	time_t PurchaseRecordUI::timeUB;
	char PurchaseRecordUI::filterStr[256];
	bool PurchaseRecordUI::stringFilterOn;
	bool PurchaseRecordUI::timeFilterOn;

	PurchaseRecordUI::PurchaseRecordUI() {
		_assignPageIter();
	}

	PurchaseRecordUI::~PurchaseRecordUI() {}

	BEGIN_KEYBOARD_EVENT_MAP(PurchaseRecordUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F2, onKeyF2);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
	END_KEYBOARD_EVENT_MAP


	const char* PurchaseRecordUI::getTitle() {
		return "�ɹ���¼";
	}

	const char* PurchaseRecordUI::getHeaderFormat() {
		return "%10s|%32s|%10s|%12s|%10s|%10s|%10s|%18s";
	}

	const void** PurchaseRecordUI::getHeaderPrintArgs() {
		static const void* headerStr[] = { "��¼��","��Ʒ������Ϣ","����UID","�ɹ���","�ɹ�����","�ɱ�","�ɹ�����","ʱ��" };
		return headerStr;
	}

	const char* PurchaseRecordUI::getFooterFormat() {
		return "%10s|%14s|%12s|%9s|%9s";
	}

	const void** PurchaseRecordUI::getFooterPrintArgs() {
		static const void* footerStr[] = { "���� ѡ��","ESC ������һҳ��" ,"ENTER �鿴�й���Ʒ������Ϣ","",""};
		footerStr[3] = timeFilterOn ? "F1 �ر�ʱ��ɸѡ" : "F1 ʱ���ɸѡ";
		footerStr[4] = stringFilterOn ? "F2 �ر���Ϣɸѡ" : "F2 ģ��������Ʒ��Ϣ";
		return footerStr;
	}

	const char* PurchaseRecordUI::getEntryFormat() {
		return "%10d|%32s|%10llu|%12d|%10.2f|%10.2f|%10d|%18s";
	}

	const void** PurchaseRecordUI::getEntryPrintArgs(const PurchaseRecordList::Iterator& iter, const DWORD order) {
		static union {
			double real;
			long long integer;
			char* string;
		}entryStr[8];
		static char timeStr[24];
		ConsoleUtil::getTimeString(iter->timeOfPurchase, timeStr);
		entryStr[0].integer = iter->recordUID;
		StringifiedItemInfo strInfo;
		ItemInfo info;
		info.info.itemUID = iter->itemUID;
		itemInfoService.getStringifiedItemInfo(info, strInfo);
		static char itemStr[128];

		sprintf_s(itemStr, "%s-%s-%s",strInfo.itemName,strInfo.spec,strInfo.producer);
		ConsoleUtil::truncateStringWithDots(itemStr, 32);
		entryStr[1].string = itemStr;
		entryStr[2].integer = iter->batchID;
		entryStr[3].integer = iter->purchasedAmount;
		entryStr[4].real = iter->unitCost;
		entryStr[5].real = iter->purchasedAmount * iter->unitCost;
		entryStr[6].integer = iter->remainingCount;
		entryStr[7].string = timeStr;
		return (const void**)entryStr;
	}


	const DWORD PurchaseRecordUI::getPageElemCntLimit() {
		return PageElemCntLimit;
	}

	const PurchaseRecordList::Iterator PurchaseRecordUI::getBeginIterator() {
		if (stringFilterOn){
			return purchaseService.getPurchaseRecordListIteratorFilteredBySubstring(filterStr,timeLB,timeUB);
		} 
		return purchaseService.getPurchaseRecordListIterator(timeLB, timeUB);
	}

	const PurchaseRecordList::Iterator PurchaseRecordUI::getEndIterator() {
		if (stringFilterOn) {
			return purchaseService.getPurchaseRecordListEndIteratorFiltered();
		}
		return purchaseService.getPurchaseRecordListEndIterator(timeLB,timeUB);
	}

	Message PurchaseRecordUI::onKeyEnter() {
		ItemInfo infoEntry;
		infoEntry.info.itemUID = pageIter.getFocusedIterator()->itemUID;
		StringifiedItemInfo infoStr;
		itemInfoService.getStringifiedItemInfo(infoEntry,infoStr);
		ItemInfoUI::setItemInfo(infoStr,infoEntry.info.itemUID);
		ItemInfoUI::setLastUI(UserInterface::PURCHASE_RECORD);
		SWITCH_UI(UserInterface::ITEM_INFO);
	}

	Message PurchaseRecordUI::onKeyEsc() {
		refreshPageIter();
		SWITCH_UI(UserInterface::MAIN_MENU);
	}


	Message PurchaseRecordUI::onKeyF1() {
		if (!timeFilterOn) {
			char temp[256];
			EditBoxUI::pop("������ʱ���(��ʽ��:2021-03-15 18:00:12 - 2021-03-25 18:25:12):", temp, 256);
			this->timeLB = ConsoleUtil::getTimestamp(temp);
			this->timeUB = ConsoleUtil::getTimestamp(temp + 22);
			if (this->timeLB == -1 || this->timeUB == -1) {
				MessageBoxUI::pop("ʱ���ʽ����", 1500);
			}
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

	Message PurchaseRecordUI::onKeyF2() {
		if (!stringFilterOn) {
			EditBoxUI::pop("������Ҫ���ҵ��ַ���", filterStr, 256);
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
#include "ItemBatchInfoUI.h"
#include "ItemBatchInfoUI.h"
#include "PurchaseItemUI.h"
#include "SaleItemUI.h"
#include "../Utility/ConsoleUtil.h"
#include <conio.h>
using namespace Util;

namespace UI
{
	ItemInfoList::Iterator ItemBatchInfoUI::targetItem;

	ItemBatchInfoUI::ItemBatchInfoUI() {
		_assignPageIter();
	}

	ItemBatchInfoUI::~ItemBatchInfoUI() {
	}

	BEGIN_KEYBOARD_EVENT_MAP(ItemBatchInfoUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F3, onKeyF3);
	END_KEYBOARD_EVENT_MAP

	const char* ItemBatchInfoUI::getTitle() {
		return "�������";
	}

	const char* ItemBatchInfoUI::getHeaderFormat() {
		return "%4s|%10s|%20s|%10s|%10s|%10s|%10s|%10s|%10s|%10s|%9s";
	}
	const void** ItemBatchInfoUI::getHeaderPrintArgs() {
		static const void* args[] = { 
			"���",
			"����",
			"�ɹ�ʱ��",
			"�ɹ���",
			"���",
			"������",
			"�ɹ��ɱ�",
			"�ɹ�����",
			"���۶�",
			"����",
			"״̬"
		};
		return args;
	}

	const char* ItemBatchInfoUI::getFooterFormat() {
		return "%14s|%18s|%9s|%9s|";
	}
	const void** ItemBatchInfoUI::getFooterPrintArgs() {
		static const void* args[] = { "���� ѡ����", "ESC ������һҳ��", "F1 �ɹ�", "F3 ����" };
		return args;
	}
	const void** ItemBatchInfoUI::getEntryPrintArgs(const ItemBatchInfoList::Iterator& iter, const DWORD order) {
		static StringifiedItemBatchInfo info;
		itemBatchInfoService.getStringifiedItemBatchInfo(*iter, info);
		static const void* args[11];
		args[0] = (void*)order;
		args[1] = info.batchUID;
		args[2] = info.timeOfPurchase;
		args[3] = info.purchasedAmount;
		args[4] = info.remainingCount;
		args[5] = info.soldCount;
		args[6] = info.cost;
		args[7] = info.unitCost;
		args[8] = info.profit;
		args[9] = info.netProfit;
		args[10] = info.status;
		return args;
	}

	const DWORD ItemBatchInfoUI::getPageElemCntLimit() {
		return PageElemCntLimit;
	}

	const ItemBatchInfoList::Iterator ItemBatchInfoUI::getBeginIterator() {
		return itemBatchInfoService.getItemBatchInfoListIterator(targetItem->info.itemUID);
	}

	const ItemBatchInfoList::Iterator ItemBatchInfoUI::getEndIterator() {
		return itemBatchInfoService.getItemBatchInfoListEndIterator(targetItem->info.itemUID);
	}

	const char* ItemBatchInfoUI::getEntryFormat() {
		return "%4d|%10s|%20s|%10s|%10s|%10s|%10s|%10s|%10s|%10s|%9s";
	}

	void ItemBatchInfoUI::setTargetItem(ItemInfoList::Iterator targetItem) {
		ItemBatchInfoUI::targetItem = targetItem;
	}

	Message ItemBatchInfoUI::onKeyEsc() {
		refreshPageIter();
		SWITCH_UI(UserInterface::ITEM_MANAGER);
	}

	Message ItemBatchInfoUI::onKeyF1() {
		PurchaseItemUI::setTargetItem(targetItem);
		SWITCH_UI(UserInterface::PURCHASE_ITEM);
	}

	Message ItemBatchInfoUI::onKeyF3() {
		SaleItemUI::setTargetBatch(pageIter.getFocusedIterator());
		SaleItemUI::setTargetItem(targetItem);
		SWITCH_UI(UserInterface::SALE_ITEM);
	}

	Message ItemBatchInfoUI::onKeyF4() {
		ItemBatchInfoList::Iterator target = pageIter.getFocusedIterator();
		refreshPageIter();
		_assignPageIter();
		return { Message::PAINT_INTERFACE };
	}
}
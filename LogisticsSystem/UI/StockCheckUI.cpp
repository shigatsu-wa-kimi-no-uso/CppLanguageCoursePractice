#include "StockCheckUI.h"
#include "ItemBatchInfoUI.h"
#include "../Utility/ConsoleUtil.h"
#include "ItemInfoUI.h"
#include "EditBoxUI.h"
#include "../Algorithm/Algorithm.h"

using namespace Util;

namespace UI
{
	char StockCheckUI::filterStr[256];
	bool StockCheckUI::filterOn;
	bool StockCheckUI::sorting;
	void (*StockCheckUI::sortPolicy)(bool);
	LinkedList<ItemInfo*> StockCheckUI::itemInfoList;

	StockCheckUI::StockCheckUI(){
		_assembleItemInfoList();
		_assignPageIter();
	}

	StockCheckUI::~StockCheckUI() {}

	void  StockCheckUI::_sortItemInfoListByTotalSoldCount(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalSoldCount < rgt->totalSoldCount;
		};
		auto inDescending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalSoldCount > rgt->totalSoldCount;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}

	void  StockCheckUI::_sortItemInfoListByRecentSoldCount(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->recentSoldCount < rgt->recentSoldCount;
		};
		auto inDescending=[](ItemInfo* lft, ItemInfo* rgt) {
			return lft->recentSoldCount > rgt->recentSoldCount;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}

	void  StockCheckUI::_sortItemInfoListByTotalCost(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalCost < rgt->totalCost;
		};
		auto inDescending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalCost > rgt->totalCost;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}

	void  StockCheckUI::_sortItemInfoListByTotalSalesFigure(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalSalesFigure < rgt->totalSalesFigure;
		};
		auto inDescending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->totalSalesFigure > rgt->totalSalesFigure;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}

	void  StockCheckUI::_sortItemInfoListByNetProfit(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->netProfit < rgt->netProfit;
		};
		auto inDescending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->netProfit > rgt->netProfit;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}

	void  StockCheckUI::_sortItemInfoListByRemainingCount(bool ascending) {
		auto inAscending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->remainingCount < rgt->remainingCount;
		};
		auto inDescending = [](ItemInfo* lft, ItemInfo* rgt) {
			return lft->remainingCount > rgt->remainingCount;
		};
		if (ascending) {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inAscending);
		} else {
			Algorithm::quickSort(itemInfoList.begin(), itemInfoList.end(), inDescending);
		}
	}


	void StockCheckUI::_assembleItemInfoList() {
		ItemInfoList::Iterator st;
		ItemInfoList::Iterator end;
		if (iterAssigned) return;
		itemInfoList.clear();
		if (filterOn) {
			st = itemInfoService.getItemInfoListIteratorFilteredBySubstring(filterStr);
			end = itemInfoService.getItemInfoListEndIteratorFiltered();
		} else {
			st = itemInfoService.getItemInfoListIterator();
			end = itemInfoService.getItemInfoListEndIterator();
		}
		for (; st != end; st._setSelfNext()) {
			itemInfoList.pushBack(&*st);
		}

	}

	BEGIN_KEYBOARD_EVENT_MAP(StockCheckUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F4, onKeyF4);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
	END_KEYBOARD_EVENT_MAP

	const char* StockCheckUI::getTitle() {
		return "�����̵�";
	}

	const char* StockCheckUI::getHeaderFormat() {
		return "%5s|%32s|%10s|%12s|%10s|%12s|%12s|%12s|%10s";
	}

	const void** StockCheckUI::getHeaderPrintArgs() {
		static const void* headerStr[] = { "���","��Ʒ������Ϣ","���","������","��һ������","�����۶�","�ܳɱ�","������" ,"״̬"};
		return headerStr;
	}

	const char* StockCheckUI::getFooterFormat() {
		return "%14s|%18s|%27s|%12s|%9s|";
	}

	const void** StockCheckUI::getFooterPrintArgs() {
		static const void* footerStr[] = { "���� ѡ����Ʒ","ESC ������һҳ��","ENTER �鿴��Ʒ������Ϣ","F1 ����","" };
		footerStr[4] = filterOn ? "F4 ȡ��ɸѡ" : "F4 ģ��ɸѡ";
		return footerStr;
	}

	const char* StockCheckUI::getEntryFormat() {
		return "%5d|%32s|%10u|%12u|%10u|%12.2f|%12.2f|%12.2f|%10s";
	}

	const void** StockCheckUI::getEntryPrintArgs(const LinkedList<ItemInfo*>::Iterator& iter, const DWORD order) {
		static union {
			double real;
			long long integer;
			char* string;
		}entryStr[9];
		static char timeStr[24];
		entryStr[0].integer = order;
		static StringifiedItemInfo strInfo;
		ItemInfo* info = *iter;
		itemInfoService.getStringifiedItemInfo(*info, strInfo);
		static char itemStr[128];
		sprintf_s(itemStr, "%s-%s-%s", strInfo.itemName, strInfo.spec, strInfo.producer);
		ConsoleUtil::truncateStringWithDots(itemStr, 32);
		//  "���","��Ʒ������Ϣ","���","������","��һ������","�����۶�","�ܳɱ�","������" ,"״̬"
		entryStr[1].string = itemStr;
		entryStr[2].integer = info->remainingCount;
		entryStr[3].integer = info->totalSoldCount;
		entryStr[4].integer = info->recentSoldCount;
		entryStr[5].real = info->totalSalesFigure;
		entryStr[6].real = info->totalCost;
		entryStr[7].real = info->netProfit;
		entryStr[8].string = strInfo.status;
		return (const void**)entryStr;
	}

	const DWORD StockCheckUI::getPageElemCntLimit() {
		return PageElemCntLimit;
	}


	const LinkedList<ItemInfo*>::Iterator StockCheckUI::getBeginIterator() {
		return itemInfoList.begin();
	}

	const LinkedList<ItemInfo*>::Iterator StockCheckUI::getEndIterator() {
		return itemInfoList.end();
	}

	Message StockCheckUI::onKeyEsc() {
		SWITCH_UI(UserInterface::MAIN_MENU);
	}

	Message StockCheckUI::onKeyEnter() {
		ItemInfo infoEntry;
		infoEntry.info.itemUID = (*pageIter.getFocusedIterator())->info.itemUID;
		StringifiedItemInfo infoStr;
		itemInfoService.getStringifiedItemInfo(infoEntry, infoStr);
		ItemInfoUI::setItemInfo(infoStr, infoEntry.info.itemUID);
		ItemInfoUI::setLastUI(UserInterface::STOCK_CHECK);
		SWITCH_UI(UserInterface::ITEM_INFO);
	}


	Message StockCheckUI::onKeyF1() {
		//"���", "������", "��һ������", "�����۶�", "�ܳɱ�", "������"
		char sortPolicyStr[64];
		EditBoxUI::pop("����������ʽ(��:������ ����)", sortPolicyStr, 64);
		char target[32], ord[32];
		sscanf_s(sortPolicyStr, "%s %s", target, 32, ord, 32);
		ascending = strcmp("����", ord) == 0;

		if (strcmp("��һ������", target) == 0) {
			sortPolicy = _sortItemInfoListByRecentSoldCount;
		} else if (strcmp("���", target) == 0) {
			sortPolicy = _sortItemInfoListByRemainingCount;
		} else if (strcmp("�����۶�", target) == 0) {
			sortPolicy = _sortItemInfoListByTotalSalesFigure;
		} else if (strcmp("�ܳɱ�", target) == 0) {
			sortPolicy = _sortItemInfoListByTotalCost;
		} else if (strcmp("������", target) == 0) {
			sortPolicy = _sortItemInfoListByNetProfit;
		} else if (strcmp("������", target) == 0) {
			sortPolicy = _sortItemInfoListByTotalSoldCount;
		} else {
			sortPolicy = nullptr;
		}
		if (sortPolicy != nullptr) {
			sortPolicy(ascending);
			sorting = true;
		} else {
			sorting = false;
		}
		
		return { Message::PAINT_INTERFACE };
	}


	Message StockCheckUI::onKeyF4() {
		if (!filterOn) {
			EditBoxUI::pop("������Ҫ���ҵ��ַ���", filterStr, 256);
			if (strlen(filterStr) >= 1) {
				filterOn = true;
			} else {
				filterOn = false;
			}
		} else {
			filterOn = false;
		}
		
		refreshPageIter();
		_assembleItemInfoList();
		if (sorting) {
			sortPolicy(ascending);
		}
		_assignPageIter();

		return { Message::PAINT_INTERFACE };
	}
}
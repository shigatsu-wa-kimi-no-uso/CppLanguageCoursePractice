#pragma once
#include "ListTemplateUI.h"
#include "../Service/ItemInfoService.h"

namespace UI
{
	class StockCheckUI : public ListTemplateUI<LinkedList<ItemInfo*>::Iterator>
	{

	private:
		using ItemInfoService = Service::ItemInfoService;
		//header
		ItemInfoService itemInfoService;
		static LinkedList<ItemInfo*> itemInfoList;
		bool ascending;
		static void (*sortPolicy)(bool);
		void _assembleItemInfoList();
		static void _sortItemInfoListByTotalSoldCount(bool ascending);
		static void _sortItemInfoListByRecentSoldCount(bool ascending);
		static void _sortItemInfoListByTotalCost(bool ascending);
		static void _sortItemInfoListByTotalSalesFigure(bool ascending);
		static void _sortItemInfoListByNetProfit(bool ascending);
		static void _sortItemInfoListByRemainingCount(bool ascending);

	protected:
		Message onKeyEsc();
		Message onKeyF1();
		Message onKeyF4();
		Message onKeyEnter();
		static char filterStr[256];
		static bool filterOn;
		static bool sorting;
		virtual ~StockCheckUI() override;
		virtual const char* getTitle() override;
		virtual const char* getHeaderFormat() override;
		virtual const void** getHeaderPrintArgs() override;
		virtual const char* getFooterFormat() override;
		virtual const void** getFooterPrintArgs() override;
		virtual const void** getEntryPrintArgs(const LinkedList<ItemInfo*>::Iterator& iter, const DWORD order) override;
		virtual const DWORD getPageElemCntLimit() override;
		virtual const LinkedList<ItemInfo*>::Iterator getBeginIterator() override;
		virtual const LinkedList<ItemInfo*>::Iterator getEndIterator() override;
		virtual const char* getEntryFormat() override;
	public:
		StockCheckUI();
		virtual Message processKeyboardEvents() override;
	};
}


#pragma once
#ifndef ItemBatchInfoUI_h__
#define ItemBatchInfoUI_h__
#include "ListTemplateUI.h"
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "../Service/ItemBatchInfoService.h"
#include "../Service/ItemInfoService.h"
#include "../Container/ScrollIterator.h"

namespace UI
{
	class ItemBatchInfoUI : public ListTemplateUI<ItemBatchInfoList::Iterator>
	{
	private:
		static ItemInfoList::Iterator targetItem;
		Service::ItemBatchInfoService itemBatchInfoService;
		Service::ItemInfoService itemInfoService;
	protected:
		Message onKeyEsc();
		Message onKeyF1();
		Message onKeyF3();
		Message onKeyF4();
		virtual ~ItemBatchInfoUI() override;
		virtual Message processKeyboardEvents() override;
		virtual const char* getTitle() override;
		virtual const char* getHeaderFormat() override;
		virtual const void** getHeaderPrintArgs() override;
		virtual const char* getFooterFormat() override;
		virtual const void** getFooterPrintArgs() override;
		virtual const void** getEntryPrintArgs(const ItemBatchInfoList::Iterator& iter, const DWORD order) override;
		virtual const DWORD getPageElemCntLimit() override;
		virtual const ItemBatchInfoList::Iterator getBeginIterator() override;
		virtual const ItemBatchInfoList::Iterator getEndIterator() override;
		virtual const char* getEntryFormat() override;
	public:
		ItemBatchInfoUI();
	
		static void setTargetItem(ItemInfoList::Iterator targetItem);
	};

}
#endif // ItemBatchInfoUI_h__
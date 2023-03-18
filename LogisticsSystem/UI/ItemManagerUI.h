#pragma once
#ifndef PurchaseUI_h__
#define PurchaseUI_h__
#include "UserInterface.h"
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "../Service/ItemInfoService.h"
#include "../Container/ScrollIterator.h"
#include "ListTemplateUI.h"

namespace UI
{
	class ItemManagerUI : public ListTemplateUI<ItemInfoList::Iterator>
	{

	private:
		using ItemInfoService = Service::ItemInfoService;
		//header
		ItemInfoService itemInfoService;
		Message onKeyEsc();
		Message onKeyF1();
		Message onKeyEnter();
		Message onKeyF4();
		static char filterStr[256];
		static bool onFilter;
	protected:
		virtual ~ItemManagerUI();
		virtual const char* getTitle() override;
		virtual const char* getHeaderFormat() override;
		virtual const void** getHeaderPrintArgs() override;
		virtual const char* getFooterFormat() override;
		virtual const void** getFooterPrintArgs() override;
		virtual const void** getEntryPrintArgs(const ItemInfoList::Iterator& iter, const DWORD order) override;
		virtual const DWORD getPageElemCntLimit() override;
		virtual const ItemInfoList::Iterator getBeginIterator() override;
		virtual const ItemInfoList::Iterator getEndIterator() override;
		virtual const char* getEntryFormat() override;
	public:
		ItemManagerUI();
		virtual Message processKeyboardEvents() override;
	};

}
#endif // PurchaseUI_h__
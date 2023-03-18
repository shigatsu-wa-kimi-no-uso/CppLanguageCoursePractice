#pragma once
#ifndef PurchaseRecordUI_h__
#define PurchaseRecordUI_h__
#include "UserInterface.h"
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "../Service/PurchaseService.h"
#include "../Service/ItemInfoService.h"
#include "../Container/ScrollIterator.h"
#include "ListTemplateUI.h"

namespace UI
{
	class PurchaseRecordUI : public ListTemplateUI<PurchaseRecordList::Iterator>
	{

	private:
		//header
		Service::PurchaseService purchaseService;
		Service::ItemInfoService itemInfoService;
		static time_t timeLB;
		static time_t timeUB;
		static char filterStr[256];
		static bool stringFilterOn;
		static bool timeFilterOn;
		Message onKeyEsc();
		Message onKeyF1();
		Message onKeyEnter();
		Message onKeyF2();

	protected:
		virtual Message processKeyboardEvents() override;
		virtual const char* getTitle() override;
		virtual const char* getHeaderFormat() override;
		virtual const void** getHeaderPrintArgs() override;
		virtual const char* getFooterFormat() override;
		virtual const void** getFooterPrintArgs() override;
		virtual const void** getEntryPrintArgs(const PurchaseRecordList::Iterator& iter, const DWORD order) override;
		virtual const DWORD getPageElemCntLimit() override;
		virtual const PurchaseRecordList::Iterator getBeginIterator() override;
		virtual const PurchaseRecordList::Iterator getEndIterator() override;
		virtual const char* getEntryFormat() override;
	public:
		PurchaseRecordUI();
		virtual ~PurchaseRecordUI();
	};

}
#endif // PurchaseRecordUI_h__
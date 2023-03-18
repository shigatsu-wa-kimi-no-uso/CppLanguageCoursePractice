#pragma once
#ifndef SalesRecordUI_h__
#define SalesRecordUI_h__
#include "UserInterface.h"
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "../Service/SalesService.h"
#include "../Container/ScrollIterator.h"
#include "../Service/ItemInfoService.h"
#include "ListTemplateUI.h"

namespace UI
{
	class SalesRecordUI : public ListTemplateUI<SalesRecordList::Iterator>
	{
	private:
		//header
		static literal DWORD pageElemCntLimit = 8;
		Service::SalesService salesService;
		Service::ItemInfoService itemInfoService;
		static time_t timeUB;
		static time_t timeLB;
		static char filterStr[256];
		static bool stringFilterOn;
		static bool timeFilterOn;
	protected:
		Message onKeyEsc();
		Message onKeyEnter();
		Message onKeyF1();
		Message onKeyF2();
	
		virtual ~SalesRecordUI() override;
		virtual Message processKeyboardEvents() override;
		virtual const char* getTitle() override;
		virtual const char* getHeaderFormat() override;
		virtual const void** getHeaderPrintArgs() override;
		virtual const char* getFooterFormat() override;
		virtual const void** getFooterPrintArgs() override;
		virtual const void** getEntryPrintArgs(const SalesRecordList::Iterator& iter, const DWORD order) override;
		virtual const DWORD getPageElemCntLimit() override;
		virtual const SalesRecordList::Iterator getBeginIterator() override;
		virtual const SalesRecordList::Iterator getEndIterator() override;
		virtual const char* getEntryFormat() override;
	public:
		SalesRecordUI();
	};

}
#endif // SalesRecordUI_h__
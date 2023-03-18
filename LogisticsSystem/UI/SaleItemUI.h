#pragma once
#include "../Config/typedef.h"
#include "../Service/SalesService.h"
#include "../Service/ItemBatchInfoService.h"
#include "../Service/ItemInfoService.h"
#include "../Entity/SalesRecord.h"
#include "TextEditTemplateUI.h"


namespace UI
{
	enum class SaleItemOption
	{
		AMOUNT,
		UNIT_PRICE,
		TOTAL_PRICE,
		ENUMCNT
	};

	class SaleItemUI : public TextEditTemplateUI<SaleItemOption, (int)SaleItemOption::ENUMCNT>
	{
	private:
		static ItemBatchInfoList::Iterator targetBatch;
		static ItemInfoList::Iterator targetItem;
		char saleAmount[16];
		char unitPrice[16];
		Service::SalesService salesSvc;
		Service::ItemBatchInfoService itemBatchInfoSvc;
		void _edit();
		const char* _calculateTotalPrice(const char* saleAmount, const char* unitCost);
	protected:
		Message onKeyEsc();
		Message onKeyF1();
		virtual const char*** getOptionStrings() override;
		virtual Message processKeyboardEvents() override;
		virtual SaleItemOption getSelectableUpperBound();
		virtual SaleItemOption getSelectableLowerBound();
		virtual const char* getTitle();
		virtual const char* getFooterFormatWhenEditing();
		virtual const char* getFooterFormatWhenSelecting();
		virtual const void** getFooterPrintArgsWhenEditing();
		virtual const void** getFooterPrintArgsWhenSelecting();
	public:
		SaleItemUI();
		static void setTargetItem(ItemInfoList::Iterator targetItem);
		static void setTargetBatch(ItemBatchInfoList::Iterator targetBatch);
	};

}
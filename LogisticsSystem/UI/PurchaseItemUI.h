#pragma once
#include "../Config/typedef.h"
#include "../Service/PurchaseService.h"
#include "../Service/ItemBatchInfoService.h"
#include "../Service/ItemInfoService.h"
#include "../Entity/PurchaseRecord.h"
#include "TextEditTemplateUI.h"


namespace UI
{
	enum class PurchaseItemOption
	{
		AMOUNT,
		UNIT_PRICE,
		TOTAL_PRICE,
		ENUMCNT
	};

	class PurchaseItemUI : public TextEditTemplateUI<PurchaseItemOption,(int)PurchaseItemOption::ENUMCNT>
	{
	private:
		static ItemInfoList::Iterator targetItem;
		char purchaseAmount[16];
		char unitCost[16];
		Service::PurchaseService purchaseSvc;
		Service::ItemBatchInfoService itemBatchInfoSvc;
		void _edit();
		const char* _calculateTotalPrice(const char* purchaseAmount, const char* unitCost);
	protected:
		Message onKeyEsc();
		Message onKeyF1();
		virtual const char*** getOptionStrings() override;
		virtual Message processKeyboardEvents() override;
		virtual PurchaseItemOption getSelectableUpperBound();
		virtual PurchaseItemOption getSelectableLowerBound();
		virtual const char* getTitle();
		virtual const char* getFooterFormatWhenEditing();
		virtual const char* getFooterFormatWhenSelecting();
		virtual const void** getFooterPrintArgsWhenEditing();
		virtual const void** getFooterPrintArgsWhenSelecting();
	public:
		static void setTargetItem(ItemInfoList::Iterator targetItem);
		PurchaseItemUI();
	};

}
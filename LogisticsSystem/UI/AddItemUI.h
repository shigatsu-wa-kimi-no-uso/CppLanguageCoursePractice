#pragma once
#include "TextEditTemplateUI.h"
#include "../Entity/StringifiedItemInfo.h"
#include "../Service/ItemInfoService.h"

namespace UI
{
	enum class AddItemOption
	{
		ITEM_NAME,
		TYPE,
		PRODUCER,
		SPEC,
		ENUMCNT
	};

	class AddItemUI : public TextEditTemplateUI<AddItemOption, (int)AddItemOption::ENUMCNT>
	{
	private:
		StringifiedItemInfo newItemInfo;
		StringifiedItemInfo savedItemInfo;
		Service::ItemInfoService itemInfoSvc;
		bool saved;
	public:
		AddItemUI();
		virtual void _edit() override;
		virtual const char*** getOptionStrings() override;
		virtual Message processKeyboardEvents() override;
		virtual AddItemOption getSelectableUpperBound();
		virtual AddItemOption getSelectableLowerBound();
		virtual const char* getTitle();
		virtual const char* getFooterFormatWhenEditing();
		virtual const char* getFooterFormatWhenSelecting();
		virtual const void** getFooterPrintArgsWhenEditing();
		virtual const void** getFooterPrintArgsWhenSelecting();
		Message onKeyEsc();
		Message onKeyF1();
	};

}

#pragma once
#include "TextEditTemplateUI.h"
#include "../Entity/StringifiedItemInfo.h"

namespace UI
{
	enum class ItemInfoEntry
	{
		ITEM_NAME,
		TYPE_NAME,
		PRODUCER,
		SPEC,
		ITEM_UID,
		ENUMCNT
	};

	class ItemInfoUI : public TextEditTemplateUI<ItemInfoEntry,(int)ItemInfoEntry::ENUMCNT>
	{
	protected:
		static StringifiedItemInfo itemInfo;
		static QWORD itemUID;
		
		static int lastUI;
		virtual void _edit() override;
		virtual const char*** getOptionStrings() override;
		virtual Message processKeyboardEvents() override;
		virtual ItemInfoEntry getSelectableUpperBound();
		virtual ItemInfoEntry getSelectableLowerBound();
		virtual const char* getTitle();
		virtual const char* getFooterFormatWhenEditing();
		virtual const char* getFooterFormatWhenSelecting();
		virtual const void** getFooterPrintArgsWhenEditing();
		virtual const void** getFooterPrintArgsWhenSelecting();
		Message onKeyEsc();
	public:
		static void setLastUI(int ui);
		static void setItemInfo(StringifiedItemInfo& info,QWORD itemUID);
	};

}

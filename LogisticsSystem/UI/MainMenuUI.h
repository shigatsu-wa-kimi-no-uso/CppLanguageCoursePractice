#pragma once
#ifndef MainMenu_h__
#define MainMenu_h__
#include "UserInterface.h"
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "../Service/ItemInfoService.h"

namespace UI
{
	class MainMenuUI : public UserInterface
	{
	private:
		enum Option {
			ITEM_MANAGER,
			PURCHASE_RECORD,
			SALES_RECORD,
			STOCK_CHECK,
			GENERATE_ORDER_LIST,
			ENUMCNT
		};
		int currSelection;
		static literal char* title = "欢迎使用抗议物资进销存系统";
		static literal char* optionDisplayStrings[ENUMCNT] = {
			"1.进入库存系统",
			"2.查询采购记录",
			"3.查询销售记录",
			"4.物资盘点",
			"5.生成采购通知单" //顺序应与Option枚举中的顺序对应
		};

		//每次都需要打印的不变的内容(页眉,页脚等)
		void _printHeader(const char* title);
		void _printFooter();
		void _clear();
		Message onKeyEsc();
		Message onKeyUp();
		Message onKeyDown();
		Message onKeyEnter();
		Service::ItemInfoService itemInfoService;
	public:
		MainMenuUI();
		~MainMenuUI();
		void paint() override;
		Message processKeyboardEvents() override;
	};

}
#endif // MainMenu_h__
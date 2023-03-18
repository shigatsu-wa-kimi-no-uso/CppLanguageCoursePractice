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
		static literal char* title = "��ӭʹ�ÿ������ʽ�����ϵͳ";
		static literal char* optionDisplayStrings[ENUMCNT] = {
			"1.������ϵͳ",
			"2.��ѯ�ɹ���¼",
			"3.��ѯ���ۼ�¼",
			"4.�����̵�",
			"5.���ɲɹ�֪ͨ��" //˳��Ӧ��Optionö���е�˳���Ӧ
		};

		//ÿ�ζ���Ҫ��ӡ�Ĳ��������(ҳü,ҳ�ŵ�)
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
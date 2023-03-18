#include "MainMenuUI.h"
#include "../Utility/ConsoleUtil.h"
#include "MessageBoxUI.h"
#include <conio.h>
using namespace Util;

namespace UI
{
	void MainMenuUI::_clear() {
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, 1, 1);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::ERASE_IN_DISPLAY, 2);
	}

	MainMenuUI::MainMenuUI():currSelection(0) {}

	MainMenuUI::~MainMenuUI() {
		_clear();
	}

	BEGIN_KEYBOARD_EVENT_MAP(MainMenuUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER,onKeyEnter);
	END_KEYBOARD_EVENT_MAP

	void MainMenuUI::_printHeader(const char* title) {
		UserInterface::_printHeader(title);
	}

	void MainMenuUI::_printFooter() {
		ConsoleUtil::putChar('*', COLUMN_CHARCOUNT);
	}

	void MainMenuUI::paint() {
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, 1, 1);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::ERASE_IN_DISPLAY, 2);
		_printHeader(title);
		putchar('\n');
		char centeringPaddings[COLUMN_CHARCOUNT + 1];
		ConsoleUtil::getCenteringPaddings(optionDisplayStrings[3], centeringPaddings, ' ', COLUMN_CHARCOUNT);
		for (int i = 0; i < Option::ENUMCNT; ++i) {
			putchar('\n');
			printf(centeringPaddings);
			if (currSelection == i) {
				ConsoleUtil::printfEx(ConsoleUtil::BGCOLOR::BG_BRIGHT_CYAN, ConsoleUtil::FGCOLOR::FG_BLACK, { ConsoleUtil::SGRCODE::NONE }, optionDisplayStrings[i]);
			} else {
				ConsoleUtil::printfEx({ ConsoleUtil::SGRCODE::NONE }, optionDisplayStrings[i]);
			}
			putchar('\n');
		}
		putchar('\n');
		_printFooter();
		putchar('\n');
	}

	Message MainMenuUI::onKeyEsc() {
		return { Message::EXIT_APPLICATION };
	}

	Message MainMenuUI::onKeyUp() {
		currSelection = currSelection == 0 ? 0 : currSelection - 1;
		return { Message::PAINT_INTERFACE };
	}

	Message MainMenuUI::onKeyDown() {
		currSelection = currSelection == (Option::ENUMCNT - 1) ? currSelection : currSelection + 1;
		return { Message::PAINT_INTERFACE };
	}

	Message MainMenuUI::onKeyEnter() {
		switch ((Option)currSelection) 
		{
		case ITEM_MANAGER:
			SWITCH_UI(UserInterface::ITEM_MANAGER);
		case PURCHASE_RECORD:
			SWITCH_UI(UserInterface::PURCHASE_RECORD);
		case SALES_RECORD:
			SWITCH_UI(UserInterface::SALES_RECORD);
		case STOCK_CHECK:
			SWITCH_UI(UserInterface::STOCK_CHECK);
		case GENERATE_ORDER_LIST:
			itemInfoService.generatePurchaseList();
			MessageBoxUI::pop("生成成功,文件地址为'程序所在目录\\采购通知单.txt'", 1500);
			return { Message::PAINT_INTERFACE };
		default:
			break;
		}

	}
}
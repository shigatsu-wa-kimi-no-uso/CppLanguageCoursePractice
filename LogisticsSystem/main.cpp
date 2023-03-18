#include"Utility/ConsoleUtil.h"
#include "UI/UserInterface.h"
#include "UI/MainMenuUI.h"
#include "UI/MessageHandler.h"
using namespace Util;
using namespace UI;

const char* path;



int main(int argc,char** argv) {
	for (int i = strlen(argv[0]); i >= 0; --i) {
		if (argv[0][i] == '\\') {
			argv[0][i] = 0;
			break;
		}
	}
	path = argv[0];
	ConsoleUtil::enableCountFormatSpecifier();
	ConsoleUtil::setConsoleSize(COLUMN_CHARCOUNT, LINE_CHARCOUNT);
	ConsoleUtil::getStdinHandle();
	MessageHandler msgHandler;

	Message msg;
	
	msg = { Message::SWITCH_INTERFACE };
	UserInterface::targetUI = UserInterface::MAIN_MENU;
	msgHandler.handleMessage(&msg);

	while (1) {
		Message msg = UserInterface::pCurrentUI->processKeyboardEvents();
		msgHandler.handleMessage(&msg);
	}

}
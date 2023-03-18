#include "UserInterface.h"
#include "../Utility/ConsoleUtil.h"
#include <string.h>

namespace UI
{
	int UserInterface::targetUI;
	UserInterface* UserInterface::pCurrentUI;

	using namespace Util;
	void UserInterface::_printHeader(const char* title) {
		char buffer[COLUMN_CHARCOUNT + 1];
		ConsoleUtil::putChar('_', COLUMN_CHARCOUNT);
		putchar('\n');
		strcpy_s(buffer, title);
		ConsoleUtil::alignStringCenter(buffer, ' ');
		ConsoleUtil::printfEx({ ConsoleUtil::SGRCODE::UNDERLINE}, buffer);
		putchar('\n');
	}

	UserInterface::~UserInterface() {
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, 1, 1);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::ERASE_IN_DISPLAY, 2);
	}

}
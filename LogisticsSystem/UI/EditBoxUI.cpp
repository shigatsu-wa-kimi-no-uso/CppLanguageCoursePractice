#include "EditBoxUI.h"
#include "../Utility/ConsoleUtil.h"
#include <string.h>

using namespace Util;
namespace UI
{

	void EditBoxUI::pop(const char* message, char* buf, const DWORD bufSize) {
		constexpr int frameLineCnt = 5;
		char frame[COLUMN_CHARCOUNT + 1];
		char paddings[COLUMN_CHARCOUNT + 1];
		DWORD msgLen = strlen(message);
		DWORD boxColumnCnt = msgLen + 10;
		memset(frame, 0, sizeof(frame));
		memset(paddings, 0, sizeof(paddings));
		ConsoleUtil::putChar('-', boxColumnCnt, frame);
		ConsoleUtil::putChar(' ', boxColumnCnt, paddings);
		
		int colpos = ConsoleUtil::getCenteringPaddingsCharCount(frame, COLUMN_CHARCOUNT);
		int linepos = (LINE_CHARCOUNT - 5) / 2;
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("+%s+\n", frame);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("|%s|\n", paddings);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("|%5s%s%5s|\n", " ", message, " ");
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos, colpos + 6);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::SAVE_CURSOR_POSITION);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("|%s|\n", paddings);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("|%s|\n", paddings);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, linepos++, colpos);
		printf("+%s+\n", frame);
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::RESTORE_CURSOR_POSITION);
		gets_s(buf, bufSize);
	}
}
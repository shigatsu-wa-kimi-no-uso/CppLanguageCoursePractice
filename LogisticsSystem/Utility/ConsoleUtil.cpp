#define _CRT_SECURE_NO_WARNINGS
#include "ConsoleUtil.h"
#include <conio.h>
#include <Windows.h>
#include <time.h>

namespace Util
{
	int ConsoleUtil::columnCharCount = -1;

	int ConsoleUtil::lineCharCount = -1;

	HANDLE ConsoleUtil::hStdin;
	HANDLE ConsoleUtil::hStdout;
	HANDLE ConsoleUtil::hStderr;



	void ConsoleUtil::enableCountFormatSpecifier() {
		_set_printf_count_output(1);
	}

	/*
	* 将输入的字符按照GBK编码转换并打印
	* 若输入的是ASCII字符, 则直接打印, 并写入buffer且返回0
	* 若高位为非ASCII范围, 则等待输入第二个字符, 写入buffer, 返回0
	* 若输入的字符ASCII<=32, 则直接返回该值, 不写入buffer
	* byteWritten返回写入的字符所占的字节(用于判断刚刚输入的是否是ASCII字符)
	* 若字符无法识别, 返回-1
	*/
	int ConsoleUtil::getcharEx_GBK(char* buffer, int& byteWritten) {
		unsigned char c = _getch();
		//GBK: 0x8140~0xFEFE excluding 0xXX7F
		if (c > 32 && c < 0x80) {
			buffer[0] = c;
			putchar(c);
			byteWritten = 1;
			return 0;
		} else if (c > 0x80 && c < 0xFF ) {
			buffer[0] = c;
			putchar(c);
			if (c >= 0x40 && c < 0xFF && c != 0x7F) {
				buffer[1] = c;
				putchar(c);
				byteWritten = 2;
				return 0;
			} else {
				return -1;
			}
		} else if (c <= 32 || c == 0xFF) {
			return c;
		} else {
			return -1;
		}
	}

	void ConsoleUtil::backspace() {
		printf("\b \b");
	}


	void ConsoleUtil::putChar(const char ch) {
		putchar(ch);
	}

	void ConsoleUtil::putChar(const char ch, int count) {
		while (count--) {
			putchar(ch);
		}
	}

	void ConsoleUtil::putChar(const char ch, int count, char* buffer) {
		for (int i = 0; i < count; ++i) {
			buffer[i] = ch;
		}
	}

	void ConsoleUtil::getTimeString(time_t timestamp,char* buffer,size_t bufferSize) {
		tm time;
		localtime_s(&time, &timestamp);
		sprintf_s(buffer,bufferSize, "%4d-%02d-%02d %02d:%02d:%02d",
			1900 + time.tm_year,
			1 + time.tm_mon,
			time.tm_mday,
			time.tm_hour,
			time.tm_min,
			time.tm_sec);
	}

	time_t ConsoleUtil::getTimestamp(const char* timeStr) {
		tm time;
		sscanf_s(timeStr, "%4d-%02d-%02d %02d:%02d:%02d",
			&time.tm_year,
			&time.tm_mon,
			&time.tm_mday,
			&time.tm_hour,
			&time.tm_min,
			&time.tm_sec);
		time.tm_year -= 1900;
		time.tm_mon -= 1;
		return mktime(&time);
	}

	void ConsoleUtil::truncateStringWithDots(char* targetStr, const DWORD constraintLen) {
		int offset = 0;
		int i = 0;
		bool isKanji = false;
		while (1) {
			if (i == constraintLen - 3) {
				//汉字判断, 如果28是一半汉字, 则设置28 为空格 29 30 31为'.'
				//否则29 30 31为'.' 28为空格
				if (isKanji) {
					targetStr[i - 1] = ' ';
				}
				targetStr[i] = '.';
				targetStr[i + 1] = '.';
				targetStr[i + 2] = '.';
				break;
			}
			if (targetStr[i] == 0) {
				break;
			} else {
				if ((signed char)targetStr[i] < 0 && isKanji == false) {
					isKanji = true;
				} else {
					isKanji = false;
				}
			}
			++i;
		}
		targetStr[constraintLen] = '\0';
	}

	void ConsoleUtil::getLeftAlignPaddings(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount) {
		size_t targetStrLen = strlen(targetStr);
		size_t paddingCharCount = columnCharCount - targetStrLen;
		for (int i = 0; i < paddingCharCount; ++i) {
			buffer[i] = paddingChar;
		}
		buffer[paddingCharCount] = 0;
	}

	void ConsoleUtil::moveStringRight(const char* targetStr, char* buffer, const char paddingChar, const size_t moveLengthCharCount) {
		size_t targetStrLen = strlen(targetStr);
		if (targetStr != buffer) {
			for (size_t i = 0; i < targetStrLen; ++i) {
				buffer[i + moveLengthCharCount] = targetStr[i];
			}
		} else {
			char* tmp = new char[targetStrLen];
			for (size_t i = 0; i < targetStrLen; ++i) {
				tmp[i] = targetStr[i];
			}
			for (size_t i = 0; i < targetStrLen; ++i) {
				buffer[i + moveLengthCharCount] = tmp[i];
			}
			delete tmp;
		}
		buffer[targetStrLen + moveLengthCharCount] = 0;
		for (size_t i = 0; i < moveLengthCharCount; ++i) {
			buffer[i] = paddingChar;
		}
	}


	void ConsoleUtil::alignStringLeft(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount) {
		size_t targetStrLen = strlen(targetStr);
		if (buffer != targetStr) {
			for (size_t i = 0; i < targetStrLen; ++i) {
				buffer[i] = targetStr[i];
			}
		}
		for (size_t i = targetStrLen; i < columnCharCount;  ++i) {
			buffer[i] = paddingChar;
		}
		buffer[columnCharCount] = 0;
	}


	const char* ConsoleUtil::alignStringCenter(const char* targetStr, const char paddingChar, const size_t columnCharCount) {
		char* buffer = new char[columnCharCount + 1];
		size_t targetStrLen = strlen(targetStr);
		size_t paddingCharCount = (columnCharCount - targetStrLen) / 2;
		int i;
		for (i = 0; i < paddingCharCount; ++i) {
			buffer[i] = paddingChar;
		}
		for (; i - paddingCharCount < targetStrLen; ++i) {
			buffer[i] = targetStr[i - paddingCharCount];
		}
		for (; i - paddingCharCount - targetStrLen < paddingCharCount; ++i) {
			buffer[i] = paddingChar;
		}
		buffer[columnCharCount] = 0;
		return buffer;
	}

	void ConsoleUtil::alignStringCenter(const char* targetStr, char* buffer, const char paddingChar, size_t columnCharCount) {
		size_t targetStrLen = strlen(targetStr);
		size_t paddingsCount = (columnCharCount - targetStrLen) / 2;
		int i;
		char* temp = new char[targetStrLen + 1];
		for (int i = 0; i < targetStrLen; ++i) {
			temp[i] = targetStr[i];
		}

		for (i = 0; i < paddingsCount; ++i) {
			buffer[i] = paddingChar;
		}
		for (; i - paddingsCount < targetStrLen; ++i) {
			buffer[i] = temp[i - paddingsCount];
		}
		for (; i - paddingsCount - targetStrLen < paddingsCount; ++i) {
			buffer[i] = paddingChar;
		}
		buffer[columnCharCount-1] = paddingChar;
		buffer[columnCharCount] = 0;
		delete[] temp;
	}

	void ConsoleUtil::getCenteringPaddings(const char* targetStr, char* destination, const char paddingChar, const size_t columnCharCount) {
		size_t targetStrLen = strlen(targetStr);
		size_t paddingCount = (columnCharCount - targetStrLen) / 2;
		int i;
		for (i = 0; i < paddingCount; ++i) {
			destination[i] = paddingChar;
		}
		destination[i] = 0;
	}

	size_t ConsoleUtil::getCenteringPaddingsCharCount(const char* targetStr, const size_t columnCharCount) {
		size_t targetStrLen = strlen(targetStr);
		return (columnCharCount - targetStrLen) / 2;
	}

	void ConsoleUtil::setConsoleSize(const int columnCharCount, const int lineCharCount) {
		char buf[128];
		ConsoleUtil::columnCharCount = columnCharCount;
		ConsoleUtil::lineCharCount = lineCharCount;
		sprintf_s(buf, "mode con cols=%d lines=%d", columnCharCount, lineCharCount);
		system(buf);
	}

	void ConsoleUtil::runCSIFunction(const CSICODE& csiCode) {
		_runCSIFunction(csiCode);
	}

	void ConsoleUtil::runCSIFunction(const CSICODE& csiCode, const int param1) {
		_runCSIFunction(csiCode, param1);
	}

	void ConsoleUtil::runCSIFunction(const CSICODE& csiCode, const int param1, const int param2) {
		_runCSIFunction(csiCode, param1, param2);
	}

	bool ConsoleUtil::prefillConsoleWindowInput(const char* strFilled) {
		size_t strLen = strlen(strFilled);
		DWORD wszStrLen = MultiByteToWideChar(CP_ACP, 0, strFilled, strLen, NULL, 0);
		WCHAR* wszStrFilled = new WCHAR[wszStrLen];
		MultiByteToWideChar(CP_ACP, 0, strFilled, strLen, wszStrFilled, wszStrLen);
		INPUT_RECORD* inputRecords = new INPUT_RECORD[wszStrLen];
		for (int i = 0; i < wszStrLen; ++i) {
			inputRecords[i].EventType = KEY_EVENT;
			inputRecords[i].Event.KeyEvent.bKeyDown = TRUE;
			inputRecords[i].Event.KeyEvent.dwControlKeyState = 0;
			inputRecords[i].Event.KeyEvent.uChar.UnicodeChar = wszStrFilled[i];
			inputRecords[i].Event.KeyEvent.wRepeatCount = 1;
			inputRecords[i].Event.KeyEvent.wVirtualKeyCode = VkKeyScanW(wszStrFilled[i]);
			WORD vsc = MapVirtualKey(inputRecords[i].Event.KeyEvent.wVirtualKeyCode, MAPVK_VK_TO_VSC);
			inputRecords[i].Event.KeyEvent.wVirtualScanCode = vsc;
		}
		DWORD successCnt;
		WriteConsoleInput(hStdin, inputRecords,wszStrLen, &successCnt);
		delete[] inputRecords;
		return successCnt == wszStrLen;
	}

	void ConsoleUtil::getStdinHandle() {
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
	}

	void ConsoleUtil::getStdoutHandle() {
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	void ConsoleUtil::getStderrHandle() {
		hStderr = GetStdHandle(STD_ERROR_HANDLE);
	}


	void ConsoleUtil::_sprintfEx(char* buf,const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		size_t offset = _putSGRCodes(buf,sgrCodes, sgrCodesCount);
		vsprintf(buf + offset, format, arglist);
		offset = strlen(buf);
		_putSGRCode(buf + offset ,SGRCODE::RESET);
	}

	void ConsoleUtil::_sprintfEx(char* buf, const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		size_t offset = 0;
		offset += _putBGColorCode(buf + offset, bgColor);
		offset += _putFGColorCode(buf + offset, fgColor);
		offset += _putSGRCodes(buf + offset, sgrCodes, sgrCodesCount);
		vsprintf(buf + offset, format, arglist);
		offset = strlen(buf);
		offset +=  _putSGRCode(buf + offset, SGRCODE::RESET);
	}

	void ConsoleUtil::_sprintfEx(char* buf, const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		
		size_t offset = 0;
		offset += _putBG24BitColorCode(buf + offset,bgColor24bit);
		offset += _putFG24BitColorCode(buf + offset, fgColor24bit);
		offset += _putSGRCodes(buf + offset, sgrCodes, sgrCodesCount);
		vsprintf(buf + offset, format, arglist);
		offset = strlen(buf); 
		offset += _putSGRCode(buf + offset, SGRCODE::RESET);
	}

	void ConsoleUtil::_sprintfEx(char* buf, const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		
		size_t offset = 0;
		offset += _putBG256ColorCode(buf + offset, bgColor256);
		offset += _putFG256ColorCode(buf + offset, fgColor256);
		offset += _putSGRCodes(buf + offset, sgrCodes, sgrCodesCount);
		vsprintf(buf + offset, format, arglist);
		offset += strlen(buf);
		offset += _putSGRCode(buf + offset, SGRCODE::RESET);
	}


	void ConsoleUtil::_printfEx(const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		char buffer[2048];
		vsprintf_s(buffer, format, arglist);
		_putSGRCodes(sgrCodes,sgrCodesCount);
		printf(buffer);
		_putSGRCode(SGRCODE::RESET);
	}

	void ConsoleUtil::_printfEx(const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		char buffer[2048];
		vsprintf_s(buffer, format, arglist);
		_putBGColorCode(bgColor);
		_putFGColorCode(fgColor);
		_putSGRCodes(sgrCodes,sgrCodesCount);
		printf(buffer);
		_putSGRCode(SGRCODE::RESET);
	}

	void ConsoleUtil::_printfEx(const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE* sgrCodes,const size_t sgrCodesCount, const char* format, va_list arglist) {
		char buffer[2048];
		vsprintf_s(buffer, format, arglist);
		_putBG24BitColorCode(bgColor24bit);
		_putFG24BitColorCode(fgColor24bit);
		_putSGRCodes(sgrCodes, sgrCodesCount);
		printf(buffer);
		_putSGRCode(SGRCODE::RESET);
	}

	void ConsoleUtil::_printfEx(const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist) {
		char buffer[2048];
		vsprintf_s(buffer, format, arglist);
		_putBG256ColorCode(bgColor256);
		_putFG256ColorCode(fgColor256);
		_putSGRCodes(sgrCodes, sgrCodesCount);
		printf(buffer);
		_putSGRCode(SGRCODE::RESET);
	}


	void  ConsoleUtil::_putSGRCodes(const SGRCODE* sgrCodes, const size_t sgrCodesCount) {
		if (sgrCodes[0] == SGRCODE::NONE) return;
		for (int i = 0; i < sgrCodesCount; ++i) {
			printf("\033[%dm", sgrCodes[i]);
		}
	}

	void ConsoleUtil::_putSGRCode(const SGRCODE sgrCode) {
		printf("\033[%dm", sgrCode);
	}

	void ConsoleUtil::_putBGColorCode(const BGCOLOR bgColor) {
		printf("\033[%dm", bgColor);
	}

	void ConsoleUtil::_putFGColorCode(const FGCOLOR fgColor) {
		printf("\033[%dm", fgColor);
	}

	void ConsoleUtil::_putFG24BitColorCode(const unsigned char(&fgColor24bit)[3]) {
		printf("\033[38;2;%d;%d;%dm", fgColor24bit[0], fgColor24bit[1], fgColor24bit[2]);
	}

	void ConsoleUtil::_putBG24BitColorCode(const unsigned char(&bgColor24bit)[3]) {
		printf("\033[48;2;%d;%d;%dm", bgColor24bit[0], bgColor24bit[1], bgColor24bit[2]);
	}

	void ConsoleUtil::_putFG256ColorCode(const unsigned char fgColor256) {
		printf("\033[38;5;%dm", fgColor256);
	}

	void ConsoleUtil::_putBG256ColorCode(const unsigned char bgColor256) {
		printf("\033[48;5;%dm", bgColor256);
	}

	void ConsoleUtil::_runCSIFunction(const CSICODE csiCode) {
		printf("\033[%c",  csiCode);
	}

	void ConsoleUtil::_runCSIFunction(const CSICODE csiCode, const int param1) {
		printf("\033[%d%c", param1, csiCode);
	}

	void ConsoleUtil::_runCSIFunction(const CSICODE csiCode, const int param1, const int param2) {
		printf("\033[%d;%d%c", param1, param2, csiCode);
	}

	// 向buffer输出控制符的函数, 返回值为写入的字节数, 便于指针移动

	size_t  ConsoleUtil::_putSGRCodes(char* buf, const SGRCODE* sgrCodes, const size_t sgrCodesCount) {
		size_t offset = 0;
		if (sgrCodes[0] == SGRCODE::NONE) {
			return 0;
		}
		for (int i = 0; i < sgrCodesCount; ++i) {
			size_t delta;
			sprintf(buf,"\033[%dm%zn", sgrCodes[i],&delta);
			offset += delta;
		}
		return offset;
	}

	size_t ConsoleUtil::_putSGRCode(char* buf, const SGRCODE sgrCode) {
		size_t offset;
		sprintf(buf,"\033[%dm%zn", sgrCode,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putBGColorCode(char* buf, const BGCOLOR bgColor) {
		size_t offset;
		sprintf(buf,"\033[%dm%zn", bgColor,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putFGColorCode(char* buf, const FGCOLOR fgColor) {
		size_t offset;
		sprintf(buf,"\033[%dm%zn", fgColor,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putFG24BitColorCode(char* buf, const unsigned char(&fgColor24bit)[3]) {
		size_t offset;
		sprintf(buf,"\033[38;2;%d;%d;%dm%zn", fgColor24bit[0], fgColor24bit[1], fgColor24bit[2],&offset);
		return offset;
	}

	size_t ConsoleUtil::_putBG24BitColorCode(char* buf, const unsigned char(&bgColor24bit)[3]) {
		size_t offset;
		printf("\033[48;2;%d;%d;%dm%zn", bgColor24bit[0], bgColor24bit[1], bgColor24bit[2],&offset);
		return offset;
	}

	size_t ConsoleUtil::_putFG256ColorCode(char* buf, const unsigned char fgColor256) {
		size_t offset;
		printf("\033[38;5;%dm%zn", fgColor256,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putBG256ColorCode(char* buf, const unsigned char bgColor256) {
		size_t offset;
		printf("\033[48;5;%dm%zn", bgColor256,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putCSIFunctionCode(char* buf, const CSICODE csiCode) {
		size_t offset;
		printf("\033[%c%zn", csiCode,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putCSIFunctionCode(char* buf, const CSICODE csiCode, const int param1) {
		size_t offset;
		printf("\033[%d%c%zn", param1, csiCode,&offset);
		return offset;
	}

	size_t ConsoleUtil::_putCSIFunctionCode(char* buf, const CSICODE csiCode, const int param1, const int param2) {
		size_t offset;
		printf("\033[%d;%d%c%zn", param1, param2, csiCode,&offset);
		return offset;
	}


}
#pragma once
#ifndef ConsoleUtil_h__
#define ConsoleUtil_h__
#include<stdarg.h>
#include<stdio.h>
#include"../Config/typedef.h"


/**
 * 控制台工具
 * 基于ANSI Escape Code 封装的控制台打印函数扩展
 * 可以实现文字变色, 改变光标位置等效果
 * 有关ANSI Escape Code 的详细信息见https://en.wikipedia.org/wiki/ANSI_escape_code
 * 封装者: kiminouso
 */
namespace Util
{
	class ConsoleUtil
	{
	public:
		enum class BGCOLOR
		{
			BG_BLACK = 40,
			BG_RED = 41,
			BG_GREEN = 42,
			BG_YELLOW = 43,
			BG_BLUE = 44,
			BG_MAGENTA = 45,
			BG_CYAN = 46,
			BG_WHITE = 47,
			BG_GRAY = 100,
			BG_BRIGHT_RED = 101,
			BG_BRIGHT_GREEN = 102,
			BG_BRIGHT_YELLOW = 103,
			BG_BRIGHT_BLUE = 104,
			BG_BRIGHT_MAGENTA = 105,
			BG_BRIGHT_CYAN = 106,
			BG_BRIGHT_WHITE = 107,
		};

		enum class FGCOLOR
		{
			FG_BLACK = 30,
			FG_RED = 31,
			FG_GREEN = 32,
			FG_YELLOW = 33,
			FG_BLUE = 34,
			FG_MAGENTA = 35,
			FG_CYAN = 36,
			FG_WHITE = 37,
			FG_GRAY = 90,
			FG_BRIGHT_RED = 91,
			FG_BRIGHT_GREEN = 92,
			FG_BRIGHT_YELLOW = 93,
			FG_BRIGHT_BLUE = 94,
			FG_BRIGHT_MAGENTA = 95,
			FG_BRIGHT_CYAN = 96,
			FG_BRIGHT_WHITE = 97,
		};

		enum class SGRCODE
		{
			NONE = -1,
			RESET = 0,
			HIGHLIGHT = 1,
			DIM = 2,
			ITALIC = 3,
			UNDERLINE = 4,
			SLOW_BLINK = 5,
			RAPID_BLINK = 6,
			REVERSE_VIDEO = 7,
			HIDE = 8,
			CROSS_OUT = 9
		};

		enum class CSICODE
		{
			CURSOR_UP = 'A',
			CURSOR_DOWN = 'B',
			CURSOR_FORWARD = 'C',
			CURSOR_BACK = 'D',
			CURSOR_NEXT_LINE = 'E',
			CURSOR_PREVIOUS_LINE = 'F',
			CURSOR_HORIZONTAL_ABSOLUTE = 'G',
			CURSOR_POSITION = 'H',
			ERASE_IN_DISPLAY = 'J',
			ERASE_IN_LINE = 'K',
			SCROLL_UP = 'S',
			SCROLL_DOWN = 'T',
			SAVE_CURSOR_POSITION = 's',
			RESTORE_CURSOR_POSITION = 'u'
		};

		static void enableCountFormatSpecifier();

		static void putChar(const char ch);

		static void putChar(const char ch, int count);

		static void putChar(const char ch, int count, char* buffer);

		static void getTimeString(time_t timestamp,char* buffer,size_t bufferSize);

		/*日期转换为时间戳, 日期必须为yyyy-mm-dd hh:mm:ss 格式*/
		static time_t getTimestamp(const char* timeStr);

		template<size_t N>
		static void getTimeString(time_t timestamp, char(&buffer)[N]) {
			getTimeString(timestamp, buffer, N);
		}


		template<size_t N>
		static void moveStringRight(char(&targetStr)[N], const char paddingChar, const size_t moveLengthCharCount) {
			moveStringRight(targetStr, targetStr, paddingChar, moveLengthCharCount);
		}

		/*
		* 字符串右移函数
		* 例: "123" 右移 3 个字符单位, 用'*'填充后为 "***123"
		*/
		static void moveStringRight(const char* targetStr, char* buffer, const char paddingChar, const size_t moveLengthCharCount);

		/*
		* 获取让字符串左对齐的填充字符
		* 例如, 假设窗口宽度为10个字符, 字符串"123"用'*'填充为"123*******"
		*/
		static void getLeftAlignPaddings(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount);

		/*
		* 使字符串左对齐, 假设窗口宽度为columnCharCount个字符长度
		* paddingChar为对齐填充的字符串
		*/
		static void alignStringLeft(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount);

		template<size_t N>
		static void alignStringLeft(char(&targetStr)[N], const char paddingChar) {
			alignStringLeft(targetStr, targetStr, paddingChar, N - 1);
		}

		/*
		* 使字符串居中化
		* 传入一个字符串targetStr, 和填充字符padding
		* 会加工该字符串使其在控制台居中显示(假设控制台宽度为columnCharCount)
		* 注意:返回的加工后的字符串需要delete
		*/
		[[deprecated("返回的字符串需要delete, 不安全, 请使用 template<size_t N> void alignStringCenter(const char (&targetStr)[N], const char paddingChar)")]]
		static const char* alignStringCenter(const char* targetStr, const char paddingChar, const size_t columnCharCount);


		/*
		* 使字符串居中化
		* 传入一个字符串targetStr, 和填充字符padding
		* 会加工该字符串使其在控制台居中显示(假设控制台宽度为columnCharCount)
		* 注意:返回的加工后的字符串需要delete
		*/
		static void alignStringCenter(const char* targetStr, char* buffer, const char paddingChar, size_t columnCharCount);




		/*
		* 使字符串居中化
		* 传入一个字符串targetStr, 和填充字符padding
		* 会加工该字符串使其在控制台居中显示(假设控制台宽度为数组长度N-1)
		* 注意:返回的加工后的字符串需要delete
		*/
		template<size_t N>
		static void alignStringCenter(char (&targetStr)[N], const char paddingChar) {
			alignStringCenter(targetStr, targetStr, paddingChar,N - 1);
		}

		/*
		* 获取使某字符串居中时所需的填充字符数
		* 传入一个字符串targetStr和控制台宽度(以字符记)
		* 返回要使该字符串居中所需填充的字符(单侧字符)
		*/
		static void getCenteringPaddings(const char* targetStr, char* destination, const char paddingChar, const size_t columnCharCount);

		static size_t getCenteringPaddingsCharCount(const char* targetStr, const size_t columnCharCount);


		static void setConsoleSize(const int columnCharCount, const int lineCharCount);

		/*
		* 提前填充控制台输入字符
		*/
		static bool prefillConsoleWindowInput(const char* strFilled);

		static void runCSIFunction(const CSICODE& csiCode);

		static void runCSIFunction(const CSICODE& csiCode, const int param1);

		static void runCSIFunction(const CSICODE& csiCode, const int param1, const int param2);

		template<size_t N>
		static void sprintfEx(char* buf,const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_sprintfEx(buf,sgrCodes, N, format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void sprintfEx(char* buf,const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_sprintfEx(buf,bgColor, fgColor, sgrCodes, N, format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void sprintfEx(char* buf, const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_sprintfEx(buf,bgColor24bit, fgColor24bit, sgrCodes, N, format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void sprintfEx(char* buf, const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_sprintfEx(buf,bgColor256, fgColor256, sgrCodes, N, format, arglist);
			va_end(arglist);
		}



		template<size_t N>
		static void printfEx(const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_printfEx(sgrCodes, N, format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void printfEx(const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_printfEx(bgColor, fgColor, sgrCodes, N,format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void printfEx(const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_printfEx(bgColor24bit, fgColor24bit, sgrCodes, N,format, arglist);
			va_end(arglist);
		}

		template<size_t N>
		static void printfEx(const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE(&sgrCodes)[N], const char* format, ...) {
			va_list arglist;
			va_start(arglist, format);
			_printfEx(bgColor256, fgColor256, sgrCodes, N,format, arglist);
			va_end(arglist);
		}

		static int getcharEx_GBK(char* buffer,int& byteGotten);

		static void backspace();


		static bool prefillConsoleWindowInput(const char* strFilled, const DWORD strLen);

		static void getStdinHandle();

		static void getStdoutHandle();

		static void getStderrHandle();

		static void truncateStringWithDots(char* targetStr, const DWORD constraintLen);

	private:
		static int columnCharCount;
		static int lineCharCount;

		static HANDLE hStdin;
		static HANDLE hStdout;
		static HANDLE hStderr;

		static void _sprintfEx(char* buf,const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _sprintfEx(char* buf, const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _sprintfEx(char* buf, const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _sprintfEx(char* buf, const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _printfEx(const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _printfEx(const BGCOLOR bgColor, const FGCOLOR fgColor, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _printfEx(const unsigned char(&bgColor24bit)[3], const unsigned char(&fgColor24bit)[3], const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _printfEx(const unsigned char bgColor256, const unsigned char fgColor256, const SGRCODE* sgrCodes, const size_t sgrCodesCount, const char* format, va_list arglist);

		static void _putSGRCodes(const SGRCODE* sgrCodes, const size_t sgrCodesCount);

		static void _putSGRCode(const SGRCODE sgrCode);

		static void _putBGColorCode(const BGCOLOR bgColor);

		static void _putFGColorCode(const FGCOLOR fgColor);

		static void _putFG24BitColorCode(const unsigned char(&fgColor24bit)[3]);

		static void _putBG24BitColorCode(const unsigned char(&bgColor24bit)[3]);

		static void _putFG256ColorCode(const unsigned char fgColor256);

		static void _putBG256ColorCode(const unsigned char bgColor256);

		static void _runCSIFunction(const CSICODE csiCode);

		static void _runCSIFunction(const CSICODE csiCode, const int param1);

		static void _runCSIFunction(const CSICODE csiCode, const int param1, const int param2);


		static size_t _putSGRCodes(char* buf,const SGRCODE* sgrCodes, const size_t sgrCodesCount);

		static size_t _putSGRCode(char* buf,const SGRCODE sgrCode);

		static size_t _putBGColorCode(char* buf, const BGCOLOR bgColor);

		static size_t _putFGColorCode(char* buf, const FGCOLOR fgColor);

		static size_t _putFG24BitColorCode(char* buf, const unsigned char(&fgColor24bit)[3]);

		static size_t _putBG24BitColorCode(char* buf, const unsigned char(&bgColor24bit)[3]);

		static size_t _putFG256ColorCode(char* buf, const unsigned char fgColor256);

		static size_t _putBG256ColorCode(char* buf, const unsigned char bgColor256);

		static size_t _putCSIFunctionCode(char* buf, const CSICODE csiCode);

		static size_t _putCSIFunctionCode(char* buf, const CSICODE csiCode, const int param1);

		static size_t _putCSIFunctionCode(char* buf, const CSICODE csiCode, const int param1, const int param2);

	};


}



#endif // ConsoleUtil_h__
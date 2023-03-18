#pragma once
#ifndef ConsoleUtil_h__
#define ConsoleUtil_h__
#include<stdarg.h>
#include<stdio.h>
#include"../Config/typedef.h"


/**
 * ����̨����
 * ����ANSI Escape Code ��װ�Ŀ���̨��ӡ������չ
 * ����ʵ�����ֱ�ɫ, �ı���λ�õ�Ч��
 * �й�ANSI Escape Code ����ϸ��Ϣ��https://en.wikipedia.org/wiki/ANSI_escape_code
 * ��װ��: kiminouso
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

		/*����ת��Ϊʱ���, ���ڱ���Ϊyyyy-mm-dd hh:mm:ss ��ʽ*/
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
		* �ַ������ƺ���
		* ��: "123" ���� 3 ���ַ���λ, ��'*'����Ϊ "***123"
		*/
		static void moveStringRight(const char* targetStr, char* buffer, const char paddingChar, const size_t moveLengthCharCount);

		/*
		* ��ȡ���ַ�������������ַ�
		* ����, ���贰�ڿ��Ϊ10���ַ�, �ַ���"123"��'*'���Ϊ"123*******"
		*/
		static void getLeftAlignPaddings(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount);

		/*
		* ʹ�ַ��������, ���贰�ڿ��ΪcolumnCharCount���ַ�����
		* paddingCharΪ���������ַ���
		*/
		static void alignStringLeft(const char* targetStr, char* buffer, const char paddingChar, const size_t columnCharCount);

		template<size_t N>
		static void alignStringLeft(char(&targetStr)[N], const char paddingChar) {
			alignStringLeft(targetStr, targetStr, paddingChar, N - 1);
		}

		/*
		* ʹ�ַ������л�
		* ����һ���ַ���targetStr, ������ַ�padding
		* ��ӹ����ַ���ʹ���ڿ���̨������ʾ(�������̨���ΪcolumnCharCount)
		* ע��:���صļӹ�����ַ�����Ҫdelete
		*/
		[[deprecated("���ص��ַ�����Ҫdelete, ����ȫ, ��ʹ�� template<size_t N> void alignStringCenter(const char (&targetStr)[N], const char paddingChar)")]]
		static const char* alignStringCenter(const char* targetStr, const char paddingChar, const size_t columnCharCount);


		/*
		* ʹ�ַ������л�
		* ����һ���ַ���targetStr, ������ַ�padding
		* ��ӹ����ַ���ʹ���ڿ���̨������ʾ(�������̨���ΪcolumnCharCount)
		* ע��:���صļӹ�����ַ�����Ҫdelete
		*/
		static void alignStringCenter(const char* targetStr, char* buffer, const char paddingChar, size_t columnCharCount);




		/*
		* ʹ�ַ������л�
		* ����һ���ַ���targetStr, ������ַ�padding
		* ��ӹ����ַ���ʹ���ڿ���̨������ʾ(�������̨���Ϊ���鳤��N-1)
		* ע��:���صļӹ�����ַ�����Ҫdelete
		*/
		template<size_t N>
		static void alignStringCenter(char (&targetStr)[N], const char paddingChar) {
			alignStringCenter(targetStr, targetStr, paddingChar,N - 1);
		}

		/*
		* ��ȡʹĳ�ַ�������ʱ���������ַ���
		* ����һ���ַ���targetStr�Ϳ���̨���(���ַ���)
		* ����Ҫʹ���ַ����������������ַ�(�����ַ�)
		*/
		static void getCenteringPaddings(const char* targetStr, char* destination, const char paddingChar, const size_t columnCharCount);

		static size_t getCenteringPaddingsCharCount(const char* targetStr, const size_t columnCharCount);


		static void setConsoleSize(const int columnCharCount, const int lineCharCount);

		/*
		* ��ǰ������̨�����ַ�
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
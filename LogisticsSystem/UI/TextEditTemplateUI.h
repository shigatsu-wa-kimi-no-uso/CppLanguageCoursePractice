#pragma once
#include "UserInterface.h"
#include "../Utility//ConsoleUtil.h"

#define	BEGIN_OPTION_STRING_MAP(clazz,enumCnt) const char*** clazz::getOptionStrings() {\
								static const char* str[(int)enumCnt][2];

#define BIND_OPTION_STRINGS(option,dispStr,prefilledStr) str[(unsigned int)option][0]= dispStr; str[(unsigned int)option][1]=prefilledStr

#define END_OPTION_STRING_MAP return (const char***)str;}

#define BEGIN_TEXTEDIT_OPTION_MAP(clazz,bufferSize) void clazz::_edit(){\
		ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::RESTORE_CURSOR_POSITION);\
		constexpr int _bufferSize=bufferSize;\
		char buf[_bufferSize];\
		switch (currSelection){

#define BIND_TEXTEDIT_OPTION(optionEnum,injectedVal,constraintSize) case (int)optionEnum:\
		sprintf_s(buf, "%s", injectedVal);\
		ConsoleUtil::prefillConsoleWindowInput(buf);\
		__readChars(injectedVal,constraintSize);\
		break
#define END_TEXTEDIT_OPTION_MAP };isEditing= false;}

namespace UI
{
	template<typename OptionEnum_t, int enumCnt>
	class TextEditTemplateUI : public UserInterface
	{
	protected:
		using ConsoleUtil = Util::ConsoleUtil;
		int currSelection;
		bool isEditing;
		void __readChars(char* buffer, const size_t bufferSize) {
			int offset = 0;
			char c;
			int i = 0;
			bool isInputingKanji = false;
			while (1) {
				if (i == bufferSize - 1) {
					while (getchar() != '\n');
					//汉字判断, 如果输入了一半汉字, 则清空
					if (isInputingKanji) {
						buffer[i - 1] = 0;
					}
					break;
				}
				c = getchar();
				if (c == '\n') {
					break;
				} else {
					if ((signed char)c < 0 && isInputingKanji == false) {
						isInputingKanji = true;
					} else {
						isInputingKanji = false;
					}
					buffer[i++] = c;
				}
			}
			buffer[i] = 0;
		}
		void _printHeader() {
			UserInterface::_printHeader(getTitle());
		}
		void _printFooter() {
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, LINE_CHARCOUNT - 1, 1);
			ConsoleUtil::putChar('_', COLUMN_CHARCOUNT);
			putchar('\n');
			char buffer[COLUMN_CHARCOUNT + 1];
			if (isEditing) {
				va_list arglst = (va_list)getFooterPrintArgsWhenEditing();
				vsprintf_s(buffer, getFooterFormatWhenEditing(), arglst);
			} else {
				va_list arglst = (va_list)getFooterPrintArgsWhenSelecting();
				vsprintf_s(buffer, getFooterFormatWhenSelecting(), arglst);
			}
			ConsoleUtil::alignStringLeft(buffer, ' ');
			ConsoleUtil::printfEx({ ConsoleUtil::SGRCODE::UNDERLINE }, buffer);
		}
		Message onKeyUp() {
			currSelection = currSelection == (int)getSelectableLowerBound() ? currSelection : currSelection - 1;
			return { Message::PAINT_INTERFACE };
		}

		Message onKeyDown() {
			currSelection = ((currSelection == (int)getSelectableUpperBound() - 1) ? currSelection : currSelection + 1);
			return { Message::PAINT_INTERFACE };
		}

		Message onKeyEnter() {
			isEditing = true;
			class _Callback : public Callback {
				void operator()(void* param1) {
					((TextEditTemplateUI*)param1)->_edit();
				}
				~_Callback() {}
			};
			return { Message::PAINT_INTERFACE, new _Callback, this };
		}

		TextEditTemplateUI() : currSelection(0), isEditing(false){}

		void paint() {
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, 1, 1);
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::ERASE_IN_DISPLAY, 2);
			_printHeader();
			putchar('\n');
			putchar('\n');
			char alignPaddings[COLUMN_CHARCOUNT + 1] = { 0 };
			ConsoleUtil::moveStringRight(alignPaddings, ' ', 50);
			const char** optionStrings = (const char**)getOptionStrings();
			for (int i = 0; i < 2*enumCnt; i+=2) {
				printf(alignPaddings);
				if (currSelection == i/2) {
					ConsoleUtil::printfEx(ConsoleUtil::BGCOLOR::BG_BRIGHT_CYAN, ConsoleUtil::FGCOLOR::FG_BLACK, { ConsoleUtil::SGRCODE::NONE }, optionStrings[i]);
					putchar(':');
					ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::SAVE_CURSOR_POSITION);
				} else {
					printf(optionStrings[i]);
					putchar(':');
				}
				printf(optionStrings[i+1]);
				putchar('\n');
				putchar('\n');
			}
			putchar('\n');
			_printFooter();
		}
		//重写下面的函数来修改模版UI
		virtual void _edit() = 0;
		virtual const char*** getOptionStrings() = 0;
		virtual OptionEnum_t getSelectableUpperBound() = 0;
		virtual OptionEnum_t getSelectableLowerBound() = 0;
		virtual const char* getTitle() = 0;
		virtual const char* getFooterFormatWhenEditing() = 0;
		virtual const char* getFooterFormatWhenSelecting() = 0;
		virtual const void** getFooterPrintArgsWhenEditing() = 0;
		virtual const void** getFooterPrintArgsWhenSelecting() = 0;
		Message processKeyboardEvents() = 0;
	};

}
#pragma once
#include "UserInterface.h"
#include "../Container/ScrollIterator.h"
#include "../Utility/ConsoleUtil.h"
#include <stdarg.h>
#include <stdio.h>

namespace UI
{
	template<typename Iter_t>
	class ListTemplateUI : public UserInterface
	{
	public:
		static void refreshPageIter() {
			iterAssigned = false;
			topElemOrder = 0;
			selectionOrder = 0;
		}

	protected:
		using ConsoleUtil = Util::ConsoleUtil;
		static literal int PageElemCntLimit = 24;
		static bool iterAssigned;
		static DWORD topElemOrder;
		static DWORD selectionOrder;
		static DWORD currPageElemCnt;
		static ScrollIterator<Iter_t> pageIter;
		void _assignPageIter() {
			if (!iterAssigned || !pageIter.getTopIterator().hasNext()) {
				Iter_t topIter = getBeginIterator();
				Iter_t bottomIter = topIter;
				for (DWORD i = 0; i < getPageElemCntLimit(); ++i) {
					++currPageElemCnt;
					if (bottomIter.hasNext()) {
						bottomIter._setSelfNext();
					} else {
						break;
					}
				}
				pageIter = ScrollIterator<Iter_t>(topIter, topIter, bottomIter);
				iterAssigned = true;
			} else {
				while (currPageElemCnt <= getPageElemCntLimit()) {
					if (pageIter.isBottomIteratorIncreasable()) {
						pageIter.increaseBottomIterator();
						++currPageElemCnt;
					} else {
						break;
					}
				}
			}
		}
		void _printHeader() {
			UserInterface::_printHeader(getTitle());
			char buffer[COLUMN_CHARCOUNT + 64];
			va_list arglist = (va_list)getHeaderPrintArgs();
			vsprintf_s(buffer, getHeaderFormat(), arglist);
			ConsoleUtil::alignStringLeft(buffer, buffer, ' ',COLUMN_CHARCOUNT);
			ConsoleUtil::printfEx({ ConsoleUtil::SGRCODE::UNDERLINE }, buffer);
		}
		void _printFooter() {
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, LINE_CHARCOUNT - 1, 1);
			ConsoleUtil::putChar('_', COLUMN_CHARCOUNT);
			putchar('\n');
			char buffer[COLUMN_CHARCOUNT + 1];
			va_list arglist = (va_list)getFooterPrintArgs();
			vsprintf_s(buffer, getFooterFormat(), arglist);
			ConsoleUtil::alignStringLeft(buffer, ' ');
			ConsoleUtil::printfEx({ ConsoleUtil::SGRCODE::UNDERLINE }, buffer);
		}
		void _printEntry(const Iter_t& iter, const DWORD order) {
			char buffer[COLUMN_CHARCOUNT + 1];
			va_list arglist = (va_list)getEntryPrintArgs(iter, order);
			vsprintf_s(buffer, getEntryFormat(), arglist);
			ConsoleUtil::alignStringLeft(buffer,buffer,' ', COLUMN_CHARCOUNT);
			if (pageIter.getFocusedIterator() == iter) {
				ConsoleUtil::printfEx(ConsoleUtil::BGCOLOR::BG_BRIGHT_CYAN, ConsoleUtil::FGCOLOR::FG_BLACK, { ConsoleUtil::SGRCODE::NONE }, "%s",buffer);
			} else {
				printf(buffer);
			}
			putchar('\n');
		}
		void paint() {
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::CURSOR_POSITION, 1, 1);
			ConsoleUtil::runCSIFunction(ConsoleUtil::CSICODE::ERASE_IN_DISPLAY, 2);
			_printHeader();
			Iter_t iter = pageIter.getTopIterator();
			Iter_t bottomIter = pageIter.getBottomIterator();
			Iter_t endIter = getEndIterator();
			int cnt = topElemOrder;
			if (iter != endIter) {
				do {
					_printEntry(iter, cnt + 1);
					if (iter.hasNext() && iter != bottomIter) {
						iter._setSelfNext();
						++cnt;
					} else {
						break;
					}
				} while (1);
			}
			_printFooter();
		}
		Message onKeyUp() {
			if (pageIter.hasPrevious()) {
				if (selectionOrder == 0) {
					--topElemOrder;
				} else {
					--selectionOrder;
				}
				pageIter._setSelfPrevious();
				return { Message::PAINT_INTERFACE };
			}
			return { Message::IGNORE };
		}
		Message onKeyDown() {
			if (pageIter.hasNext()) {
				if (selectionOrder == getPageElemCntLimit()) {
					++topElemOrder;
				} else {
					++selectionOrder;
				}
				pageIter._setSelfNext();
				return { Message::PAINT_INTERFACE };
			}
			return { Message::IGNORE };
		}
		virtual ~ListTemplateUI() {};
		virtual Message processKeyboardEvents() = 0;
		/*重写以下函数以达到设置标题、格式等效果*/
		virtual const char* getTitle() = 0;
		virtual const char* getHeaderFormat() = 0;
		virtual const void** getHeaderPrintArgs() = 0;
		virtual const char* getFooterFormat() = 0;
		virtual const void** getFooterPrintArgs() = 0;
		virtual const void** getEntryPrintArgs(const Iter_t& iter, const DWORD order) = 0;
		virtual const DWORD getPageElemCntLimit() = 0;
		virtual const Iter_t getBeginIterator() = 0;
		virtual const Iter_t getEndIterator() = 0;
		virtual const char* getEntryFormat() = 0;
	};

	template<typename Iter_t> bool ListTemplateUI<Iter_t>::iterAssigned;
	template<typename Iter_t> DWORD ListTemplateUI<Iter_t>::topElemOrder;
	template<typename Iter_t> DWORD ListTemplateUI<Iter_t>::selectionOrder;
	template<typename Iter_t> DWORD ListTemplateUI<Iter_t>::currPageElemCnt;
	template<typename Iter_t> ScrollIterator<Iter_t> ListTemplateUI<Iter_t>::pageIter;
}


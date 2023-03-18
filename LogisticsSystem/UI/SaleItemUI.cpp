#include "SaleItemUI.h"
#include "../Utility/ConsoleUtil.h"
#include "../Config/enumeration.h"
#include "MessageBoxUI.h"
#include <conio.h>
using namespace Util;

namespace UI
{
	ItemBatchInfoList::Iterator SaleItemUI::targetBatch;
	ItemInfoList::Iterator SaleItemUI::targetItem;

	SaleItemUI::SaleItemUI() {
		memset(saleAmount, 0, sizeof(saleAmount));
		memset(unitPrice, 0, sizeof(unitPrice));
	}

	BEGIN_KEYBOARD_EVENT_MAP(SaleItemUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
	END_KEYBOARD_EVENT_MAP

	void SaleItemUI::setTargetBatch(ItemBatchInfoList::Iterator targetBatch) {
		SaleItemUI::targetBatch = targetBatch;
	}

	void SaleItemUI::setTargetItem(ItemInfoList::Iterator targetItem) {
		SaleItemUI::targetItem = targetItem;
	}

	BEGIN_TEXTEDIT_OPTION_MAP(SaleItemUI, 64)
		BIND_TEXTEDIT_OPTION(SaleItemOption::AMOUNT, saleAmount,16);
		BIND_TEXTEDIT_OPTION(SaleItemOption::UNIT_PRICE, unitPrice,16);
	END_TEXTEDIT_OPTION_MAP

	BEGIN_OPTION_STRING_MAP(SaleItemUI, SaleItemOption::ENUMCNT)
		BIND_OPTION_STRINGS(SaleItemOption::AMOUNT, "出售数量", saleAmount);
		BIND_OPTION_STRINGS(SaleItemOption::UNIT_PRICE, "单价", unitPrice);
		BIND_OPTION_STRINGS(SaleItemOption::TOTAL_PRICE, "总价", _calculateTotalPrice(saleAmount, unitPrice));
	END_OPTION_STRING_MAP

	const char* SaleItemUI::_calculateTotalPrice(const char* saleAmount, const char* unitPrice) {
		int _saleAmount; double _unitPrice;
		if (sscanf_s(saleAmount, "%d", &_saleAmount) != 1
			|| sscanf_s(unitPrice, "%lf", &_unitPrice) != 1) {
			return "请填写数字";
		} else {
			static char buf[11];
			sprintf_s(buf, "%.3f", _saleAmount * _unitPrice);
			return (const char*)buf;
		}
	}

	SaleItemOption SaleItemUI::getSelectableUpperBound() {
		return SaleItemOption::TOTAL_PRICE;
	}

	SaleItemOption SaleItemUI::getSelectableLowerBound() {
		return SaleItemOption::AMOUNT;
	}


	const char* SaleItemUI::getTitle() {
		return "填写出售信息";
	}

	const char* SaleItemUI::getFooterFormatWhenEditing() {
		return "%10s";
	};

	const char* SaleItemUI::getFooterFormatWhenSelecting() {
		return "%14s|%18s|%12s|%12s";
	}

	const void** SaleItemUI::getFooterPrintArgsWhenEditing() {
		static const void* args[] = { "ENTER 确定" };
		return args;
	}

	const void** SaleItemUI::getFooterPrintArgsWhenSelecting() {
		static const void* args[] = { "↑↓ 切换选项","ESC 返回上一页面","ENTER 编辑", "F1 出售" };
		return args;
	}

	Message SaleItemUI::onKeyEsc() {
		SWITCH_UI(UserInterface::ITEM_BATCH_INFO);
	}

	Message SaleItemUI::onKeyF1() {
		int _saleAmount; 
		double _unitPrice;
		DWORD recordUID;
		if (sscanf_s(saleAmount, "%d", &_saleAmount) != 1
			|| sscanf_s(unitPrice, "%lf", &_unitPrice) != 1) {
			MessageBoxUI::pop("出售失败,请输入数字", 1500);
			return { Message::PAINT_INTERFACE};
		} else {
			if (targetBatch->purchasedAmount < targetBatch->soldCount + _saleAmount ) {
				MessageBoxUI::pop("出售失败,库存不足", 1500);
				return { Message::PAINT_INTERFACE };
			}
			salesSvc.saleItem(targetItem,targetBatch,_saleAmount,_unitPrice,recordUID);
			char msg[128];
			sprintf_s(msg, "%s %u", "出售成功, 编号为:", recordUID);
			MessageBoxUI::pop(msg, 1500);
			return { Message::PAINT_INTERFACE };
		}
	}
}
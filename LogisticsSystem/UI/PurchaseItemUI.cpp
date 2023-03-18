#include "PurchaseItemUI.h"
#include "../Utility/ConsoleUtil.h"
#include "../Config/enumeration.h"
#include "MessageBoxUI.h"
#include <conio.h>
using namespace Util;

namespace UI
{
	ItemInfoList::Iterator PurchaseItemUI::targetItem;

	PurchaseItemUI::PurchaseItemUI() {
		memset(purchaseAmount, 0, sizeof(purchaseAmount));
		memset(unitCost, 0, sizeof(unitCost));
	}


	BEGIN_KEYBOARD_EVENT_MAP(PurchaseItemUI)
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_F1, onKeyF1);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ESC, onKeyEsc);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_UP, onKeyUp);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_DOWN, onKeyDown);
		BIND_NEEDRETURN_RESPONSE_FUNC(KeyEvent::KEY_ENTER, onKeyEnter);
	END_KEYBOARD_EVENT_MAP


	void PurchaseItemUI::setTargetItem(ItemInfoList::Iterator targetItem) {
		PurchaseItemUI::targetItem = targetItem;
	}

	BEGIN_TEXTEDIT_OPTION_MAP(PurchaseItemUI, 64)
		BIND_TEXTEDIT_OPTION(PurchaseItemOption::AMOUNT,  purchaseAmount,16);
		BIND_TEXTEDIT_OPTION(PurchaseItemOption::UNIT_PRICE, unitCost,16);
	END_TEXTEDIT_OPTION_MAP

	BEGIN_OPTION_STRING_MAP(PurchaseItemUI, PurchaseItemOption::ENUMCNT)
		BIND_OPTION_STRINGS(PurchaseItemOption::AMOUNT, "�ɹ�����", purchaseAmount);
		BIND_OPTION_STRINGS(PurchaseItemOption::UNIT_PRICE, "����", unitCost);
		BIND_OPTION_STRINGS(PurchaseItemOption::TOTAL_PRICE, "�ܳɱ�",_calculateTotalPrice(purchaseAmount,unitCost));
	END_OPTION_STRING_MAP

	const char* PurchaseItemUI::_calculateTotalPrice(const char* purchaseAmount,const char* unitCost) {
		int _purchaseAmount; double _unitCost;
		if (sscanf_s(purchaseAmount, "%d", &_purchaseAmount) != 1
			|| sscanf_s(unitCost, "%lf", &_unitCost) != 1) {
			return "����д����";
		} else {
			static char buf[11];
			sprintf_s(buf, "%.3f", _purchaseAmount * _unitCost);
			return (const char*)buf;
		}
	}

	PurchaseItemOption PurchaseItemUI::getSelectableUpperBound() {
		return PurchaseItemOption::TOTAL_PRICE;
	}

	PurchaseItemOption PurchaseItemUI::getSelectableLowerBound() {
		return PurchaseItemOption::AMOUNT;
	}


	const char* PurchaseItemUI::getTitle() {
		return "��д�ɹ���Ϣ";
	}

	const char* PurchaseItemUI::getFooterFormatWhenEditing() {
		return "%10s";
	};

	const char* PurchaseItemUI::getFooterFormatWhenSelecting() {
		return "%14s|%18s|%12s|%12s";
	}

	const void** PurchaseItemUI::getFooterPrintArgsWhenEditing() {
		static const void* args[] = { "ENTER ȷ��" };
		return args;
	}

	const void** PurchaseItemUI::getFooterPrintArgsWhenSelecting() {
		static const void* args[] = { "���� �л�ѡ��","ESC ������һҳ��","ENTER �༭", "F1 �ɹ�" };
		return args;
	}

	Message PurchaseItemUI::onKeyEsc() {
		SWITCH_UI(UserInterface::ITEM_BATCH_INFO);
	}

	Message PurchaseItemUI::onKeyF1() {
		int _purchaseAmount; double _unitCost;
		if (sscanf_s(purchaseAmount, "%d", &_purchaseAmount) != 1
			|| sscanf_s(unitCost, "%lf", &_unitCost) != 1) {
			MessageBoxUI::pop("�ɹ�ʧ��,����������", 1500);
			return { Message::PAINT_INTERFACE};
		} else {
			DWORD batchUID;
			purchaseSvc.purchaseItem(targetItem, _purchaseAmount, _unitCost, batchUID);
			if (batchUID >= 0) {
				char msg[128];
				sprintf_s(msg, "%s %u", "�ɹ��ɹ�,����Ϊ:", batchUID);
				MessageBoxUI::pop(msg, 1500);
				return { Message::PAINT_INTERFACE };
			}
			MessageBoxUI::pop("�ɹ�ʧ��", 1500);
			return { Message::PAINT_INTERFACE };
		}
	}
}
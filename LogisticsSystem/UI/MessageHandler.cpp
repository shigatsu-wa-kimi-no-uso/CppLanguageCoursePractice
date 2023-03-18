#include "MessageHandler.h"
#include "MainMenuUI.h"
#include "ItemManagerUI.h"
#include "ItemBatchInfoUI.h"
#include "AddItemUI.h"
#include "PurchaseItemUI.h"
#include "MessageBoxUI.h"
#include "SaleItemUI.h"
#include "SalesRecordUI.h"
#include "PurchaseRecordUI.h"
#include "ItemInfoUI.h"
#include "StockCheckUI.h"

using namespace UI;

void MessageHandler::handleMessage(Message* pMsg) {
	switch (pMsg->msg) {
	case Message::EXIT_APPLICATION:
		exit(0);
		break;
	case Message::SWITCH_INTERFACE:
		delete UserInterface::pCurrentUI;
		switch (UserInterface::targetUI)
		{
		case UserInterface::MAIN_MENU:
			UserInterface::pCurrentUI = new MainMenuUI;
			break;
		case UserInterface::ITEM_MANAGER:
			UserInterface::pCurrentUI = new ItemManagerUI;
			break;
		case UserInterface::ADD_ITEM:
			UserInterface::pCurrentUI = new AddItemUI;
			break;
		case UserInterface::ITEM_BATCH_INFO:
			UserInterface::pCurrentUI = new ItemBatchInfoUI;
			break;
		case UserInterface::PURCHASE_ITEM:
			UserInterface::pCurrentUI = new PurchaseItemUI;
			break;
		case UserInterface::SALE_ITEM:
			UserInterface::pCurrentUI = new SaleItemUI;
			break;
		case UserInterface::PURCHASE_RECORD:
			UserInterface::pCurrentUI = new PurchaseRecordUI;
			break;
		case UserInterface::SALES_RECORD:
			UserInterface::pCurrentUI = new SalesRecordUI;
			break;
		case UserInterface::ITEM_INFO:
			UserInterface::pCurrentUI = new ItemInfoUI;
			break;
		case UserInterface::STOCK_CHECK:
			UserInterface::pCurrentUI = new StockCheckUI;
			break;
		}
		UserInterface::pCurrentUI->paint();
		break;
	case Message::PAINT_INTERFACE:
		UserInterface::pCurrentUI->paint();
		break;
	case Message::IGNORE:
		break;
	default:
		break;
	}
	if (pMsg->callback != nullptr) {
		(*pMsg->callback)(pMsg->param1);
		UserInterface::pCurrentUI->paint();
	}
}

#include "ItemBatchInfoService.h"
#include "../Utility/ConsoleUtil.h"
#include "../Entity/SalesRecord.h"
#include <time.h>
using namespace Util;

namespace Service
{
	bool ItemBatchInfoService::addItemBatchInfo(ItemBatchInfo& info) {
		itemBatchInfoDPO.addBatchInfo(info);
		return itemBatchInfoDPO.syncUpdatesToFile();
	}

	ItemBatchInfoList::Iterator ItemBatchInfoService::getItemBatchInfoListIterator(QWORD itemUID) {
		ItemBatchInfoList::Iterator iter;
		itemBatchInfoDPO.getItemBatchInfoListIterator(itemUID, iter);
		return iter;
	}

	ItemBatchInfoList::Iterator ItemBatchInfoService::getItemBatchInfoListEndIterator(QWORD itemUID) {
		ItemBatchInfoList::Iterator iter;
		itemBatchInfoDPO.getItemBatchInfoListEndIterator(itemUID, iter);
		return iter;
	}

	void ItemBatchInfoService::getStringifiedItemBatchInfo(ItemBatchInfo& infoEntry, StringifiedItemBatchInfo& stringifiedInfo) {
		sprintf_s(stringifiedInfo.batchUID, "%d", infoEntry.batchUID);
		sprintf_s(stringifiedInfo.cost, "%.2f", infoEntry.unitCost*infoEntry.purchasedAmount);
		sprintf_s(stringifiedInfo.profit, "%.2f", infoEntry.profit);
		sprintf_s(stringifiedInfo.purchasedAmount, "%d", infoEntry.purchasedAmount);
		sprintf_s(stringifiedInfo.soldCount, "%d", infoEntry.soldCount); 
		sprintf_s(stringifiedInfo.netProfit, "%.2f", infoEntry.profit - infoEntry.unitCost*infoEntry.soldCount);
		sprintf_s(stringifiedInfo.unitCost, "%.2f", infoEntry.unitCost);
		sprintf_s(stringifiedInfo.remainingCount, "%d", infoEntry.purchasedAmount - infoEntry.soldCount);
		sprintf_s(stringifiedInfo.status, "%s", [&] {
			switch (infoEntry.status) {
			case ItemStatus::DELETED:
				return "已删除";
			case ItemStatus::NEED_SUPPLEMENT:
				return "需要补充";
			case ItemStatus::NORMAL:
				return "正常";
			case ItemStatus::SOLD_OUT:
				return "售罄";
			}
			}());
		if (infoEntry.timeOfPurchase == 0) {
			sprintf_s(stringifiedInfo.timeOfPurchase, "N/A");
		} else {
			ConsoleUtil::getTimeString(infoEntry.timeOfPurchase, stringifiedInfo.timeOfPurchase);
		}
	}

	void ItemBatchInfoService::unloadMap() {
		itemBatchInfoDPO.unloadMap();
	}

	void ItemBatchInfoService::deleteItemBatchInfo(ItemBatchInfoList::Iterator& target,ItemInfoList::Iterator& targetItem) {
		itemBatchInfoDPO.deleteItemBatchInfo(target);
		itemBatchInfoDPO.syncUpdatesToFile();
	}

}
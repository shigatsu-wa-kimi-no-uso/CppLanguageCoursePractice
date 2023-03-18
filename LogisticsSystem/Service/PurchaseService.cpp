#include "PurchaseService.h"
#include "../Entity/StringifiedItemInfo.h"

namespace Service
{
	PurchaseRecordList PurchaseService::purchaseRecordListFiltered;
	
	void PurchaseService::purchaseItem(ItemInfoList::Iterator &targetItem, DWORD count,double unitCost,DWORD& batchUID) {
		PurchaseRecord record;
		ItemBatchInfo batchInfo;

		//创建BatchInfo
		batchInfo.purchasedAmount = count;
		batchInfo.unitCost = unitCost;
		batchInfo.timeOfPurchase = time(0);
		batchInfo.profit = 0;
		batchInfo.soldCount = 0;
		batchInfo.status = ItemStatus::NORMAL;
		batchInfo.itemUID = targetItem->info.itemUID;
		batchInfo.batchUID = itemBatchInfoDPO.getNewBatchID();

		//创建record
		record.batchID = batchInfo.batchUID;
		record.recordUID = purchaseRecordDPO.getNewRecordID();
		record.itemUID = batchInfo.itemUID;
		record.purchasedAmount = batchInfo.purchasedAmount;
		record.timeOfPurchase = batchInfo.timeOfPurchase;
		record.unitCost = batchInfo.unitCost;
		record.remainingCount = targetItem->remainingCount + count;

		//更新ItemInfo
		targetItem->remainingCount += count;
		targetItem->totalCost += batchInfo.unitCost* count;
		itemInfoDPO.updateItemStatus(targetItem);
		purchaseRecordDPO.addPurchaseRecord(record);
		itemBatchInfoDPO.addBatchInfo(batchInfo);
		itemInfoDPO.markModified();
		if (itemInfoDPO.syncUpdatesToFile()
			&& purchaseRecordDPO.syncUpdatesToFile()
			&& itemBatchInfoDPO.syncUpdatesToFile()) {
			batchUID = batchInfo.batchUID;
		} else {
			batchUID = ~0L;
		}
	
	}

	PurchaseRecordList::Iterator PurchaseService::getPurchaseRecordListIterator(time_t timeLB,time_t timeUB) {
		PurchaseRecordList::Iterator iter;
		if (timeLB <= 0 || timeUB <= 0) {
			purchaseRecordDPO.getPurchaseRecordListIterator(iter);
		} else {
			purchaseRecordDPO.getPurchaseRecordListIteratorFilteredByTime(iter, timeLB, timeUB);
		}
		return iter;
	}

	PurchaseRecordList::Iterator PurchaseService::getPurchaseRecordListEndIterator(time_t timeLB, time_t timeUB) {
		PurchaseRecordList::Iterator iter;
		if (timeLB <= 0 || timeUB <= 0) {
			purchaseRecordDPO.getPurchaseRecordListEndIterator(iter);
		} else {
			purchaseRecordDPO.getPurchaseRecordListEndIteratorFilteredByTime(iter);
		}
		return iter;
	}

	PurchaseRecordList::Iterator PurchaseService::getPurchaseRecordListIteratorFilteredBySubstring(const char* substr, time_t timeLB, time_t timeUB) {
		PurchaseRecordList::Iterator iter;
		PurchaseRecordList::Iterator endIter;
		ItemInfo info;
		StringifiedItemInfo strInfo;
		purchaseRecordListFiltered.clear();
		iter = getPurchaseRecordListIterator(timeLB, timeUB);
		endIter = getPurchaseRecordListEndIterator(timeLB, timeUB);
		for (; iter != endIter; iter._setSelfNext()) {
			info.info.itemUID = iter->itemUID;
			displayStringMapDPO.getDisplayStrings(info, strInfo);
			if (strstr(strInfo.itemName, substr) != NULL
				|| strstr(strInfo.typeName, substr) != NULL
				|| strstr(strInfo.producer, substr) != NULL
				|| strstr(strInfo.spec, substr) != NULL) {
				purchaseRecordListFiltered.pushBack(*iter);
			}
		}
		return purchaseRecordListFiltered.begin();
	}

	PurchaseRecordList::Iterator PurchaseService::getPurchaseRecordListEndIteratorFiltered() {
		return purchaseRecordListFiltered.end();
	}

}
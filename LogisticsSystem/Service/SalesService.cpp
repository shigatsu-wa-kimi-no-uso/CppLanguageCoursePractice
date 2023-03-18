#include "SalesService.h"


namespace Service
{

	SalesRecordList SalesService::salesRecordListFiltered;

	void SalesService::saleItem(ItemInfoList::Iterator& targetItem,ItemBatchInfoList::Iterator& targetBatch,DWORD count,double unitPrice,DWORD& recordUID) {
		SalesRecord record;

		//创建record
		record.batchID = targetBatch->batchUID;
		record.recordUID = salesRecordDPO.getNewRecordID();
		record.itemUID = targetBatch->itemUID;
		record.unitPrice = unitPrice;
		record.soldCount = count;
		record.netProfit = (unitPrice - targetBatch->unitCost) * count;
		record.recordTime = time(0);
		record.remainingCount = targetBatch->purchasedAmount - targetBatch->soldCount - count;
		salesRecordDPO.addSalesRecord(record);

		//更新BatchInfo
		targetBatch->soldCount += count;
		targetBatch->profit += unitPrice * count;
		if (targetBatch->purchasedAmount - targetBatch->soldCount <= 0) {
			targetBatch->status = ItemStatus::SOLD_OUT;
		}

		//更新ItemInfo
		targetItem->totalSoldCount += count;
		targetItem->recentSoldCount += count;
		targetItem->remainingCount -= count;
		targetItem->netProfit += record.netProfit;
		targetItem->totalSalesFigure += unitPrice * count;
		itemInfoDPO.updateItemStatus(targetItem);

		//更新文件
		itemBatchInfoDPO.markModified();
		itemInfoDPO.markModified();
		salesRecordDPO.syncUpdatesToFile();
		itemBatchInfoDPO.syncUpdatesToFile();
		itemInfoDPO.syncUpdatesToFile();

		recordUID = record.recordUID;
	}

	SalesRecordList::Iterator SalesService::getSalesRecordListIterator(time_t timeLB,time_t timeUB) {
		SalesRecordList::Iterator iter;
		if (timeLB <= 0 || timeUB <= 0) {
			salesRecordDPO.getSalesRecordListIterator(iter);
		} else {
			salesRecordDPO.getSalesRecordListIteratorFilteredByTime(iter, timeLB, timeUB);
		}
		return iter;
	}

	SalesRecordList::Iterator SalesService::getSalesRecordListEndIterator(time_t timeLB, time_t timeUB) {
		SalesRecordList::Iterator iter;
		if (timeLB <= 0 || timeUB <= 0) {
			salesRecordDPO.getSalesRecordListEndIterator(iter);
		} else {
			salesRecordDPO.getSalesRecordListEndIteratorFilteredByTime(iter);
		}
		return iter;
	}

	SalesRecordList::Iterator SalesService::getSalesRecordListIteratorFilteredBySubstring(const char* substr, time_t timeLB, time_t timeUB) {
		SalesRecordList::Iterator iter;
		SalesRecordList::Iterator endIter;
		ItemInfo info;
		StringifiedItemInfo strInfo;
		salesRecordListFiltered.clear();
		iter = getSalesRecordListIterator(timeLB, timeUB);
		endIter = getSalesRecordListEndIterator(timeLB, timeUB);
		for (; iter != endIter; iter._setSelfNext()) {
			info.info.itemUID = iter->itemUID;
			displayStringMapDPO.getDisplayStrings(info, strInfo);
			if (strstr(strInfo.itemName, substr) != NULL
				|| strstr(strInfo.typeName, substr) != NULL
				|| strstr(strInfo.producer, substr) != NULL
				|| strstr(strInfo.spec, substr) != NULL) {
				salesRecordListFiltered.pushBack(*iter);
			}
		}
		return salesRecordListFiltered.begin();
	}

	SalesRecordList::Iterator SalesService::getSalesRecordListEndIteratorFiltered() {
		return salesRecordListFiltered.end();
	}


}
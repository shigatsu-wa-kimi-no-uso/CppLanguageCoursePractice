#pragma once
#include "../Config/typedef.h"
#include "../DPL/ItemInfoDPO.h"
#include "../Entity/ItemBatchInfo.h"
#include "../Entity/PurchaseRecord.h"
#include "../DPL/PurchaseRecordDPO.h"
#include "../DPL/ItemBatchInfoDPO.h"
#include "../DPL/DisplayStringMapDPO.h"

namespace Service
{
	class PurchaseService
	{
	private:
		static PurchaseRecordList purchaseRecordListFiltered;
		DPL::PurchaseRecordDPO purchaseRecordDPO;
		DPL::ItemBatchInfoDPO itemBatchInfoDPO;
		DPL::DisplayStringMapDPO displayStringMapDPO;
		DPL::ItemInfoDPO itemInfoDPO;
	public:
		void purchaseItem(ItemInfoList::Iterator& targetItem, DWORD purchaseAmount, double unitCost, DWORD& batchUID);
		PurchaseRecordList::Iterator getPurchaseRecordListIterator(time_t timeLB, time_t timeUB);
		PurchaseRecordList::Iterator getPurchaseRecordListEndIterator(time_t timeLB, time_t timeUB);
		PurchaseRecordList::Iterator getPurchaseRecordListIteratorFilteredBySubstring(const char* substr, time_t timeLB, time_t timeUB);
		PurchaseRecordList::Iterator getPurchaseRecordListEndIteratorFiltered();
	};
}

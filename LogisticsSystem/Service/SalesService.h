#pragma once
#include "../Config/typedef.h"
#include "../Entity/ItemBatchInfo.h"
#include "../Entity/SalesRecord.h"
#include "../Entity/ItemInfo.h"
#include "../DPL/SalesRecordDPO.h"
#include "../DPL/ItemInfoDPO.h"
#include "../DPL/ItemBatchInfoDPO.h"
#include "../DPL/DisplayStringMapDPO.h"
#include "../Entity/StringifiedItemInfo.h"

namespace Service
{
	class SalesService
	{
	private:
		static SalesRecordList salesRecordListFiltered;
		DPL::SalesRecordDPO salesRecordDPO;
		DPL::DisplayStringMapDPO displayStringMapDPO;
		DPL::ItemBatchInfoDPO itemBatchInfoDPO;
		DPL::ItemInfoDPO itemInfoDPO;
	public:
		void saleItem(ItemInfoList::Iterator& targetItem, ItemBatchInfoList::Iterator& targetBatch ,DWORD count,double unitPrice,DWORD& recordUID);
		SalesRecordList::Iterator getSalesRecordListIterator(time_t timeLB, time_t timeUB);
		SalesRecordList::Iterator getSalesRecordListEndIterator(time_t timeLB, time_t timeUB);
		SalesRecordList::Iterator getSalesRecordListIteratorFilteredBySubstring(const char* substr, time_t timeLB, time_t timeUB);
		SalesRecordList::Iterator getSalesRecordListEndIteratorFiltered();
	};
}

#pragma once
#include "../DPL/ItemBatchInfoDPO.h"
#include "../DPL/ItemInfoDPO.h"
#include "../Entity/StringifiedItemBatchInfo.h"

namespace Service
{
	class ItemBatchInfoService
	{
	private:
		DPL::ItemBatchInfoDPO itemBatchInfoDPO;
	public:
		bool addItemBatchInfo(ItemBatchInfo& info);
		ItemBatchInfoList::Iterator getItemBatchInfoListIterator(QWORD itemUID);
		ItemBatchInfoList::Iterator getItemBatchInfoListEndIterator(QWORD itemUID);
		void deleteItemBatchInfo(ItemBatchInfoList::Iterator& target, ItemInfoList::Iterator& targetItem);
		void getStringifiedItemBatchInfo(ItemBatchInfo& infoEntry, StringifiedItemBatchInfo& stringifiedInfo);
		void unloadMap();
	};
}


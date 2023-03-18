#pragma once
#ifndef ItemInfoService_h__
#define ItemInfoService_h__
#include "../Config/typedef.h"
#include "../Entity/StringifiedItemInfo.h"
#include "../Entity/ItemInfo.h"
#include "../DPL/DisplayStringMapDPO.h"
#include "../DPL/ItemInfoDPO.h"
#include "../DPL/ItemBatchInfoDPO.h"


namespace Service
{
	class ItemInfoService
	{
	private:
		using ItemInfoDPO = DPL::ItemInfoDPO;
		using ItemBatchInfoDPO = DPL::ItemBatchInfoDPO;
		using DisplayStringMapDPO = DPL::DisplayStringMapDPO;

		static ItemInfoList itemInfoListFiltered;
		ItemInfoDPO itemInfoDPO;
		DisplayStringMapDPO displayStringMapDPO;
		ItemBatchInfoDPO itemBatchInfoDPO;
	public:
		bool addItemInfo(StringifiedItemInfo& info,QWORD& itemUID,bool& isDuplicate);
		void updateItemInfoDisplayStrings(StringifiedItemInfo& newStringifiedInfo,ItemInfo& info);
		ItemInfoList::Iterator getItemInfoListIterator();
		ItemInfoList::Iterator getItemInfoListEndIterator();
		ItemInfoList::Iterator getItemInfoListIteratorFilteredBySubstring(const char* substr);
		ItemInfoList::Iterator getItemInfoListEndIteratorFiltered();
		void sortItemInfo(const char* policy);
		void generatePurchaseList();
		void getStringifiedItemInfo(ItemInfo& infoEntry, StringifiedItemInfo& stringifiedInfo);
	};
}

#endif // ItemInfoService_h__
#pragma once
#include "../Config/typedef.h"
#include "../DAL/ItemBatchInfoDAO.h"

namespace DPL
{
	class ItemBatchInfoDPO
	{
	private:
		static ItemBatchInfoMap batchInfoMap;
		static bool isLoaded;
		static bool isModified;
		DAL::ItemBatchInfoDAO itemBatchInfoDAO;
		static DWORD lastBatchID;
	public:
		ItemBatchInfoDPO();

		bool assembleIntoMap();

		void markModified();

		void deleteItemBatchInfo(ItemBatchInfoList::Iterator& target);

		void getItemBatchInfoListIterator(QWORD itemUID, ItemBatchInfoList::Iterator& iter);

		void getItemBatchInfoListEndIterator(QWORD itemUID, ItemBatchInfoList::Iterator& iter);

		DWORD getNewBatchID();

		void updateItemBatchInfo(const ItemBatchInfoList::Iterator& pos, const ItemBatchInfo& info);

		void addBatchInfo(ItemBatchInfo& batchInfo);

		bool syncUpdatesToFile();

		void unloadMap();
	};
}


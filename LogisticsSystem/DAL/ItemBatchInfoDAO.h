#pragma once
#include "../Config/FileName.h"
#include "../Entity/ItemBatchInfo.h"
#include "../Container/LinkedList.h"
#include "../Container/HashMap.h"

using ItemBatchInfoList = LinkedList<ItemBatchInfo>;
using ItemBatchInfoMap = HashMap<QWORD, ItemBatchInfoList>;

namespace DAL
{
	class ItemBatchInfoDAO
	{
	private:
		static literal char* batchInfoFileName = Config::FileName::batchInfo;

	public:
		bool createFile();
		bool update(ItemBatchInfoMap& infoMap);
		bool load(ItemBatchInfoMap& infoMap, DWORD& maxID);
	};
}

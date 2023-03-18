#include "ItemBatchInfoDPO.h"

namespace DPL
{
	ItemBatchInfoMap ItemBatchInfoDPO::batchInfoMap;
	bool ItemBatchInfoDPO::isLoaded = false;
	bool ItemBatchInfoDPO::isModified = false;
	DWORD ItemBatchInfoDPO::lastBatchID = 0;

	ItemBatchInfoDPO::ItemBatchInfoDPO() {
		if (!isLoaded) {
			assembleIntoMap();
		}
	}

	bool ItemBatchInfoDPO::assembleIntoMap() {
		return isLoaded = (itemBatchInfoDAO.load(batchInfoMap, lastBatchID) || itemBatchInfoDAO.createFile());
	}

	void ItemBatchInfoDPO::markModified() {
		isModified = true;
	}

	void ItemBatchInfoDPO::getItemBatchInfoListIterator(QWORD itemUID,ItemBatchInfoList::Iterator& iter) {
		if (batchInfoMap.hasKey(itemUID)) {
			iter = batchInfoMap[itemUID].begin();
		} else {
			iter = ItemBatchInfoList::Iterator();
		}
	}

	void ItemBatchInfoDPO::getItemBatchInfoListEndIterator(QWORD itemUID, ItemBatchInfoList::Iterator& iter) {
		if (batchInfoMap.hasKey(itemUID)) {
			iter = batchInfoMap[itemUID].end();
		}
	}

	DWORD ItemBatchInfoDPO::getNewBatchID() {
		return ++lastBatchID;
	}

	void ItemBatchInfoDPO::updateItemBatchInfo(const ItemBatchInfoList::Iterator& pos,const  ItemBatchInfo& info) {
		memcpy_s(&*pos, sizeof(ItemBatchInfo), &info, sizeof(info));
		isModified = true;
	}

	void ItemBatchInfoDPO::addBatchInfo(ItemBatchInfo& batchInfo) {
		if (batchInfoMap.hasKey(batchInfo.itemUID)) {
			batchInfoMap[batchInfo.itemUID].pushBack(batchInfo);
		} else {
			ItemBatchInfoList list;
			list.pushBack(batchInfo);
			batchInfoMap.put(batchInfo.itemUID, list);
		}
		isModified = true;
	}

	bool ItemBatchInfoDPO::syncUpdatesToFile() {
		if (isModified) {
			return itemBatchInfoDAO.update(batchInfoMap);
		}
		return true;
	}

	void ItemBatchInfoDPO::deleteItemBatchInfo(ItemBatchInfoList::Iterator& target) {
		batchInfoMap.get(target->itemUID)->value.remove(target);
		isModified = true;
	}

	void ItemBatchInfoDPO::unloadMap() {
		isModified = false;
		isLoaded = false;
		batchInfoMap.~HashMap();
	}
}
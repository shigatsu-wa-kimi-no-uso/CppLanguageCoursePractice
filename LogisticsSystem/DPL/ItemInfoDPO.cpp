#include "ItemInfoDPO.h"

namespace DPL
{
	ItemInfoList ItemInfoDPO::itemInfoList;
	ItemUIDMap ItemInfoDPO::itemUIDMap;
	bool ItemInfoDPO::isLoaded = false;
	bool ItemInfoDPO::isModified = false;

	ItemInfoDPO::ItemInfoDPO(){
		assembleIntoList();
	}

	bool ItemInfoDPO::assembleIntoList(){
		if (isLoaded) {
			return true;
		}
		DWORD itemCount;
		ItemInfo* buffer = itemInfoDAO.load(itemCount);
		if (buffer == nullptr) {
			isLoaded = itemInfoDAO.createFile();
			return isLoaded;
		}
		for (DWORD i = 0; i < itemCount; ++i) {
			itemInfoList.pushBack(buffer[i]);
			itemUIDMap.put(buffer[i].info.itemUID, nullptr);
		}
		if (itemCount == itemInfoList.size()) {
			delete buffer;
			isLoaded = true;
			return true;
		}
		itemInfoDAO.unload(buffer);
		return false;
	}

	void ItemInfoDPO::markModified() {
		isModified = true;
	}

	/*
	* 更新商品信息
	*/
	void ItemInfoDPO::updateItemInfo(ItemInfoList::Iterator& pos, ItemInfo& itemInfoEntry) {
		memcpy(&*pos, &itemInfoEntry, sizeof(itemInfoEntry));
		isModified = true;
	}

	void ItemInfoDPO::updateItemStatus(ItemInfoList::Iterator& pos) {
		if (pos->remainingCount == 0) {
			pos->status = ItemStatus::SOLD_OUT;
		} else if (pos->recentSoldCount != 0 && (double)pos->remainingCount / pos->recentSoldCount < NEEDSUPPLEMENT_THRESHOLD) {
			pos->status = ItemStatus::NEED_SUPPLEMENT;
		} else {
			pos->status = ItemStatus::NORMAL;
		}
	}

	void ItemInfoDPO::addItemInfo(ItemInfo& itemInfoEntry) {
		itemInfoList.pushBack(itemInfoEntry);
		itemUIDMap.put(itemInfoEntry.info.itemUID,nullptr);
		isModified = true;
	}


	bool ItemInfoDPO::hasExisted(QWORD uid) {
		return itemUIDMap.hasKey(uid);
	}

	ItemInfoList::Iterator ItemInfoDPO::getIterator() {
		return itemInfoList.begin();
	}

	bool ItemInfoDPO::syncUpdatesToFile() {
		if (!isModified) {
			return true;
		}
		return itemInfoDAO.update(itemInfoList);
	}


	ItemInfoList::Iterator ItemInfoDPO::getEndIterator() {
		return itemInfoList.end();
	}
}
#include "ItemInfoService.h"
#include "../Utility/FileUtil.h"

using namespace Util;

namespace Service
{

	ItemInfoList ItemInfoService::itemInfoListFiltered;

	bool ItemInfoService::addItemInfo(StringifiedItemInfo& info, QWORD& itemUID, bool& isDuplicate) {
		isDuplicate = false;
		ItemInfo itemInfo;
		memset(&itemInfo, 0, sizeof(itemInfo));
		displayStringMapDPO.getItemInfoIDs(info, itemInfo);
		if (itemInfoDPO.hasExisted(itemInfo.info.itemUID)) {
			isDuplicate = true;
			return false;
		}
		itemInfo.status = ItemStatus::SOLD_OUT;
		itemInfoDPO.addItemInfo(itemInfo);
		return itemInfoDPO.syncUpdatesToFile()
			&& displayStringMapDPO.syncUpdatesToFile()
			&& ((itemUID = itemInfo.info.itemUID), true);
	}

	void ItemInfoService::updateItemInfoDisplayStrings(StringifiedItemInfo& newStringifiedInfo, ItemInfo& info) {
		return;
	}

	ItemInfoList::Iterator ItemInfoService::getItemInfoListIteratorFilteredBySubstring(const char* substr) {
		itemInfoListFiltered.clear();
		ItemInfoList::Iterator iter = itemInfoDPO.getIterator();
		ItemInfoList::Iterator endIter = itemInfoDPO.getEndIterator();
		StringifiedItemInfo strInfo;
		for (; iter != endIter; iter._setSelfNext()) {
			getStringifiedItemInfo(*iter, strInfo);
			if (strstr(strInfo.itemName, substr) != NULL
			|| strstr(strInfo.typeName, substr) != NULL 
			|| strstr(strInfo.producer,substr)!=NULL 
			|| strstr(strInfo.spec,substr)!=NULL){
				itemInfoListFiltered.pushBack(*iter);
			}
		}
		return itemInfoListFiltered.begin();
	}

	ItemInfoList::Iterator ItemInfoService::getItemInfoListEndIteratorFiltered() {
		return itemInfoListFiltered.end();
	}

	ItemInfoList::Iterator  ItemInfoService::getItemInfoListIterator() {
		return itemInfoDPO.getIterator();
	}

	ItemInfoList::Iterator  ItemInfoService::getItemInfoListEndIterator() {
		return itemInfoDPO.getEndIterator();
	}

	void  ItemInfoService::getStringifiedItemInfo(ItemInfo& infoEntry, StringifiedItemInfo& stringifiedInfo) {
		displayStringMapDPO.getDisplayStrings(infoEntry, stringifiedInfo);
	}

	void ItemInfoService::generatePurchaseList() {
		ItemInfoList::Iterator iter;
		ItemInfoList::Iterator endIter;
		FileUtil fileUtil;
		StringifiedItemInfo strInfo;
		extern const char* path;
		
		char outputPath[240];

		sprintf_s(outputPath, "%s\\采购通知单.txt", path);

		fileUtil.open(outputPath, "wb");
		iter = itemInfoDPO.getIterator();
		endIter = itemInfoDPO.getEndIterator();
		char buffer[512];
		sprintf_s(buffer,"********************************************************采购通知单********************************************************\n");
		fileUtil.write((BYTE*)buffer, strlen(buffer) + 1);
		sprintf_s(buffer, "%24s|%16s|%24s|%28s|%12s|%12s\n", "商品名", "类型", "生产商", "规格","当前库存","状态");
		fileUtil.write((BYTE*)buffer, strlen(buffer) + 1);
		for (; iter != endIter; iter._setSelfNext()) {
			if (iter->status != ItemStatus::NEED_SUPPLEMENT && iter->status !=ItemStatus::SOLD_OUT) {
				continue;
			}
			displayStringMapDPO.getDisplayStrings(*iter,strInfo);
			sprintf_s(buffer, "%24s|%16s|%24s|%28s|%12s|%12s\n", 
				strInfo.itemName, 
				strInfo.typeName, 
				strInfo.producer,
				strInfo.spec,
				strInfo.remainingCount,
				strInfo.status
				);
			fileUtil.write((BYTE*)buffer, strlen(buffer) + 1);
		}
		fileUtil.close();
	}

}
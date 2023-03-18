#include "ItemBatchInfoDAO.h"
#include "../Utility/FileUtil.h"
#include "../Entity/ItemBatchInfo.h"

using namespace Util;
namespace DAL
{
	bool ItemBatchInfoDAO::update(ItemBatchInfoMap& infoMap) {
		FileUtil fileUtil;
		bool result = fileUtil.open(batchInfoFileName,"wb");
		if (!result) {
			return false;
		}
		for (ItemBatchInfoMap::Entry& entry : infoMap) {
			for (ItemBatchInfo& info : entry.value) {
				result = fileUtil.write((BYTE*)&info, sizeof(info));
				if (!result) {
					return false;
				}
			}
		}
		return true;
	}

	bool ItemBatchInfoDAO::createFile() {
		FileUtil fileUtil;
		return fileUtil.open(batchInfoFileName, "wb")
			&& fileUtil.close();
	}

	bool ItemBatchInfoDAO::load(ItemBatchInfoMap& infoMap,DWORD& maxID) {
		FileUtil fileUtil;
		maxID = 0;
		bool result = fileUtil.open(batchInfoFileName, "rb");
		if (!result) {
			return false;
		}
		DWORD size = fileUtil.getFileSize();
		for (DWORD i = 0; i < size; i += sizeof(ItemBatchInfo)) {
			ItemBatchInfo info;
			result = fileUtil.read((BYTE*)&info, sizeof(ItemBatchInfo));
			if (!result) {
				fileUtil.close();
				return false;
			}
			if (infoMap.hasKey(info.itemUID)) {
				infoMap[info.itemUID].pushBack(info);
			} else {
				ItemBatchInfoList lst;
				lst.pushBack(info);
				infoMap.put(info.itemUID, lst);
			}
			maxID = info.batchUID > maxID ? info.batchUID : maxID;
		}
		fileUtil.close();
		return true;
	}
}


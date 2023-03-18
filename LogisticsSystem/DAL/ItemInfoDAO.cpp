#include "ItemInfoDAO.h"
#include "../Utility/FileUtil.h"
#include "../Config/FileName.h"

namespace DAL
{
	using namespace Util;
	/*
	bool ItemInfoDAO::addItemInfo(ItemInfoEntry& itemInfo) {
		FileUtil fileUtil;
		return fileUtil.open(Config::FileName::ItemInfo, "ab") &&
			fileUtil.write((BYTE*)&itemInfo, sizeof(itemInfo)) && 
			fileUtil.close();
	}*/


	bool ItemInfoDAO::update(LinkedList<ItemInfo>& entries) {
		FileUtil fileUtil;
		bool result = fileUtil.open(itemInfoFileName, "wb");
		if (!result) {
			return false;
		}
		for (ItemInfo& entry : entries) {
			if (!fileUtil.write((BYTE*)&entry, sizeof(entry))) {
				fileUtil.close();
				return false;
			}
		}
		fileUtil.close();
		return true;
	}

	bool ItemInfoDAO::createFile() {
		FileUtil fileUtil;
		return fileUtil.open(itemInfoFileName, "wb")
			&& fileUtil.close();
	}

	ItemInfo* ItemInfoDAO::load(DWORD& itemCount) {
		FileUtil fileUtil;
		DWORD count;
		bool result;
		result = fileUtil.open(itemInfoFileName, "rb");
		if (result) {
			count = fileUtil.getFileSize()/sizeof(ItemInfo);
			ItemInfo* buffer = new ItemInfo[count];
			result = fileUtil.readAll((BYTE*)buffer,0);
			if (result) {
				itemCount = count;
				fileUtil.close();
				return buffer;
			}
			fileUtil.close();
		}
		return nullptr;
	}

	void ItemInfoDAO::unload(ItemInfo* pBuf) {
		delete[] pBuf;
	}
}
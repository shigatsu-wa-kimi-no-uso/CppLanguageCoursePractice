#include "../Config/FileName.h"
#include "PurchaseRecordDAO.h"
#include "../Utility/FileUtil.h"
using namespace Util;

namespace DAL
{
	bool PurchaseRecordDAO::update(PurchaseRecordList& recordList) {
		FileUtil fileUtil;
		bool result = fileUtil.open(purchaseRecordFileName, "wb");
		if (!result) {
			return false;
		}
		for (PurchaseRecord& record : recordList) {
			result = fileUtil.write((BYTE*)&record, sizeof(record));
			if (!result) {
				return false;
			}

		}
		return true;
	}

	bool PurchaseRecordDAO::load(PurchaseRecordList& recordList, DWORD& maxID) {
		FileUtil fileUtil;
		maxID = 0;
		bool result = fileUtil.open(purchaseRecordFileName, "rb");
		if (!result) {
			return false;
		}
		DWORD size = fileUtil.getFileSize();
		for (DWORD i = 0; i < size; i += sizeof(PurchaseRecord)) {
			PurchaseRecord record;
			result = fileUtil.read((BYTE*)&record, sizeof(PurchaseRecord));
			if (!result) {
				return false;
			}
			recordList.pushBack(record);
			maxID = record.recordUID > maxID ? record.recordUID : maxID;
		}
		return true;
	}

	bool PurchaseRecordDAO::createFile() {
		FileUtil fileUtil;
		return fileUtil.open(purchaseRecordFileName, "wb")
			&& fileUtil.close();
	}
}
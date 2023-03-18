#include "SalesRecordDAO.h"
#include "../Config/FileName.h"
#include "../Utility/FileUtil.h"
using namespace Util;

namespace DAL
{
	bool SalesRecordDAO::update(SalesRecordList& recordList) {
		FileUtil fileUtil;
		bool result = fileUtil.open(salesRecordFileName, "wb");
		if (!result) {
			return false;
		}
		for (SalesRecord& record : recordList) {
			result = fileUtil.write((BYTE*)&record, sizeof(record));
			if (!result) {
				return false;
			}

		}
		return true;
	}

	bool SalesRecordDAO::load(SalesRecordList& recordList,DWORD& maxID) {
		FileUtil fileUtil;
		maxID = 0;
		bool result = fileUtil.open(salesRecordFileName, "rb");
		if (!result) {
			return false;
		}
		DWORD size = fileUtil.getFileSize();
		for (int i = 0; i < size; i += sizeof(SalesRecord)) {
			SalesRecord record;
			result = fileUtil.read((BYTE*)&record, sizeof(SalesRecord));
			if (!result) {
				return false;
			}
			recordList.pushBack(record);
			maxID = record.recordUID > maxID ? record.recordUID : maxID;
		}
		return true;
	}

	bool SalesRecordDAO::createFile() {
		FileUtil fileUtil;
		return fileUtil.open(salesRecordFileName, "wb")
			&& fileUtil.close();
	}
}
#pragma once
#include "../Config/typedef.h"
#include "../Config/FileName.h"
#include "../Entity/PurchaseRecord.h"
#include "../Container/LinkedList.h"

using PurchaseRecordList = LinkedList<PurchaseRecord>;

namespace DAL
{
	class PurchaseRecordDAO
	{
	private:
		static literal char* purchaseRecordFileName = Config::FileName::purchaseRecord;

	public:
		bool update(PurchaseRecordList& recordList);
		bool load(PurchaseRecordList& recordList,DWORD & maxID);
		bool createFile();
	};
}


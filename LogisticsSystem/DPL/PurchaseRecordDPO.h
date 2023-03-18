#pragma once
#include "../Config/typedef.h"
#include "../Entity/PurchaseRecord.h"
#include "../DAL/PurchaseRecordDAO.h"

namespace DPL
{
	class PurchaseRecordDPO
	{
	private:
		DAL::PurchaseRecordDAO purchaseRecordDAO;
		static PurchaseRecordList purchaseRecordList;
		static PurchaseRecordList purchaseRecordListFiltered;
		static bool isModified;
		static bool isLoaded;
		static DWORD lastRecordID;
	public:
		PurchaseRecordDPO();
		bool assembleIntoRecordList();
		void addPurchaseRecord(PurchaseRecord& purchaseRecord);
		DWORD getNewRecordID();
		bool syncUpdatesToFile();
		void updatePurchaseRecordList(PurchaseRecordList::Iterator& pos, PurchaseRecordList& info);
		void getPurchaseRecordListEndIteratorFilteredByTime(PurchaseRecordList::Iterator& iter);
		void getPurchaseRecordListIteratorFilteredByTime(PurchaseRecordList::Iterator& iter, time_t timeLB, time_t timeUB);
		void getPurchaseRecordListIterator(PurchaseRecordList::Iterator& iter);
		void getPurchaseRecordListEndIterator(PurchaseRecordList::Iterator& iter);
	};
}


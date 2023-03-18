#pragma once
#include "../Config/typedef.h"
#include "../Entity/SalesRecord.h"
#include "../DAL/SalesRecordDAO.h"

namespace DPL
{
	class SalesRecordDPO
	{
	private:
		DAL::SalesRecordDAO salesRecordDAO;
		static SalesRecordList salesRecordListFiltered;
		static SalesRecordList salesRecordList;
		static bool isModified;
		static bool isLoaded;
		static DWORD lastRecordID;
	public:
		SalesRecordDPO();
		bool assembleIntoRecordList();
		void addSalesRecord(SalesRecord& SalesRecord);
		DWORD getNewRecordID();
		bool syncUpdatesToFile();
		void updateSalesRecordList(SalesRecordList::Iterator& pos, SalesRecordList& info);
		void getSalesRecordListIterator(SalesRecordList::Iterator& iter);
		void getSalesRecordListEndIterator(SalesRecordList::Iterator& iter);
		void getSalesRecordListIteratorFilteredByTime(SalesRecordList::Iterator& iter, time_t timeLB, time_t timeUB);
		void getSalesRecordListEndIteratorFilteredByTime(SalesRecordList::Iterator& iter);
	};
}


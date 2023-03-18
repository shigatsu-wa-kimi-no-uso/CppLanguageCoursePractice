#include "SalesRecordDPO.h"

namespace DPL
{
	bool SalesRecordDPO::isLoaded = false;
	bool SalesRecordDPO::isModified = false;
	DWORD SalesRecordDPO::lastRecordID = 0;
	SalesRecordList SalesRecordDPO::salesRecordList;
	SalesRecordList SalesRecordDPO::salesRecordListFiltered;

	SalesRecordDPO::SalesRecordDPO() {
		if (!isLoaded) {
			assembleIntoRecordList();
		}
	}

	bool SalesRecordDPO::assembleIntoRecordList() {
		return isLoaded = (salesRecordDAO.load(salesRecordList,lastRecordID) || salesRecordDAO.createFile());
	}

	void SalesRecordDPO::getSalesRecordListIterator(SalesRecordList::Iterator& iter) {
		iter = salesRecordList.begin();
	}

	void SalesRecordDPO::getSalesRecordListEndIterator(SalesRecordList::Iterator& iter) {
		iter = salesRecordList.end();
	}

	DWORD SalesRecordDPO::getNewRecordID() {
		return ++lastRecordID;
	}

	void SalesRecordDPO::updateSalesRecordList(SalesRecordList::Iterator& pos, SalesRecordList& info) {
		memcpy_s(&*pos, sizeof(SalesRecordList), &info, sizeof(info));
		isModified = true;
	}

	void SalesRecordDPO::addSalesRecord(SalesRecord& salesRecord) {
		salesRecordList.pushBack(salesRecord);
		isModified = true;
	}

	bool SalesRecordDPO::syncUpdatesToFile() {
		if (isModified) {
			return salesRecordDAO.update(salesRecordList);
		}
		return true;
	}

	void SalesRecordDPO::getSalesRecordListIteratorFilteredByTime(SalesRecordList::Iterator& iter, time_t timeLB, time_t timeUB) {
		salesRecordListFiltered.clear();
		for (SalesRecord& record : salesRecordList) {
			if (record.recordTime >= timeLB && record.recordTime <= timeUB) {
				salesRecordListFiltered.pushBack(record);
			}
		}
		iter = salesRecordListFiltered.begin();
	}

	void SalesRecordDPO::getSalesRecordListEndIteratorFilteredByTime(SalesRecordList::Iterator& iter)
	{
		iter = salesRecordListFiltered.end();
	}
}
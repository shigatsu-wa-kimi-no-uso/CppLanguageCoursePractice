#include "PurchaseRecordDPO.h"

namespace DPL
{
	bool PurchaseRecordDPO::isLoaded = false;
	bool PurchaseRecordDPO::isModified = false;
	DWORD PurchaseRecordDPO::lastRecordID = 0;
	PurchaseRecordList PurchaseRecordDPO::purchaseRecordList;
	PurchaseRecordList PurchaseRecordDPO::purchaseRecordListFiltered;

	PurchaseRecordDPO::PurchaseRecordDPO() {
		if (!isLoaded) {
			assembleIntoRecordList();
		}
	}

	bool PurchaseRecordDPO::assembleIntoRecordList() {
		return isLoaded = (purchaseRecordDAO.load(purchaseRecordList,lastRecordID) || purchaseRecordDAO.createFile());
	}

	void PurchaseRecordDPO::getPurchaseRecordListIterator(PurchaseRecordList::Iterator& iter) {
		iter = purchaseRecordList.begin();
	}

	void PurchaseRecordDPO::getPurchaseRecordListEndIterator( PurchaseRecordList::Iterator& iter) {
		iter = purchaseRecordList.end();
	}

	void  PurchaseRecordDPO::getPurchaseRecordListIteratorFilteredByTime(PurchaseRecordList::Iterator& iter, time_t timeLB, time_t timeUB) {
		purchaseRecordListFiltered.clear();
		for (PurchaseRecord& record : purchaseRecordList) {
			if (record.timeOfPurchase >= timeLB && record.timeOfPurchase <= timeUB) {
				purchaseRecordListFiltered.pushBack(record);
			}
		}
		iter = purchaseRecordListFiltered.begin();
	}

	void PurchaseRecordDPO::getPurchaseRecordListEndIteratorFilteredByTime(PurchaseRecordList::Iterator& iter)
	{
		iter = purchaseRecordListFiltered.end();
	}

	DWORD PurchaseRecordDPO::getNewRecordID() {
		return ++lastRecordID;
	}

	void PurchaseRecordDPO::updatePurchaseRecordList(PurchaseRecordList::Iterator& pos, PurchaseRecordList& info) {
		memcpy_s(&*pos, sizeof(PurchaseRecordList), &info, sizeof(info));
		isModified = true;
	}

	void PurchaseRecordDPO::addPurchaseRecord(PurchaseRecord& purchaseRecord) {
		purchaseRecordList.pushBack(purchaseRecord);
		isModified = true;
	}

	bool PurchaseRecordDPO::syncUpdatesToFile() {
		if (isModified) {
			return purchaseRecordDAO.update(purchaseRecordList);
		}
		return true;
	}


}
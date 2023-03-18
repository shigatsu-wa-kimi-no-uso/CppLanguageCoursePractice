#include "DisplayStringMapDPO.h"
#include "../Entity/StringifiedItemInfo.h"
#include "../Entity/ItemInfo.h"
#include "../Utility/ConsoleUtil.h"
#include <string.h>
using namespace Util;

namespace DPL
{
	StringMap DisplayStringMapDPO::DisplayStringMaps::item;
	StringMap DisplayStringMapDPO::DisplayStringMaps::type;
	StringMap DisplayStringMapDPO::DisplayStringMaps::producer;
	StringMap DisplayStringMapDPO::DisplayStringMaps::spec;
	
	IDMap DisplayStringMapDPO::IDMaps::item;
	IDMap DisplayStringMapDPO::IDMaps::type;
	IDMap DisplayStringMapDPO::IDMaps::producer;
	IDMap DisplayStringMapDPO::IDMaps::spec;

	WORD DisplayStringMapDPO::lastItemID;
	WORD DisplayStringMapDPO::lastTypeID;
	WORD DisplayStringMapDPO::lastProducerID;
	WORD DisplayStringMapDPO::lastSpecID;
	DWORD DisplayStringMapDPO::lastSalesRecordUID;
	DWORD DisplayStringMapDPO::lastPurchaseRecordUID;
	DWORD DisplayStringMapDPO::lastBatchNumber;


	bool DisplayStringMapDPO::DisplayStringMaps::isModified = false;
	bool DisplayStringMapDPO::DisplayStringMaps::isLoaded = false;
	bool DisplayStringMapDPO::IDMaps::isModified = false;
	bool DisplayStringMapDPO::IDMaps::isLoaded = false;


	DisplayStringMapDPO::DisplayStringMapDPO(){
		assembleDisplayStringMaps();
		assembleIDMaps();
	}

	void DisplayStringMapDPO::_allocAndAddToStringMap(DWORD id, const char* dispString, StringMap& map,IDMap& idMap) {
		size_t size = strlen(dispString) + 1;
		char* buffer = new char[size];
		strcpy_s(buffer, size ,dispString);
		map.put(id, buffer);
		idMap.put(buffer, id);
		DisplayStringMaps::isModified = true;
	}

	void DisplayStringMapDPO::getDisplayStrings(ItemInfo& infoEntry, StringifiedItemInfo& stringifiedInfo) {
		strcpy_s(stringifiedInfo.itemName, DisplayStringMaps::item[infoEntry.itemID]);
		strcpy_s(stringifiedInfo.typeName, DisplayStringMaps::type[infoEntry.typeID]);
		strcpy_s(stringifiedInfo.producer, DisplayStringMaps::producer[infoEntry.producerID]);
		strcpy_s(stringifiedInfo.spec, DisplayStringMaps::spec[infoEntry.specID]);
		sprintf_s(stringifiedInfo.remainingCount, "%u", infoEntry.remainingCount);
		sprintf_s(stringifiedInfo.status, "%s", [&]{
			switch (infoEntry.status) {
			case ItemStatus::DELETED:
				return "已删除";
			case ItemStatus::NEED_SUPPLEMENT:
				return "需要补充";
			case ItemStatus::NORMAL:
				return "正常";
			case ItemStatus::SOLD_OUT:
				return "售罄";
			default:
				return "N/A";
			}
		}());
	}


	void DisplayStringMapDPO::getItemInfoIDs(const StringifiedItemInfo& strInfo, ItemInfo& info) {
		WORD id;
		int flag = 0;
		if ((id=getItemID(strInfo.itemName)) != (WORD)~0) {
			info.itemID = id;
			++flag;
		} else {
			info.itemID = ++lastItemID;
			_allocAndAddToStringMap(info.itemID, strInfo.itemName, DisplayStringMaps::item,IDMaps::item);
		}
		if ((id = getTypeID(strInfo.typeName)) != (WORD)~0) {
			info.typeID = id;
			++flag;
		} else {
			info.typeID = ++lastTypeID;
			_allocAndAddToStringMap(info.typeID, strInfo.typeName, DisplayStringMaps::type,IDMaps::type);
		}
		if ((id = getSpecID(strInfo.spec)) != (WORD)~0) {
			info.specID = id;
			++flag;
		} else {
			info.specID = ++lastSpecID;
			_allocAndAddToStringMap(info.specID, strInfo.spec, DisplayStringMaps::spec,IDMaps::spec);
		}
		if ((id = getProducerID(strInfo.producer)) != (WORD)~0) {
			info.producerID = id;
			++flag;
		} else {
			info.producerID = ++lastProducerID;
			_allocAndAddToStringMap(info.producerID, strInfo.producer, DisplayStringMaps::producer,IDMaps::producer);
		}
	}

	void DisplayStringMapDPO::_deleteStringMapEntry(DWORD id, StringMap* map) {
		map->removeIf([&](StringMap::Iterator& iter) {
			if (id == iter->key) {
				delete iter->value;
				return true;
			} else {
				return false;
			}
		});
		DisplayStringMaps::isModified = true;
	}

	bool DisplayStringMapDPO::assembleDisplayStringMaps() {
		if (DisplayStringMaps::isLoaded) return true;
		return (displayStringMapDAO.loadItemStringMap(DisplayStringMaps::item, lastItemID, DisplayStrLenLimits::item)
			|| displayStringMapDAO.createItemStringMapFile())
			&& (displayStringMapDAO.loadTypeStringMap(DisplayStringMaps::type, lastTypeID, DisplayStrLenLimits::type)
				|| displayStringMapDAO.createTypeStringMapFile())
			&& (displayStringMapDAO.loadProducerStringMap(DisplayStringMaps::producer, lastProducerID, DisplayStrLenLimits::producer)
				|| displayStringMapDAO.createProducerStringMapFile())
			&& (displayStringMapDAO.loadSpecStringMap(DisplayStringMaps::spec, lastSpecID, DisplayStrLenLimits::spec)
				|| displayStringMapDAO.createSpecStringMapFile())
			&& (DisplayStringMaps::isLoaded = true);
	}

	void DisplayStringMapDPO::_assembleIDMaps(StringMap& strMap, IDMap& idMap) {
		for (StringMap::Entry& entry : strMap) {
			String str = entry.value;
			idMap.put(std::move(str), entry.key);
		}
	}

	void DisplayStringMapDPO::assembleIDMaps() {
		if (IDMaps::isLoaded) return;
		_assembleIDMaps(DisplayStringMaps::item, IDMaps::item);
		_assembleIDMaps(DisplayStringMaps::type, IDMaps::type);
		_assembleIDMaps(DisplayStringMaps::producer, IDMaps::producer);
		_assembleIDMaps(DisplayStringMaps::spec, IDMaps::spec);
		IDMaps::isLoaded = true;
	}

	void DisplayStringMapDPO::deleteStringMapEntries(const ItemInfo& itemInfoEntry) {
		_deleteStringMapEntry(itemInfoEntry.itemID, &DisplayStringMaps::item);
		_deleteStringMapEntry(itemInfoEntry.typeID, &DisplayStringMaps::type);
		_deleteStringMapEntry(itemInfoEntry.specID, &DisplayStringMaps::spec);
		_deleteStringMapEntry(itemInfoEntry.producerID, &DisplayStringMaps::producer);
	}

	bool DisplayStringMapDPO::syncUpdatesToFile() {
		if (!DisplayStringMaps::isModified) {
			return true;
		}
		return displayStringMapDAO.updateItemStringMap(DisplayStringMaps::item)
			&& displayStringMapDAO.updateSpecStringMap(DisplayStringMaps::spec)
			&& displayStringMapDAO.updateProducerStringMap(DisplayStringMaps::producer)
			&& displayStringMapDAO.updateTypeStringMap(DisplayStringMaps::type)
			&& ((DisplayStringMaps::isModified = false), true);
	}

	WORD DisplayStringMapDPO::getItemID(const char* item) {
		IDMap::Iterator iter = IDMaps::item.get(item);
		if (iter == IDMaps::item.end()) {
			return ~0;
		}
		return iter->value;
	}

	WORD DisplayStringMapDPO::getTypeID(const char* type) {
		IDMap::Iterator iter = IDMaps::type.get(type);
		if (iter == IDMaps::type.end()) {
			return ~0;
		}
		return iter->value;
	}

	WORD DisplayStringMapDPO::getProducerID(const char* producer) {
		IDMap::Iterator iter = IDMaps::producer.get(producer);
		if (iter == IDMaps::producer.end()) {
			return ~0;
		}
		return iter->value;
	}

	WORD DisplayStringMapDPO::getSpecID(const char* spec) {
		IDMap::Iterator iter = IDMaps::spec.get(spec);
		if (iter == IDMaps::spec.end()) {
			return ~0;
		}
		return iter->value;
	}
}
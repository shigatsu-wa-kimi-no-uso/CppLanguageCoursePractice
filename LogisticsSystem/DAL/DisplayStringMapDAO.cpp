#include "DisplayStringMapDAO.h"
#include "../Utility/FileUtil.h"
#include <string.h>
#include <stdio.h>

namespace DAL
{
	using namespace Util;

	bool DisplayStringMapDAO::_update(StringMap& updatedMap, const char* mapFile) {
		FileUtil fileUtil;
		bool result = fileUtil.open(mapFile, "wb");
		if (!result) {
			return false;
		}
		char buffer[256];
		for (StringMap::Entry entry : updatedMap) {
			sprintf_s(buffer, "%hu %s\n", entry.key,entry.value);
			result = fileUtil.write((BYTE*)buffer, strlen(buffer));
			if (!result) {
				fileUtil.close();
				return false;
			}
		}
		fileUtil.close();
		return true;
	}

	bool DisplayStringMapDAO::updateItemStringMap(StringMap& updatedMap) {
		return _update(updatedMap, StringMapFiles::item);
	}

	bool DisplayStringMapDAO::updateTypeStringMap(StringMap& updatedMap) {
		return _update(updatedMap, StringMapFiles::type);
	}
	bool DisplayStringMapDAO::updateProducerStringMap(StringMap& updatedMap) {
		return _update(updatedMap, StringMapFiles::producer);
	}
	bool DisplayStringMapDAO::updateSpecStringMap(StringMap& updatedMap) {
		return _update(updatedMap, StringMapFiles::spec);
	}


	bool DisplayStringMapDAO::_load(StringMap& map, WORD& maxID, const char* mapFile, DWORD strLenLimit) {
		FileUtil fileUtil;
		DWORD count;
		bool result;
		DWORD byteCount;
		result = fileUtil.open(mapFile, "rb");
		if (!result) {
			return false;
		}
		count = fileUtil.getFileSize();
		BYTE* buffer = new BYTE[count];
		result = fileUtil.readAll((BYTE*)buffer, 0);
		fileUtil.close();
		if (result) {
			byteCount = count;
		} else {
			return false;
		}
		
		int delta;
		int successCnt;
		for (long offset = 0; offset < byteCount; offset += delta) {
			WORD uid;
			char* dispStr = new char[strLenLimit];
			successCnt = sscanf_s((char*)&buffer[offset], "%hu %s\n%n", &uid, dispStr, strLenLimit, &delta);
			if (successCnt != 2) return false;
			maxID = uid > maxID ? uid : maxID;
			map.put(uid, dispStr);
		}
		return true;
	}

	bool DisplayStringMapDAO::loadTypeStringMap( StringMap& map, WORD& maxID, int strLenLimit) {
		return _load(map, maxID, StringMapFiles::type, strLenLimit);
	}

	bool DisplayStringMapDAO::loadItemStringMap(StringMap& map, WORD& maxID, int strLenLimit) {
		return _load(map, maxID, StringMapFiles::item, strLenLimit);
	}
	bool DisplayStringMapDAO::loadSpecStringMap(StringMap& map, WORD& maxID, int strLenLimit) {
		return _load(map, maxID, StringMapFiles::spec, strLenLimit);
	}

	bool DisplayStringMapDAO::loadProducerStringMap(StringMap& map, WORD& maxID, int strLenLimit) {
		return _load(map, maxID, StringMapFiles::producer, strLenLimit);
	}

	bool DisplayStringMapDAO::_createFile(const char* mapFile) {
		FileUtil fileUtil;
		return fileUtil.open(mapFile, "wb")
			&& fileUtil.close();
	}

	bool DisplayStringMapDAO::createItemStringMapFile() {
		return _createFile(StringMapFiles::item);
	}

	bool DisplayStringMapDAO::createTypeStringMapFile() {
		return _createFile(StringMapFiles::type);
	}

	bool DisplayStringMapDAO::createProducerStringMapFile() {
		return _createFile(StringMapFiles::producer);
	}

	bool DisplayStringMapDAO::createSpecStringMapFile() {
		return _createFile(StringMapFiles::spec);
	}



}
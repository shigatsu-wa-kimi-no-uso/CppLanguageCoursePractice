#pragma once
#ifndef DisplayStringMapDAO_h__
#define DisplayStringMapDAO_h__
#include "../Config/typedef.h"
#include "../Config/FileName.h"
#include "../Container/LinkedList.h"
#include "../Container/HashMap.h"
#include "String.h"

using StringMap = HashMap<WORD, const char*>;
using IDMap = HashMap<String, WORD>;

namespace DAL
{
	class DisplayStringMapDAO
	{
	private:
		struct StringMapFiles
		{
			static literal char* item = Config::FileName::StringMapFile::item;
			static literal char* type = Config::FileName::StringMapFile::type;
			static literal char* producer = Config::FileName::StringMapFile::producer;
			static literal char* spec = Config::FileName::StringMapFile::spec;
		};


		/*
		* ¸üÐÂID-×Ö·û´®Ó³Éä
		*/
		bool _update(StringMap& updatedMap, const char* mapFile);


	public:
		

		bool updateItemStringMap(StringMap& updatedMap);

		bool updateTypeStringMap(StringMap& updatedMap);

		bool updateProducerStringMap(StringMap& updatedMap);

		bool updateSpecStringMap(StringMap& updatedMap);


		/*
		* ¼ÓÔØID-×Ö·û´®Ó³Éä
		*/
		bool _load(StringMap& map, WORD& maxID, const char* mapFile, DWORD strLenLimit);

		bool loadItemStringMap(StringMap& map, WORD& maxID, int strLenLimit);

		bool loadTypeStringMap(StringMap& map, WORD& maxID, int strLenLimit);

		bool loadProducerStringMap(StringMap& map, WORD& maxID, int strLenLimit);

		bool loadSpecStringMap(StringMap& map, WORD& maxID, int strLenLimit);

		bool _createFile(const char* mapFile);

		bool createItemStringMapFile();

		bool createTypeStringMapFile();

		bool createProducerStringMapFile();

		bool createSpecStringMapFile();

		/*
		* É¾³ýID-×Ö·û´®Ó³Éä
		*/
		bool deleteItemInfo(DWORD id, const char* mapFile);


	};
}

#endif // DisplayStringMapDAO_h__

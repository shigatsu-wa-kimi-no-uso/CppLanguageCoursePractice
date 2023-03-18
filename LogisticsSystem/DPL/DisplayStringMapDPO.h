#pragma once
#ifndef DisplayStringMapDPO_h__
#define DisplayStringMapDPO_h__
#include "../Config/typedef.h"
#include "../DAL/DisplayStringMapDAO.h"
#include "../Entity/StringifiedItemInfo.h"
#include "../Entity/ItemInfo.h"


namespace DPL
{
	class DisplayStringMapDPO
	{
	private:

		DAL::DisplayStringMapDAO displayStringMapDAO;
		static WORD lastItemID;
		static WORD lastTypeID;
		static WORD lastProducerID;
		static WORD lastSpecID;
		static DWORD lastSalesRecordUID;
		static DWORD lastPurchaseRecordUID;
		static DWORD lastBatchNumber;


		struct DisplayStrLenLimits
		{
			static literal DWORD item = Config::Limits::DisplayString::ITEM;
			static literal DWORD type = Config::Limits::DisplayString::TYPE;
			static literal DWORD producer = Config::Limits::DisplayString::PRODUCER;
			static literal DWORD spec = Config::Limits::DisplayString::SPEC;
		};

		/*
		* ID --> string ”≥…‰
		*/
		struct DisplayStringMaps
		{
			static StringMap item;
			static StringMap type;
			static StringMap producer;
			static StringMap spec;
			static bool isLoaded;
			static bool isModified;
		};

		/*
		* string --> ID ”≥…‰
		*/
		struct IDMaps 
		{
			static IDMap item;
			static IDMap type;
			static IDMap producer;
			static IDMap spec;
			static bool isLoaded;
			static bool isModified;
		};

		bool _syncUpdatesToFile(const char* mapFile, StringMap* map);

		void _allocAndAddToStringMap(DWORD id, const char* dispString, StringMap& map,IDMap& idMap);

		void _assembleDisplayStringMap(const char* mapFile, int strLenLimit, StringMap* map, DWORD& maxID);

		void _deleteStringMapEntry(DWORD id, StringMap* map);

		void _assembleIDMaps(StringMap& strMap,IDMap& idMap);


	public:
		DisplayStringMapDPO();

		void getItemInfoIDs(const StringifiedItemInfo& stringifiedItemInfo, ItemInfo& itemInfoEntry);

		bool assembleDisplayStringMaps();

		void assembleIDMaps();

		void getDisplayStrings(ItemInfo& infoEntry, StringifiedItemInfo& stringifiedInfo);

		void deleteStringMapEntries(const ItemInfo& itemInfoEntry);

		bool syncUpdatesToFile();

		WORD getItemID(const char* item);

		WORD getTypeID(const char* type);

		WORD getProducerID(const char* producer);

		WORD getSpecID(const char* spec);
	};
}

#endif // DisplayStringMapDPO_h__
#pragma once
#ifndef ItemInfoDPO_h__
#define ItemInfoDPO_h__
#include "../Config/typedef.h"
#include "../DAL/ItemInfoDAO.h"
#include "../Entity/ItemInfo.h"
#include "../Algorithm/Algorithm.h"

using ItemInfoList = LinkedList<ItemInfo>;
using ItemUIDMap = HashMap<QWORD, void*>;

namespace DPL
{
	class ItemInfoDPO
	{
	private:
		static bool isLoaded;
		static bool isModified;
		static ItemInfoList itemInfoList;
		static ItemUIDMap itemUIDMap;
		DAL::ItemInfoDAO itemInfoDAO;
	public:

		ItemInfoDPO();

		bool assembleIntoList();

		void markModified();

		void updateItemInfo(ItemInfoList::Iterator& pos,ItemInfo& itemInfoEntry);

		void updateItemStatus(ItemInfoList::Iterator& pos);

		ItemInfoList::Iterator getEndIterator();

		ItemInfoList::Iterator getIterator();

		bool hasExisted(QWORD uid);

		bool syncUpdatesToFile();

		void addItemInfo(ItemInfo& itemInfoEntry);

	};

}
#endif // ItemInfoDPO_h__
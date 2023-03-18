#pragma once
#include "../Config/typedef.h"
#include "../Config/FileName.h"
#include "../Entity/SalesRecord.h"
#include "../Container/LinkedList.h"

using SalesRecordList = LinkedList<SalesRecord>;

namespace DAL
{
	class SalesRecordDAO
	{
	private:
		static literal char* salesRecordFileName = Config::FileName::salesRecord;
	
	public:
		bool update(SalesRecordList& recordList);
		bool load(SalesRecordList& recordList,DWORD &maxID);
		bool createFile();
	};
}



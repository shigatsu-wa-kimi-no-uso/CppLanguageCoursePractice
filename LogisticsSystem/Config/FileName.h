#pragma once
#include "typedef.h"

namespace Config
{
	struct FileName
	{
		static literal char* itemInfo = "iteminfo.dat";
		static literal char* salesRecord = "salesrec.dat";
		static literal char* purchaseRecord = "purchaserec.dat";
		static literal char* lastIDs = "lastids.dat";
		static literal char* batchInfo = "batchinfo.dat";
		struct StringMapFile
		{
			static literal char* item = "itemmap.dat";
			static literal char* type = "typemap.dat";
			static literal char* producer = "producermap.dat";
			static literal char* spec = "specmap.dat";
		};
	};
}
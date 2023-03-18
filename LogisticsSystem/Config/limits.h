#pragma once
#include "typedef.h"

namespace Config
{
	struct Limits
	{
		struct DisplayString 
		{
			static literal int ITEM = 24;
			static literal int TYPE = 16;
			static literal int PRODUCER = 24;
			static literal int SPEC = 28;
			static literal int STATUS = 64;
		};
	};
}

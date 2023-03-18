#pragma once
#ifndef EditBoxUI_h__
#define EditBoxUI_h__
#include "../Config/typedef.h"


namespace UI
{
	class EditBoxUI
	{
	public:
		static void pop(const char* message, char* buf,const DWORD bufSize);
	};

}

#endif // EditBoxUI_h__


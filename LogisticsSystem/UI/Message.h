#pragma once
#include "Callback.h"

struct Message {

	enum
	{
		EXIT_APPLICATION,
		SWITCH_INTERFACE,
		PAINT_INTERFACE,
		IGNORE
	}msg;

	Callback* callback;
	void* param1;
	void* param2;

	~Message() {
		delete callback;
	}
};


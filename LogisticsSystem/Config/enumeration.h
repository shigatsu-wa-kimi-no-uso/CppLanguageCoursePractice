#pragma once

//为方便, 不将其放入Config名字空间



enum class KeyEvent
{
	KEY_ESC = 27,
	KEY_UP = 72,
	KEY_DOWN = 80,
	KEY_LEFT = 75,
	KEY_RIGHT = 77,
	KEY_F1 = 59,
	KEY_F2 = 60,
	KEY_F3 = 61,
	KEY_F4 = 62,
	KEY_F5 = 63,
	KEY_F6 = 64,
	KEY_F7 = 65,
	KEY_F8 = 66,
	KEY_F9 = 67,
	KEY_F10 = 68,
	KEY_ENTER = 13
};


enum class ItemStatus
{
	SOLD_OUT,
	NEED_SUPPLEMENT,
	DELETED,
	NORMAL
};


#pragma once
#ifndef MessageBoxUI_h__
#define MessageBoxUI_h__


namespace UI
{
	class MessageBoxUI
	{
	public:
		static void pop(const char* message, int durationMillis);
	};

}

#endif // MessageBoxUI_h__
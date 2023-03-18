#pragma once


class Callback
{
public:
	virtual void operator()(void* param1) = 0;
	virtual ~Callback() {};
};

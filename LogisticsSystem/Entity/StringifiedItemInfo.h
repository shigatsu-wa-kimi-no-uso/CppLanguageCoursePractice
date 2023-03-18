#pragma once
#include "../Config/typedef.h"
#include "../Config/limits.h"


/*
* 基本商品信息
*/
struct StringifiedItemInfo
{
	char itemName[Config::Limits::DisplayString::ITEM];	//商品
	char typeName[Config::Limits::DisplayString::TYPE];	//商品类型
	char producer[Config::Limits::DisplayString::PRODUCER];//厂家
	char spec[Config::Limits::DisplayString::SPEC];    //规格
	char remainingCount[12];		//余量
	char status[Config::Limits::DisplayString::STATUS]; //状态
};

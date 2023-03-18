#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "StringifiedItemInfo.h"


/*
* 商品实体类
* 用于存储单个商品的字符串化显示
*/
struct StringifiedPurchaseRecord
{
	char purchasedAmount[12]; //这个批次的采购量
	char soldCount[12];	   //这个批次的销售量
	char remainingCount[12];	   //这个批次的销售量
	char timeOfPurchase[32]; //采购时间
	char cost[12];			   //采购成本
	char batchNumber[12];		//批次编号(只增不减)
};

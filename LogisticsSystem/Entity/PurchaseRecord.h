#pragma once
#ifndef Item_h__
#define Item_h__
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "ItemInfo.h"
#include <time.h>


struct PurchaseRecord
{
	QWORD itemUID;
	DWORD recordUID;
	DWORD batchID;		//批次编号(只增不减)
	DWORD purchasedAmount; //这次采购的采购量
	DWORD remainingCount;	   //这次采购后的商品余量
	time_t timeOfPurchase; //采购时间
	double unitCost;			   //单价
	ItemStatus status; //商品状态(售罄/已删除/需要补充)
};

#endif // Item_h__
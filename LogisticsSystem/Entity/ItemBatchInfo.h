#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"

struct ItemBatchInfo
{
	QWORD itemUID;		//商品UID
	DWORD  batchUID;	//批次编号
	DWORD purchasedAmount; //这个批次的采购量
	DWORD soldCount;	   //这个批次的销售量
	time_t timeOfPurchase; //采购时间
	double unitCost;			//采购单位成本
	double profit;			//这个批次的当前获利
	ItemStatus status; //这个批次的商品状态(售罄/已删除/需要补充)
};

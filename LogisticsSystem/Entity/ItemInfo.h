#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"

struct ItemInfo {
	union {
		struct {
			WORD itemID;	//商品ID 
			WORD typeID;	//商品类型ID
			WORD producerID;//厂家ID
			WORD specID;    //规格ID
		}properties;
		QWORD itemUID;
	}info;
	DWORD remainingCount;	//余量
	DWORD recentSoldCount; //最近一个月售出
	DWORD totalSoldCount;
	double totalCost;
	double totalSalesFigure;
	double netProfit;
	ItemStatus status;		//状态
#define itemID info.properties.itemID
#define typeID info.properties.typeID
#define producerID info.properties.producerID
#define specID info.properties.specID
};


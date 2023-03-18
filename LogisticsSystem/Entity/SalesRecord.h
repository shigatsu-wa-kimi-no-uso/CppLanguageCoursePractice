#pragma once
#include "../Config/typedef.h"
#include <time.h>


struct SalesRecord
{
	QWORD itemUID;
	DWORD recordUID;	//记录号
	DWORD batchID;		//批次编号(只增不减)
	DWORD soldCount;    //这次出售的出售量
	double unitPrice;	//这次出售的单价
	double netProfit;		//这次出售的净利润
	DWORD remainingCount; //这次出售后该商品还剩多少
	time_t recordTime;	//记录时间
};

#pragma once

struct StringifiedItemBatchInfo
{
	char batchUID[10];	//批次编号
	char timeOfPurchase[20]; //采购时间
	char purchasedAmount[10]; //这个批次的采购量
	char remainingCount[10]; //余量
	char soldCount[10];	   //这个批次的销售量
	char cost[10];			//采购成本
	char unitCost[10];		//采购单价
	char profit[10];			//这个批次的当前获利
	char netProfit[10];			//当前批次净利润
	char status[64]; //这个批次的商品状态(售罄/已删除/需要补充)
};
#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"

struct ItemInfo {
	union {
		struct {
			WORD itemID;	//��ƷID 
			WORD typeID;	//��Ʒ����ID
			WORD producerID;//����ID
			WORD specID;    //���ID
		}properties;
		QWORD itemUID;
	}info;
	DWORD remainingCount;	//����
	DWORD recentSoldCount; //���һ�����۳�
	DWORD totalSoldCount;
	double totalCost;
	double totalSalesFigure;
	double netProfit;
	ItemStatus status;		//״̬
#define itemID info.properties.itemID
#define typeID info.properties.typeID
#define producerID info.properties.producerID
#define specID info.properties.specID
};


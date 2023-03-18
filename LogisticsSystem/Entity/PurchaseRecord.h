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
	DWORD batchID;		//���α��(ֻ������)
	DWORD purchasedAmount; //��βɹ��Ĳɹ���
	DWORD remainingCount;	   //��βɹ������Ʒ����
	time_t timeOfPurchase; //�ɹ�ʱ��
	double unitCost;			   //����
	ItemStatus status; //��Ʒ״̬(����/��ɾ��/��Ҫ����)
};

#endif // Item_h__
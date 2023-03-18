#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"
#include "StringifiedItemInfo.h"


/*
* ��Ʒʵ����
* ���ڴ洢������Ʒ���ַ�������ʾ
*/
struct StringifiedPurchaseRecord
{
	char purchasedAmount[12]; //������εĲɹ���
	char soldCount[12];	   //������ε�������
	char remainingCount[12];	   //������ε�������
	char timeOfPurchase[32]; //�ɹ�ʱ��
	char cost[12];			   //�ɹ��ɱ�
	char batchNumber[12];		//���α��(ֻ������)
};

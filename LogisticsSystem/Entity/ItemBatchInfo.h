#pragma once
#include "../Config/typedef.h"
#include "../Config/enumeration.h"

struct ItemBatchInfo
{
	QWORD itemUID;		//��ƷUID
	DWORD  batchUID;	//���α��
	DWORD purchasedAmount; //������εĲɹ���
	DWORD soldCount;	   //������ε�������
	time_t timeOfPurchase; //�ɹ�ʱ��
	double unitCost;			//�ɹ���λ�ɱ�
	double profit;			//������εĵ�ǰ����
	ItemStatus status; //������ε���Ʒ״̬(����/��ɾ��/��Ҫ����)
};

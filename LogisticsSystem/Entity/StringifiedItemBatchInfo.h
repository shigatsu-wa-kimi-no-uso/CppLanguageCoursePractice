#pragma once

struct StringifiedItemBatchInfo
{
	char batchUID[10];	//���α��
	char timeOfPurchase[20]; //�ɹ�ʱ��
	char purchasedAmount[10]; //������εĲɹ���
	char remainingCount[10]; //����
	char soldCount[10];	   //������ε�������
	char cost[10];			//�ɹ��ɱ�
	char unitCost[10];		//�ɹ�����
	char profit[10];			//������εĵ�ǰ����
	char netProfit[10];			//��ǰ���ξ�����
	char status[64]; //������ε���Ʒ״̬(����/��ɾ��/��Ҫ����)
};
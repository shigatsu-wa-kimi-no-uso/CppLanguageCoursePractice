#pragma once
#include "../Config/typedef.h"
#include <time.h>


struct SalesRecord
{
	QWORD itemUID;
	DWORD recordUID;	//��¼��
	DWORD batchID;		//���α��(ֻ������)
	DWORD soldCount;    //��γ��۵ĳ�����
	double unitPrice;	//��γ��۵ĵ���
	double netProfit;		//��γ��۵ľ�����
	DWORD remainingCount; //��γ��ۺ����Ʒ��ʣ����
	time_t recordTime;	//��¼ʱ��
};

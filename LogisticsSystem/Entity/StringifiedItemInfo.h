#pragma once
#include "../Config/typedef.h"
#include "../Config/limits.h"


/*
* ������Ʒ��Ϣ
*/
struct StringifiedItemInfo
{
	char itemName[Config::Limits::DisplayString::ITEM];	//��Ʒ
	char typeName[Config::Limits::DisplayString::TYPE];	//��Ʒ����
	char producer[Config::Limits::DisplayString::PRODUCER];//����
	char spec[Config::Limits::DisplayString::SPEC];    //���
	char remainingCount[12];		//����
	char status[Config::Limits::DisplayString::STATUS]; //״̬
};

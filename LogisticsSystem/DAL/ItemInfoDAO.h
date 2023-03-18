#pragma once

#ifndef ItemInfoDAO_h__
#define ItemInfoDAO_h__
#include"../Entity/ItemInfo.h"
#include"../Config/FileName.h"
#include "../Container/LinkedList.h"
#include "../Container/HashMap.h"

namespace DAL
{
	/*
	* ��ȡ��Ʒ��Ϣ�����ݷ�����
	* �����ļ�: "iteminfo.dat"
	* ���ļ��洢һ������ΪEntity::ItemInfoEntry������
	*/
	class ItemInfoDAO
	{		
	private:
		static literal char* itemInfoFileName = Config::FileName::itemInfo;
	public:
		/*
		* �����Ʒ��Ϣ
		* �¼ӵ���ϢӦ����׷�����ļ�ĩβ, ��Ӧ��, ��ƷIDֻ��Ϊ��ǰ��ƷID���ֵ+1
		*/
		//bool addItemInfo(ItemInfoEntry& itemInfo);

		/*
		* ������Ʒ��Ϣ
		*/
		bool update(LinkedList<ItemInfo>& entries);

		/*
		* ������Ʒ��Ϣ
		* һ�ζ��������ļ����ڴ�
		* ����: ����ļ��ر��, ��ȡ��ܺ�ʱ, ��Ҫ����������ģʽ
		* itemCountΪ����
		*/
		ItemInfo* load(DWORD& itemCount);

		/*
		* �ͷ�load��new��buffer
		* ˭����˭�ͷ�ԭ��
		*/
		void unload(ItemInfo* pBuf);

		bool createFile();

		/*
		* ɾ����Ʒ��Ϣ
		* ɾ��ָ��ID����Ʒ��Ϣ, ��������: ������Ʒ״̬Ϊ��ɾ��״̬
		* ����: ɾ������Ʒ��Ϣ��Ȼռ���ſռ�, Ӧ����������
		*/
		/*bool deleteItemInfo(DWORD itemID);*/

	};
}

#endif // ItemInfoDAO_h__
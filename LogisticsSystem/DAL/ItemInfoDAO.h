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
	* 存取商品信息的数据访问类
	* 访问文件: "iteminfo.dat"
	* 该文件存储一个类型为Entity::ItemInfoEntry的数组
	*/
	class ItemInfoDAO
	{		
	private:
		static literal char* itemInfoFileName = Config::FileName::itemInfo;
	public:
		/*
		* 添加商品信息
		* 新加的信息应当是追加在文件末尾, 相应地, 商品ID只能为当前商品ID最大值+1
		*/
		//bool addItemInfo(ItemInfoEntry& itemInfo);

		/*
		* 更新商品信息
		*/
		bool update(LinkedList<ItemInfo>& entries);

		/*
		* 加载商品信息
		* 一次读完整个文件到内存
		* 问题: 如果文件特别大, 读取会很耗时, 需要做成懒加载模式
		* itemCount为出参
		*/
		ItemInfo* load(DWORD& itemCount);

		/*
		* 释放load中new的buffer
		* 谁分配谁释放原则
		*/
		void unload(ItemInfo* pBuf);

		bool createFile();

		/*
		* 删除商品信息
		* 删除指定ID的商品信息, 具体做法: 设置商品状态为已删除状态
		* 问题: 删除的商品信息仍然占据着空间, 应当定期整理
		*/
		/*bool deleteItemInfo(DWORD itemID);*/

	};
}

#endif // ItemInfoDAO_h__
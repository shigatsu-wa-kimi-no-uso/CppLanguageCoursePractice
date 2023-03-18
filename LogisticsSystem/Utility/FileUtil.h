#pragma once
#ifndef FileUtil_h__
#define FileUtil_h__
#include<stdio.h>
#include"../Config/typedef.h"


namespace Util
{
	enum FILEPOS
	{
		BEGIN = SEEK_SET,
		CURRENT = SEEK_CUR,
		END = SEEK_END
	};

	class FileUtil
	{
	private:
		FILE* hFile;
		long fileSize;

	public:

		FileUtil();

		~FileUtil();

		bool open(const char* filePath, const char* mode);

		bool write(const BYTE* buffer, const size_t byteCount);

		bool read(BYTE* buffer, const size_t byteCount);

		long getFileSize();

		/*
		* 在文件中查找字符串函数
		* 返回该字符串在文件中的起始位置
		* const long startPos: 约束查找的起始范围
		*/
		long findString(const char* targetStr, const long startPos);

		long getPosition();

		bool readAll(BYTE* buffer, const long startPos);

		bool setPosition(const long offset, const FILEPOS fromWhere);

		bool close();

		static bool openAndAppend(const char* filePath, const BYTE* buffer, const size_t byteCount);

		static bool openAndReadAll(const char* filePath, BYTE* buffer);

	};
}

#endif // FileUtil_h__
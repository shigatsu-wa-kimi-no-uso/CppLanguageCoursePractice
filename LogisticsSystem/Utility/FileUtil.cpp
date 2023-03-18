#include "FileUtil.h"
#include<assert.h>
#include<string.h>

namespace Util
{
	FileUtil::FileUtil() :hFile(NULL), fileSize(-1) {}

	FileUtil::~FileUtil() {
		if (fileSize != -1) {
			fclose(hFile);
		}
	}

	bool FileUtil::open(const char* filePath, const char* mode) {
		errno_t result = fopen_s(&hFile, filePath, mode);
		if (result == 0 &&
			fseek(hFile, 0, SEEK_END) == 0 &&
			(fileSize = ftell(hFile)) != -1 &&
			fseek(hFile, 0, SEEK_SET) == 0){
			return true;
		}
		return false;
	}

	bool FileUtil::write(const BYTE* buffer, const size_t byteCount) {
		assert(hFile != NULL);
		size_t result = fwrite(buffer, sizeof(BYTE), byteCount, hFile);
		if (result == byteCount) {
			return fflush(hFile) == 0;
		}
		return false;
	}



	bool FileUtil::read(BYTE* buffer, const size_t byteCount) {
		assert(hFile != NULL);
		size_t result = fread(buffer, sizeof(BYTE), byteCount, hFile);
		return result == byteCount;
	}

	long FileUtil::getFileSize() {
		return fileSize;
	}

	bool FileUtil::readAll(BYTE* buffer, const long startPos) {
		if (!setPosition(startPos, FILEPOS::BEGIN)) 
			return false;
		return read(buffer, fileSize);
	}



	long FileUtil::findString(const char* targetStr, const long startPos) {
		long bufferSize = fileSize - startPos;
		BYTE* buffer = new BYTE[bufferSize];
		bool result;
		result = readAll(buffer, startPos);
		if (!result) return -1;
		size_t targetStrLen = strlen(targetStr);
		long matchCount = 0;
		for (int i = 0; i < bufferSize; ++i) {
			if (buffer[i] == targetStr[matchCount]) {
				++matchCount;
				if (matchCount == targetStrLen) return startPos + i - targetStrLen;
			} else {
				matchCount = 0;
			}
		}
		return -1;
	}

	long FileUtil::getPosition() {
		return ftell(hFile);
	}

	bool FileUtil::setPosition(const long offset, const FILEPOS fromWhere) {
		assert(hFile != NULL);
		return fseek(hFile, offset, fromWhere) == 0;
	}

	bool FileUtil::close() {
		assert(hFile != NULL);
		if (fclose(hFile) == 0) {
			fileSize = -1;
			return true;
		}
		return false;
	}




	bool FileUtil::openAndAppend(const char* filePath, const BYTE* buffer, const size_t byteCount) {
		FILE* hFile;
		errno_t result = fopen_s(&hFile, filePath, "ab+");
		if (result == 0) {
			size_t result2 = fwrite(buffer, sizeof(BYTE), byteCount, hFile);
			if (result2 == byteCount && fflush(hFile) == 0) {
				return fclose(hFile) == 0;
			}
		}
		return false;
	}

	bool FileUtil::openAndReadAll(const char* filePath, BYTE* buffer) {
		FILE* hFile;
		errno_t result = fopen_s(&hFile, filePath, "rb");
		if (result == 0) {
			size_t fileSize;
			if (fseek(hFile, 0, SEEK_END) != 0 &&
				(fileSize = ftell(hFile)) != -1 && fseek(hFile, 0, SEEK_SET)) {
				size_t result = fread(buffer, sizeof(BYTE), fileSize, hFile);
				if (result == fileSize && fflush(hFile) == 0) {
					return fclose(hFile) == 0;
				}
			}
		}
		return false;
	}
}
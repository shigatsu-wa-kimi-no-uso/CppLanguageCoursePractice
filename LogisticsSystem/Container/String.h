#pragma once
#include "Iterable.h"
#include <string.h>


/*
* 静态字符串类
* 不支持动态修改字符串长度,仅作为字符串指针+字符串长度变量的一个包装类
*/
class String
{
private:
	char* szBuf;
	size_t len;
public:
	String():szBuf(nullptr),len(0){}

	String(const String& val) :len(val.len) {
		szBuf = new char[len + 1];
		strcpy_s(szBuf, len + 1, val.szBuf);
	}

	String(const char* val) {
		len = strlen(val);
		szBuf = new char[len + 1];
		strcpy_s(szBuf, len + 1, val);
	}

	/*
	* 移动构造函数
	*/
	String(char* &&val):szBuf(val) {
		len = strlen(val);
		val = nullptr;
	}


	String(String&& val)  noexcept
		: len(val.len),szBuf(val.szBuf) {
		val.szBuf = nullptr;
	}

	size_t length() const {
		return len;
	}

	char& operator[](size_t pos) const {
		return szBuf[pos];
	}

	void operator=(const char* val) {
		len = strlen(val);
		szBuf = new char[len + 1];
		strcpy_s(szBuf, len + 1, val);
	}

	/*
	* 拷贝赋值
	*/
	void operator=(const String& val) {
		len = val.len;
		szBuf = new char[len + 1];
		strcpy_s(szBuf, len + 1, val.szBuf);
	}

	/*
	* 移动赋值
	*/
	void operator=(String&& val) noexcept {
		len = val.len;
		szBuf = val.szBuf;
		val.szBuf = nullptr;
	}

	void operator=(char* &&val) {
		len = strlen(val);
		szBuf = val;
		val = nullptr;
	}

	bool operator==(const char* rgt) const {
		return !strcmp(szBuf, rgt);
	}

	bool operator==(const String& rgt) const {
		return len == rgt.len && !strcmp(szBuf, rgt.szBuf);
	}


	bool operator!=(const char* rgt) const {
		return operator==(rgt);
	}

	const char* operator()() const {
		return szBuf;
	}

	void free() {
		delete[] szBuf;
		szBuf = nullptr;
		len = 0;
	}

	~String() {
		free();
	}
};


#pragma once
/*
* logger.h
* 日志系统(头文件)
* 用于便捷地打印程序的日志信息
*/

#ifndef Logger_h__
#define Logger_h__
#include<stdio.h>
#include<stdarg.h>
#include<time.h>

/*
* 日志类型
* 分为调试信息(DEBUG),普通信息(INFO),警告信息(WARNING),错误信息(ERROR)四种类型
* ENUMCOUNT为方便获取枚举类的数量而添加
* 每次打印日志时, 需要指定日志类型
* 同时, 程序还需要指定日志等级(DEBUG INFO WARNING ERROR四个等级)
* 指定日志等级后,将只打印当前等级及后续等级的日志
* 例如, 若定义日志等级为WARNING, 则只有WARNING和ERROR类型的日志会被打印
*/


//名字空间限制, 防止命名污染
namespace Logger
{
	enum class LOGTYPE
	{
		DEBUG, INFO, WARNING, ERROR, ENUMCOUNT
	};

	constexpr LOGTYPE DEBUG = LOGTYPE::DEBUG;
	constexpr LOGTYPE INFO = LOGTYPE::INFO;
	constexpr LOGTYPE WARNING = LOGTYPE::WARNING;
	constexpr LOGTYPE ERROR = LOGTYPE::ERROR;

//开启日志系统(如果不想打开则删除下面的define语句)
#define LOGGER_ON

//设置日志等级
#define LOGLEVEL WARNING

	//日志系统初始化(打开文件等)
	void logger_init();

	//日志系统退出函数, 用于日志的收尾工作(如关闭文件等), 应当在程序退出前调用
	void logger_exit();

	//日志系统开关检测
	//下面为针对当日志系统打开时(LOGGER_ON被定义时)的一些预处理逻辑
#ifdef LOGGER_ON
	//检测日志等级是否已经指定

#ifndef LOGLEVEL
#error LOGLEVEL undefined.
#endif // LOGLEVEL


//获取当前时间的函数
	static char* getTimeString(time_t timestamp);

	//打印日志的函数
	void printLog(LOGTYPE type, const char* format, ...);


#define info(format,...) Logger::printLog(Logger::LOGTYPE::INFO,"%-60s| "##format,__FUNCTION__,__VA_ARGS__)
#define error(format,...) Logger::printLog(Logger::LOGTYPE::ERROR,"%-60s| "##format,__FUNCTION__,__VA_ARGS__)
#define warning(format,...) Logger::printLog(Logger::LOGTYPE::WARNING,"%-60s| "##format,__FUNCTION__,__VA_ARGS__)
#define debug(format,...) Logger::printLog(Logger::LOGTYPE::DEBUG,"%-60s| "##format,__FUNCTION__,__VA_ARGS__)
#define trace Logger::printLog(Logger::LOGTYPE::DEBUG, "\nIn file %s\nLine: %d\nFunction:%s\n", __FILE__, __LINE__, __FUNCTION__)
#else // LOGGER_ON
//日志系统关闭时, 定义下面的宏为((void)0), 将有关打印日志的代码变为空代码, 防止影响代码编译, 而不是直接不定义相关的打印日志宏
#define info(format,...) ((void)0)
#define error(format,...) ((void)0)
#define warning(format,...) ((void)0)
#define debug(format,...) ((void)0)
#define trace ((void)0)

#endif // LOGGER_ON
}
#endif // Logger_h__
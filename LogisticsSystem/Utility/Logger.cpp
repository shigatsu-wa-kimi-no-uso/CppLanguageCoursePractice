#include "Logger.h"

namespace Logger
{
#ifdef LOGGER_ON
	FILE* hLogFile;
	void logger_init() {
	#ifndef LOGGER_OUTPUT_FILE
		hLogFile = stdout;
	#else
		hLogFile = fopen((LOGGER_OUTPUT_FILE), "a+");
	#endif
	}

	static char* getTimeString(time_t timestamp) {
		static tm time;
		localtime_s(&time, &timestamp);
		static char timeStrBuf[64];
		sprintf_s(timeStrBuf, "%d-%02d-%02d %02d:%02d:%02d",
			1900 + time.tm_year,
			1 + time.tm_mon,
			time.tm_mday,
			time.tm_hour,
			time.tm_min,
			time.tm_sec);
		return timeStrBuf;
	}

	void printLog(LOGTYPE type, const char* format, ...) {
		//日志等级限制, 小于指定等级的不打印
		if (type < LOGLEVEL) return;
		static unsigned int counter = 0;
		static char logBuffer[4096];
		static const char* typestr[(int)LOGTYPE::ENUMCOUNT] = { "DEBUG","INFO","WARNING","ERROR" };
		va_list arglist;
		va_start(arglist, format);
		vsprintf_s(logBuffer, format, arglist);
		fprintf(hLogFile,
			"%4d %-8s %s %s\n",
			counter++,
			typestr[(int)type],
			getTimeString(time(0)),
			logBuffer);
		fflush(hLogFile);
		va_end(arglist);
	}

	void logger_exit() {
		fprintf(hLogFile, "----AppExit:%s----\n\n\n", getTimeString(time(0)));
		fflush(hLogFile);
		fclose(hLogFile);
	}
#else // LOGGER_ON
	void logger_init() {}
	void logger_exit() {}
#endif // LOGGER_ON
}
#pragma once
#ifndef typedef_h__
#define typedef_h__

//macros
#define literal constexpr const
#define EXPAND(x) x
#define STRINGIFY(x) #x
#define CONCAT(a,b) a##b

#define COLUMN_CHARCOUNT 124
#define LINE_CHARCOUNT 30

#define NEEDSUPPLEMENT_THRESHOLD 0.15f

//typedefs

using BYTE = unsigned char;
using WORD = unsigned short;
using DWORD = unsigned long;
using QWORD = unsigned long long;
using HANDLE = void*;
using time_t = long long;

#endif // typedef_h__
#pragma once

#ifdef _UNICODE
#ifndef UNICODE
#define UNICODE 1
#endif
#endif

//#include "../resource.h"
//moved that definition into wing.h

typedef signed char        i1;
typedef signed short       i2;
typedef signed int         i4;
typedef signed long long   i8;
typedef unsigned char      u1;
typedef unsigned short     u2;
typedef unsigned int       u4;
typedef unsigned long long u8;

typedef float      f4;
typedef double     f8;

typedef i4 errorCode;

//#define null nullptr
#define null 0

typedef wchar_t wchar;

#ifdef UNICODE
typedef wchar_t tchar;
#define t(quote) L##quote
#else
typedef char tchar;
#define t(quote) quote
#endif

#include "wincore.h"

//#ifdef __DMC__
//typedef u4 addr;
//#else
//typedef uintptr_t addr;
//#endif

//typedef u1* u1p;
//typedef u2* u2p;
//typedef u4* u4p;
//typedef u8* u8p;
//typedef i1* i1p;
//typedef i2* i2p;
//typedef i4* i4p;
//typedef i8* i8p;

typedef u1* bytep;
typedef u1 byte;
typedef void* voidp;

namespace util {
//Don't use it, use Error(code) instead;
void ErrorAlert(u2 code);
}

#if _DEBUG
#define Error(code) \
{\
	util::ErrorAlert(code);\
	__debugbreak();\
	wincore::ExitProcess(code);\
}
#else
#define Error(code) \
{\
	util::ErrorAlert(code);\
	wincore::ExitProcess(code);\
}
#endif

#if _DEBUG
#define ErrorLazy() \
{\
	util::ErrorAlert(0x1);\
	__debugbreak();\
	wincore::ExitProcess(0x1);\
}
#else
#define ErrorLazy() \
{\
	util::ErrorAlert(0x1);\
	wincore::ExitProcess(0x1);\
}
#endif

namespace util {
	u2 u2low(u4 in);
	u2 u2high(u4 in);
	i2 i2low(u4 in);
	i2 i2high(u4 in);
	void u1memzero(bytep buf, u4 size);
	void Alert(const wchar* message);
}

namespace ustr {
	u4 tcharLen(const tchar* in);
	u4 charLen(const char* in);
	u4 wcharLen(const wchar* in);
	void wcharCopy(const wchar* from, wchar* to);
	void wcharCopyMove(wchar const* from, wchar*& to);
	errorCode wcharAttachSafe(const wchar* from, wchar* to, u4 toCap);
	wchar* NumberuToWstringSafe(u4 the_int, wchar_t* buf, u4 buflen, u4* outStrlen);
}

//typedef unsigned int size_t;
//void* __cdecl memset(void* dest, int c, size_t count);
#include "miniutil.h"

//make sure that code is never 0
//paranoia
void util::ErrorAlert(u2 code) {
	tchar buf[5];
	tchar* cur = buf + 5;
	*cur-- = 0;
	do {
		tchar rem = code % 16;
		code = code / 16;
		if (rem < 10) {
			*cur = '0'+rem;
		} else {
			rem -= 10;
			*cur = 'A'+rem;
		}
		cur--;
	} while (code);
	cur++;
	wincore::MessageBoxW(0, cur, t("Error"), wincore::MB_ICONERROR);
}

void util::Alert(const wchar* message) {
	wincore::MessageBoxW(0, message, L"Alert", 0);
}

u2 util::u2low(u4 in) {
	return u2(in);
}

u2 util::u2high(u4 in) {
	return u2(in >> 16);
}

i2 util::i2low(u4 in) {
	return i2(in);
}

i2 util::i2high(u4 in) {
	return u2(in >> 16);
}

void util::u1memzero(bytep buf, u4 size) {
	bytep bytes = buf;
	while (size--)
	{
		*bytes++ = 0;
	}
}

u4 ustr::tcharLen(const tchar* in) {
	tchar* cur = (tchar*)in;
	while(*cur != 0) cur++;
	return cur - in;
}

extern "C" {

	void*  __cdecl memset(void* dst, i4 value, u4 size);
	#pragma function(memset)
	void* memset(void* dest, int c, size_t count)
	{
		char* bytes = (char*)dest;
		while (count--)
		{
			*bytes++ = (char)c;
		}
		return dest;
	}

	void*  __cdecl memcpy(void* dst, const void * src, u4 size);
	#pragma function(memcpy)
	void* memcpy(void* dest, const void* src, size_t count)
	{
		char* dest8 = (char*)dest;
		const char* src8 = (const char*)src;
		while (count--)
		{
			*dest8++ = *src8++;
		}
		return dest;
	}

//#ifndef __DMC__
//	void*  __cdecl memmove(void* dst, const void * src, u4 size);
//	#pragma function(memmove)
//#endif
//    void* memmove(void* dest, const void* src, size_t size) {
//        //Despite being specified "as if" a temporary buffer is used, actual implementations of this function 
//        //do not incur the overhead or double copying or extra memory. A common approach (glibc and bsd libc) 
//        //is to copy bytes forwards from the beginning of the buffer if the destination starts before the source, 
//        //and backwards from the end otherwise, with a fall back to the more efficient memcpy when there is no overlap at all. 
//
//        //LATER: test if it works at all
//        int diff = (int)dest - (int)src;
//        if (diff < 0) diff = -diff;
//        if (diff > size) return memcpy(dest, src, size);
//        if (dest < src) {
//            return memcpy(dest, src, size);
//        }
//        else {
//            char* dest8 = (char*)dest + size - 1; 
//            const char* src8 = (const char*)src + size - 1;
//            while (size--)
//            {
//                *dest8-- = *src8--;
//            }
//            return dest;
//        }
//    }
}

u4 ustr::charLen(const char* in) {
	const char* cur = in;
	while(*cur != 0) cur++;
	return cur - in;
}

u4 ustr::wcharLen(const wchar* in) {
	const wchar* cur = in;
	while(*cur != 0) cur++;
	return cur - in;
}

void ustr::wcharCopy(const wchar* from, wchar* to) {
	const wchar* curfrom = from;
	wchar* curto = to;
	do *curto++ = *curfrom++; while (*curfrom);
	*curto = 0;
}

void ustr::wcharCopyMove(wchar const* from, wchar*& to) {
	do *to++ = *from++; while (*from);
	*to = 0;
}

errorCode ustr::wcharAttachSafe(const wchar* from, wchar* to, u4 toCap) {
	u4 toLen = ustr::wcharLen(to);
	if (ustr::wcharLen(from) + toLen >= toCap) return 1;
	const wchar* curfrom = from;
	wchar* curto = to + toLen;
	do *curto++ = *curfrom++; while (*curfrom);
	*curto = 0;
	return 0;
}

wchar* ustr::NumberuToWstringSafe(u4 the_int, wchar_t* buf, u4 buflen, u4* outStrlen) {
	//if (the_int > 9999999999)
	//TODO: buflen unused
	wchar_t* cur = buf+buflen-1;
	wchar_t* curstart = cur;
	*cur=0;
	cur--;
	if (the_int == 0) {
		*cur = L'0';
		*outStrlen = 1;
		return cur;
	}
	int reminder;
	do {
		reminder = the_int % 10;
		the_int = the_int / 10;

		*cur = L'0' + reminder;
		cur--;
	} while (the_int);
	cur++;
	*outStrlen = curstart - cur;

	return cur;
}

//extern "C" {
//	//#pragma function(ZeroMemory)
//	//void* ZeroMemory(void* dest, size_t count) {
//	//	char* bytes = (char*)dest;
//	//	while (count--)
//	//	{
//	//		*bytes++ = 0;
//	//	}
//	//	return dest;
//	//}
//
//	#pragma function(memset)
//	void* memset(void* dest, int c, size_t count)
//	{
//		char* bytes = (char*)dest;
//		while (count--)
//		{
//			*bytes++ = (char)c;
//		}
//		return dest;
//	}
//
//	#pragma function(memcpy)
//	void* memcpy(void* dest, const void* src, size_t count)
//	{
//		char* dest8 = (char*)dest;
//		const char* src8 = (const char*)src;
//		while (count--)
//		{
//			*dest8++ = *src8++;
//		}
//		return dest;
//	}
//
//#ifndef __DMC__
//#pragma function(memmove)
//#endif
//    void* memmove(void* dest, const void* src, size_t size) {
//        //Despite being specified "as if" a temporary buffer is used, actual implementations of this function 
//        //do not incur the overhead or double copying or extra memory. A common approach (glibc and bsd libc) 
//        //is to copy bytes forwards from the beginning of the buffer if the destination starts before the source, 
//        //and backwards from the end otherwise, with a fall back to the more efficient memcpy when there is no overlap at all. 
//
//        //LATER: test if it works at all
//        int diff = (int)dest - (int)src;
//        if (diff < 0) diff = -diff;
//        if (diff > size) return memcpy(dest, src, size);
//        if (dest < src) {
//            return memcpy(dest, src, size);
//        }
//        else {
//            char* dest8 = (char*)dest + size - 1; 
//            const char* src8 = (const char*)src + size - 1;
//            while (size--)
//            {
//                *dest8-- = *src8--;
//            }
//            return dest;
//        }
//    }
//}
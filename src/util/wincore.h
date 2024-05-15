#pragma once

#include "miniutil.h"

namespace wincore {
extern "C" {

typedef u4 HANDLE;
//^ TODO check

void
__stdcall
ExitProcess(
	u4 uExitCode
	);

const u4 MB_ICONHAND  = 0x00000010;
const u4 MB_ICONERROR = MB_ICONHAND;

u4
__stdcall
MessageBoxA(
    HANDLE hWnd,
    const char* pText,
    const char* pCaption,
    u4 uType);

u4
__stdcall
MessageBoxW(
    HANDLE hWnd,
    const wchar* pText,
    const wchar* pCaption,
    u4 uType);

} //extern "C" {
} //namespace wincore {
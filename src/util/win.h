#pragma once

//This file is the alternative to
//#define WIN32_LEAN_AND_MEAN
//#define NOSERVICE
//#define NOMCX
//#include <windows.h>

//I made it to not pollute global namespace,
//and to reduce the number of intellisense suggestions

//wing.h is for gui.

#include "miniutil.h"

namespace win {
extern "C" {

typedef u4 HANDLE;

u4
__stdcall
CloseHandle(
	HANDLE hObject
	);

const u4 INVALID_HANDLE_VALUE = -1;

namespace file {

const u4 MAX_PATH = 260;

const u4 GENERIC_READ    = 0x80000000;
const u4 GENERIC_WRITE   = 0x40000000;
const u4 GENERIC_EXECUTE = 0x20000000;
const u4 GENERIC_ALL     = 0x10000000;

const u4 CREATE_NEW        = 1;
const u4 CREATE_ALWAYS     = 2;
const u4 OPEN_EXISTING     = 3;
const u4 OPEN_ALWAYS       = 4;
const u4 TRUNCATE_EXISTING = 5;

const u4 FILE_SHARE_READ   = 0x1;
const u4 FILE_SHARE_WRITE  = 0x2;
const u4 FILE_SHARE_DELETE = 0x4;

typedef struct tagSECURITY_ATTRIBUTES {
	u4 nLength;
	void* pSecurityDescriptor;
	u4 bInheritHandle;
} SECURITY_ATTRIBUTES;

HANDLE
__stdcall
CreateFileA(
	const char* lpFileName,
	u4 dwDesiredAccess,
	u4 dwShareMode,
	SECURITY_ATTRIBUTES* pSecurityAttributes,
	u4 dwCreationDisposition,
	u4 dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);

HANDLE
__stdcall
CreateFileW(
	const wchar* lpFileName,
	u4 dwDesiredAccess,
	u4 dwShareMode,
	SECURITY_ATTRIBUTES* pSecurityAttributes,
	u4 dwCreationDisposition,
	u4 dwFlagsAndAttributes,
	HANDLE hTemplateFile
	);

//#ifdef UNICODE
//#define CreateFile  CreateFileW
//#else
//#define CreateFile  CreateFileA
//#endif

u4
__stdcall
WriteFile(
	HANDLE hFile,
	bytep pBuffer,
	u4 nNumberOfBytesToWrite,
	u4* pNumberOfBytesWritten,
	u4 pOverlapped
	);

u4
__stdcall
ReadFile(
	HANDLE hFile,
	bytep pBuffer,
	u4 nNumberOfBytesToRead,
	u4* pNumberOfBytesRead,
	u4 pOverlapped
	);

u4
__stdcall
SetEndOfFile(
	HANDLE hFile
	);

const u4 FILE_BEGIN   = 0;
const u4 FILE_CURRENT = 1;
const u4 FILE_END     = 2;

u4
__stdcall
SetFilePointer(
	HANDLE hFile,
	u4 lDistanceToMove,
	u4* lpDistanceToMoveHigh,
	u4 dwMoveMethod
	);

typedef struct tagFILETIME {
	u4 dwLowDateTime;
	u4 dwHighDateTime;
} FILETIME;

typedef struct tagWIN32_FIND_DATAW {
	u4 dwFileAttributes;
	FILETIME ftCreationTime;
	FILETIME ftLastAccessTime;
	FILETIME ftLastWriteTime;
	u4 nFileSizeHigh;
	u4 nFileSizeLow;
	u4 dwReserved0;
	u4 dwReserved1;
	wchar cFileName[ MAX_PATH ];
	wchar cAlternateFileName[ 14 ];
} WIN32_FIND_DATAW;

const u4 FILE_ATTRIBUTE_DIRECTORY = 0x00000010;

HANDLE
__stdcall
FindFirstFileW(
	const wchar* pFileName,
	WIN32_FIND_DATAW* pFindFileData
	);

u4
__stdcall
FindNextFileW(
	HANDLE hFindFile,
	WIN32_FIND_DATAW* pFindFileData
	);

u4
__stdcall
FindClose(
	HANDLE hFindFile
	);

u4
__stdcall
MoveFileW(
	const wchar* pExistingFileName,
	const wchar* pNewFileName
	);

u4
__stdcall
GetFileTime(
	HANDLE hFile,
	FILETIME* pCreationTime,
	FILETIME* pLastAccessTime,
	FILETIME* pLastWriteTime
	);

u4
__stdcall
SetFileTime(
	HANDLE hFile,
	FILETIME* pCreationTime,
	FILETIME* pLastAccessTime,
	FILETIME* pLastWriteTime
	);

HANDLE
__stdcall
FindFirstChangeNotificationW(
    const wchar* pPathName,
    bool bWatchSubtree,
    u4 dwNotifyFilter
    );

const u4 FILE_NOTIFY_CHANGE_FILE_NAME    = 0x00000001;
const u4 FILE_NOTIFY_CHANGE_DIR_NAME     = 0x00000002;
const u4 FILE_NOTIFY_CHANGE_ATTRIBUTES   = 0x00000004;
const u4 FILE_NOTIFY_CHANGE_SIZE         = 0x00000008;
const u4 FILE_NOTIFY_CHANGE_LAST_WRITE   = 0x00000010;
const u4 FILE_NOTIFY_CHANGE_LAST_ACCESS  = 0x00000020;
const u4 FILE_NOTIFY_CHANGE_CREATION     = 0x00000040;
const u4 FILE_NOTIFY_CHANGE_SECURITY     = 0x00000100;

const u4 FILE_ACTION_ADDED                   = 0x00000001;
const u4 FILE_ACTION_REMOVED                 = 0x00000002;
const u4 FILE_ACTION_MODIFIED                = 0x00000003;
const u4 FILE_ACTION_RENAMED_OLD_NAME        = 0x00000004;
const u4 FILE_ACTION_RENAMED_NEW_NAME        = 0x00000005;

u4
__stdcall
FindNextChangeNotification(
    HANDLE hChangeHandle
    );

u4
__stdcall
FindCloseChangeNotification(
    HANDLE hChangeHandle
    );

//BY_HANDLE_FILE_INFORMATION fileInformation;
//GetFileInformationByHandle(file, &fileInformation);
//u4 filesize = fileInformation.nFileSizeLow;

} //namespace file

namespace clipboard {

const u4 CF_TEXT = 1;
const u4 CF_UNICODETEXT = 13;

u4
__stdcall
IsClipboardFormatAvailable(
	u4 format);

u4
__stdcall
OpenClipboard(
	HANDLE hWndNewOwner);


HANDLE
__stdcall
GetClipboardData(
	u4 uFormat);

//those are technically heap functions...
void*
__stdcall
GlobalLock ( 
	HANDLE hMem
	);

u4
__stdcall
GlobalUnlock(
	HANDLE hMem
	);

u4
__stdcall
CloseClipboard();
} // namespace clipboard


namespace memory {
const u4 PAGE_NOACCESS  = 0x01;
const u4 PAGE_READONLY  = 0x02;
const u4 PAGE_READWRITE = 0x04;
const u4 MEM_COMMIT  = 0x1000;
const u4 MEM_RESERVE = 0x2000;

bytep
__stdcall
VirtualAlloc(
	bytep pAddress,
	u4 dwSize,
	u4 flAllocationType,
	u4 flProtect
	);
} //namespace memory 


//MultiByte flags
const u4 MB_ERR_INVALID_CHARS = 0x08;

//WideChar flags
const u4 WC_ERR_INVALID_CHARS = 0x80;
//^ This one is only available since Vista.
//^ Use this only with UTF8
const u4 WC_NO_BEST_FIT_CHARS = 0x400;
//^ Slightly older value, WINVER >= 0x0500
//^ Use this with everything else.

//Code Page
const u4 CP_UTF7 = 65000;
const u4 CP_UTF8 = 65001;

int
__stdcall
MultiByteToWideChar(
    u4       CodePage,
    u4       dwFlags,
    const char*  pMultiByteStr,
    u4          cbMultiByte,
    const wchar*   pWideCharStr, //out
    int      cchWideChar); //out

int
__stdcall
WideCharToMultiByte(
    u4     CodePage,
    u4    dwFlags,
    const wchar*  pWideCharStr,
    u4      cchWideChar,
    const char*   pMultiByteStr,
    int      cbMultiByte,
    const char*   pDefaultChar,
    u4*  pUsedDefaultChar);

u4
__stdcall
GetTickCount();

const u4 INFINITE            = 0xFFFFFFFF;  // Infinite timeout

u4
__stdcall
MsgWaitForMultipleObjects(
    u4 nCount,
    const HANDLE *pHandles,
    bool fWaitAll,
    u4 dwMilliseconds,
    u4 dwWakeMask);

//Queue status flags for GetQueueStatus() and MsgWaitForMultipleObjects()
const u4 QS_KEY              = 0x0001;
const u4 QS_MOUSEMOVE        = 0x0002;
const u4 QS_MOUSEBUTTON      = 0x0004;
const u4 QS_POSTMESSAGE      = 0x0008;
const u4 QS_TIMER            = 0x0010;
const u4 QS_PAINT            = 0x0020;
const u4 QS_SENDMESSAGE      = 0x0040;
const u4 QS_HOTKEY           = 0x0080;
const u4 QS_ALLPOSTMESSAGE   = 0x0100;

const u4 QS_MOUSE         = QS_MOUSEMOVE     |
                            QS_MOUSEBUTTON;

const u4 QS_INPUT          = QS_MOUSE|
                             QS_KEY;

const u4 QS_ALLINPUT      = QS_INPUT         |
                            QS_POSTMESSAGE   |
                            QS_TIMER         |
                            QS_PAINT         |
                            QS_HOTKEY        |
                            QS_SENDMESSAGE;

const u4 WAIT_OBJECT_0       = 0;

void
__stdcall
Sleep(
    u4 dwMilliseconds
    );

namespace reg {

typedef wincore::HANDLE HKEY;

// Reserved Key Handles.

const HKEY HKEY_CLASSES_ROOT           = HKEY(0x80000000);
const HKEY HKEY_CURRENT_USER           = HKEY(0x80000001);
const HKEY HKEY_LOCAL_MACHINE          = HKEY(0x80000002);
const HKEY HKEY_USERS                  = HKEY(0x80000003);
const HKEY HKEY_PERFORMANCE_DATA       = HKEY(0x80000004);
const HKEY HKEY_PERFORMANCE_TEXT       = HKEY(0x80000050);
const HKEY HKEY_PERFORMANCE_NLSTEXT    = HKEY(0x80000060);

// RRF - Registry Routine Flags (for RegGetValue)
const u4 RRF_RT_REG_NONE        = 0x00000001;
const u4 RRF_RT_REG_SZ          = 0x00000002;
const u4 RRF_RT_REG_EXPAND_SZ   = 0x00000004;
const u4 RRF_RT_REG_BINARY      = 0x00000008;
const u4 RRF_RT_REG_DWORD       = 0x00000010;
const u4 RRF_RT_REG_MULTI_SZ    = 0x00000020;
const u4 RRF_RT_REG_QWORD       = 0x00000040;

const u4 RRF_RT_DWORD           = (RRF_RT_REG_BINARY | RRF_RT_REG_DWORD);
const u4 RRF_RT_QWORD           = (RRF_RT_REG_BINARY | RRF_RT_REG_QWORD);
const u4 RRF_RT_ANY             = 0x0000ffff;

const u4 RRF_NOEXPAND           = 0x10000000;
const u4 RRF_ZEROONFAILURE      = 0x20000000;

u4
__stdcall
RegGetValueW (
    HKEY          hkey,
    const wchar*  pSubKey,
    const wchar*  pValue,
    u4            dwFlags,
    u4*           pdwType,
    void*         pvData,
    u4*           pcbData 
    );

// Open/Create Options

const u4 REG_OPTION_RESERVED         = 0x00000000;
const u4 REG_OPTION_NON_VOLATILE     = 0x00000000;
const u4 REG_OPTION_VOLATILE         = 0x00000001;
const u4 REG_OPTION_CREATE_LINK      = 0x00000002;
const u4 REG_OPTION_BACKUP_RESTORE   = 0x00000004;
const u4 REG_OPTION_OPEN_LINK        = 0x00000008;
const u4 REG_LEGAL_OPTION            =
                (REG_OPTION_RESERVED            |
                 REG_OPTION_NON_VOLATILE        |
                 REG_OPTION_VOLATILE            |
                 REG_OPTION_CREATE_LINK         |
                 REG_OPTION_BACKUP_RESTORE      |
                 REG_OPTION_OPEN_LINK);

// Registry Specific Access Rights.

const u4 KEY_QUERY_VALUE         = 0x0001;
const u4 KEY_SET_VALUE           = 0x0002;
const u4 KEY_CREATE_SUB_KEY      = 0x0004;
const u4 KEY_ENUMERATE_SUB_KEYS  = 0x0008;
const u4 KEY_NOTIFY              = 0x0010;
const u4 KEY_CREATE_LINK         = 0x0020;
const u4 KEY_WOW64_32KEY         = 0x0200;
const u4 KEY_WOW64_64KEY         = 0x0100;
const u4 KEY_WOW64_RES           = 0x0300;

const u4 READ_CONTROL    = 0x00020000;
const u4 SYNCHRONIZE     = 0x00100000;

const u4 STANDARD_RIGHTS_READ             = READ_CONTROL;
const u4 STANDARD_RIGHTS_WRITE            = READ_CONTROL;
const u4 STANDARD_RIGHTS_EXECUTE          = READ_CONTROL;
const u4 STANDARD_RIGHTS_ALL              = 0x001F0000;

const u4 KEY_READ             = ((STANDARD_RIGHTS_READ       |
                                  KEY_QUERY_VALUE            |
                                  KEY_ENUMERATE_SUB_KEYS     |
                                  KEY_NOTIFY)                 
                                  &                           
                                 (~SYNCHRONIZE));


const u4 KEY_WRITE            = ((STANDARD_RIGHTS_WRITE      |
                                  KEY_SET_VALUE              |
                                  KEY_CREATE_SUB_KEY)         
                                  &                           
                                 (~SYNCHRONIZE));

const u4 KEY_EXECUTE          = ((KEY_READ)                   
                                  &                           
                                 (~SYNCHRONIZE));

const u4 KEY_ALL_ACCESS       = ((STANDARD_RIGHTS_ALL        |
                                  KEY_QUERY_VALUE            |
                                  KEY_SET_VALUE              |
                                  KEY_CREATE_SUB_KEY         |
                                  KEY_ENUMERATE_SUB_KEYS     |
                                  KEY_NOTIFY                 |
                                  KEY_CREATE_LINK)            
                                  &                           
                                 (~SYNCHRONIZE));


typedef u4 REGSAM;

u4
__stdcall
RegCreateKeyExW (
    HKEY hKey,
    const wchar* pSubKey,
    u4 Reserved,
    const wchar* pClass,
    u4 dwOptions,
    REGSAM samDesired,
    void* pSecurityAttributes, //LPSECURITY_ATTRIBUTES
    HKEY* phkResult,
    u4* pdwDisposition
    );

u4
__stdcall
RegSetValueExW (
    HKEY hKey,
    const wchar* pValueName,
    u4 Reserved,
    u4 dwType,
    const byte* pData,
    u4 cbData
    );

u4
__stdcall
RegCloseKey (
    HKEY hKey
    );

u4
__stdcall
RegDeleteKeyW (
    HKEY hKey,
    const wchar* lpSubKey
    );

const u4 REG_NONE                     = ( 0 );   // No value type
const u4 REG_SZ                       = ( 1 );   // Unicode nul terminated string
const u4 REG_EXPAND_SZ                = ( 2 );   // Unicode nul terminated string
                                                // (with environment variable references)
const u4 REG_BINARY                   = ( 3 );   // Free form binary
const u4 REG_DWORD                    = ( 4 );   // 32-bit number
const u4 REG_DWORD_LITTLE_ENDIAN      = ( 4 );   // 32-bit number (same as REG_DWORD)
const u4 REG_DWORD_BIG_ENDIAN         = ( 5 );   // 32-bit number
const u4 REG_LINK                     = ( 6 );   // Symbolic Link (unicode)
const u4 REG_MULTI_SZ                 = ( 7 );   // Multiple Unicode strings
const u4 REG_RESOURCE_LIST            = ( 8 );   // Resource list in the resource map
const u4 REG_FULL_RESOURCE_DESCRIPTOR = ( 9 );  // Resource list in the hardware description
const u4 REG_RESOURCE_REQUIREMENTS_LIST = ( 10 );
const u4 REG_QWORD                    = ( 11 );  // 64-bit number
const u4 REG_QWORD_LITTLE_ENDIAN      = ( 11 );  // 64-bit number (same as REG_QWORD)

} //namespace reg {

} //extern "C" {
} //namespace win {
#pragma once

#include "miniutil.h"

namespace wing {
extern "C" {

const u4 IDI_ICON1 = 101;
//local icon placed in exe

//#define HWND HANDLE
typedef wincore::HANDLE HWND;
typedef wincore::HANDLE HFONT;
typedef wincore::HANDLE HINSTANCE;
typedef wincore::HANDLE HDC;
typedef wincore::HANDLE HMODULE;
typedef wincore::HANDLE HICON;
typedef wincore::HANDLE HCURSOR;
typedef wincore::HANDLE HBRUSH;
typedef wincore::HANDLE HGDIOBJ;
typedef wincore::HANDLE HMENU;

const u4 WM_CREATE  = 0x0001;
const u4 WM_DESTROY = 0x0002;
const u4 WM_MOVE    = 0x0003;
const u4 WM_SIZE    = 0x0005;

const u4 WM_ACTIVATE    = 0x0006;
const u4 WM_SETFOCUS    = 0x0007;
const u4 WM_KILLFOCUS   = 0x0008;
const u4 WM_ENABLE      = 0x000A;
const u4 WM_SETTEXT		= 0x000C;
const u4 WM_GETTEXT		= 0x000D;
const u4 WM_GETTEXTLENGTH = 0x000E;
const u4 WM_PAINT       = 0x000F;
const u4 WM_CLOSE       = 0x0010;
const u4 WM_QUIT        = 0x0012;
const u4 WM_ERASEBKGND  = 0x0014;

const u4 WM_SETFONT = 0x0030;
const u4 WM_GETFONT = 0x0031;

const u4 WM_KEYDOWN     = 0x0100;
const u4 WM_KEYUP       = 0x0101;
const u4 WM_CHAR        = 0x0102;
const u4 WM_DEADCHAR    = 0x0103;
const u4 WM_SYSKEYDOWN  = 0x0104;
const u4 WM_SYSKEYUP    = 0x0105;
const u4 WM_SYSCHAR     = 0x0106;
const u4 WM_SYSDEADCHAR = 0x0107;

const u4 WM_TIMER       = 0x0113;

const u4 WM_CTLCOLOREDIT  = 0x0133;

const u4 WM_MOUSEMOVE     = 0x0200;
const u4 WM_LBUTTONDOWN   = 0x0201;
const u4 WM_LBUTTONUP     = 0x0202;
const u4 WM_LBUTTONDBLCLK = 0x0203;
const u4 WM_RBUTTONDOWN   = 0x0204;
const u4 WM_RBUTTONUP     = 0x0205;
const u4 WM_RBUTTONDBLCLK = 0x0206;
const u4 WM_MBUTTONDOWN   = 0x0207;
const u4 WM_MBUTTONUP     = 0x0208;
const u4 WM_MBUTTONDBLCLK = 0x0209;

//Edit Control Messages
const u4 EM_GETSEL = 0x00B0;
const u4 EM_SETSEL = 0x00B1;

//u4
//__stdcall
//DefWindowProcA(
//    HWND hWnd,
//    u4 Msg,
//    u4 wParam,
//    u4 lParam);

u4
__stdcall
DefWindowProcW(
    HWND hWnd,
    u4 Msg,
    u4 wParam,
    u4 lParam);

//#ifdef UNICODE
//#define DefWindowProc  DefWindowProcW
//#else
//#define DefWindowProc  DefWindowProcA
//#endif

//WNDPROC
typedef u4 (__stdcall *WNDPROC)(HWND, u4, u4, u4);

u4
__stdcall
CallWindowProcW(
    WNDPROC lpPrevWndFunc,
    HWND hWnd,
    u4 Msg,
    u4 wParam,
    u4 lParam);

typedef struct tagRECT
{
    u4    left;
    u4    top;
    u4    right;
    u4    bottom;
} RECT;

//TODO
typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    u4          fErase;
    RECT        rcPaint;
    u4          fRestore;
    u4          fIncUpdate;
    u1          rgbReserved[32];
} PAINTSTRUCT;

typedef struct tagPOINT
{
    u4  x;
    u4  y;
} POINT;

typedef struct tagMSG {
    HWND        hwnd;
    u4          message;
    u4          wParam;
    u4          lParam;
    u4          time;
    POINT       pt;
} MSG;

u4
__stdcall
GetMessageA(
    MSG* pMsg,
    HWND hWnd,
    u4   wMsgFilterMin,
    u4   wMsgFilterMax);

u4
__stdcall
GetMessageW(
    MSG* pMsg,
    HWND hWnd,
    u4   wMsgFilterMin,
    u4   wMsgFilterMax);

u4
__stdcall
TranslateMessage(
    MSG* pMsg);

u4
__stdcall
DispatchMessageW(
    MSG* pMsg);

// PeekMessage() Options
const u4 PM_NOREMOVE = 0;
const u4 PM_REMOVE   = 1;
const u4 PM_NOYIELD  = 2;

u4
__stdcall
PeekMessageW(
    MSG* pMsg,
    HWND hWnd,
    u4 wMsgFilterMin,
    u4 wMsgFilterMax,
    u4 wRemoveMsg);

u4
__stdcall
SendMessageW(
    HWND hWnd,
    u4 Msg,
    u4 wParam,
    u4 lParam);

u4
__stdcall
InvalidateRect(
    HWND  hWnd,
    const RECT* pRect,
    u4    bErase);

HMODULE
__stdcall
GetModuleHandleA(
    char* pModuleName
    );

HMODULE
__stdcall
GetModuleHandleW(
    char* pModuleName
    );

//#ifdef UNICODE
//#define GetModuleHandle  GetModuleHandleW
//#else
//#define GetModuleHandle  GetModuleHandleA
//#endif

typedef struct tagWNDCLASSA {
    u4          style;
    WNDPROC     pfnWndProc;
    i4          cbClsExtra;
    i4          cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    const char*       pszMenuName;
    const char*       pszClassName;
} WNDCLASSA;

typedef struct tagWNDCLASSW {
    u4          style;
    WNDPROC     pfnWndProc;
    i4          cbClsExtra;
    i4          cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    const wchar*      pszMenuName;
    const wchar*      pszClassName;
} WNDCLASSW;

//#ifdef UNICODE
//typedef WNDCLASSW WNDCLASS;
//#else
//typedef WNDCLASSA WNDCLASS;
//#endif

//Class Styles
const u4 CS_VREDRAW = 0x0001;
const u4 CS_HREDRAW = 0x0002;

HICON
__stdcall
LoadIconA(
    HINSTANCE hInstance,
    u4        pIconName);

HICON
__stdcall
LoadIconW(
    HINSTANCE hInstance,
    u4        pIconName);

//#ifdef UNICODE
//#define LoadIcon  LoadIconW
//#else
//#define LoadIcon  LoadIconA
//#endif

//ID Cursors
const u4 IDC_ARROW   = 0x7F00;
const u4 IDC_IBEAM   = 0x7F00;
const u4 IDC_WAIT    = 0x7F00;
const u4 IDC_CROSS   = 0x7F00;
const u4 IDC_UPARROW = 0x7F00;

HCURSOR
__stdcall
LoadCursorA(
    HINSTANCE hInstance,
    u4 pCursorName);

HCURSOR
__stdcall
LoadCursorW(
    HINSTANCE hInstance,
    u4 pCursorName);

//#ifdef UNICODE
//#define LoadCursor  LoadCursorW
//#else
//#define LoadCursor  LoadCursorA
//#endif

/* Stock Logical Objects */
const u4 WHITE_BRUSH = 0;
const u4 LTGRAY_BRUSH = 1;
const u4 GRAY_BRUSH = 2;
const u4 DKGRAY_BRUSH = 3;
const u4 BLACK_BRUSH = 4;
const u4 NULL_BRUSH = 5;
const u4 HOLLOW_BRUSH = NULL_BRUSH;
const u4 WHITE_PEN = 6;
const u4 BLACK_PEN = 7;
const u4 NULL_PEN = 8;
const u4 OEM_FIXED_FONT = 10;
const u4 ANSI_FIXED_FONT = 11;
const u4 ANSI_VAR_FONT = 12;
const u4 SYSTEM_FONT = 13;
const u4 DEVICE_DEFAULT_FONT = 14;
const u4 DEFAULT_PALETTE = 15;
const u4 SYSTEM_FIXED_FONT = 16;

HGDIOBJ
__stdcall
GetStockObject(
	u4    i);

u2
__stdcall
RegisterClassA(
    const WNDCLASSA* pWndClass);

u2
__stdcall
RegisterClassW(
    const WNDCLASSW* pWndClass);

//#ifdef UNICODE
//#define RegisterClass  RegisterClassW
//#else
//#define RegisterClass  RegisterClassA
//#endif

//Window Styles
const u4 WS_OVERLAPPED     = 0x00000000;
const u4 WS_POPUP          = 0x80000000;
const u4 WS_CHILD          = 0x40000000;
const u4 WS_MINIMIZE       = 0x20000000;
const u4 WS_VISIBLE        = 0x10000000;
const u4 WS_DISABLED       = 0x08000000;
const u4 WS_CLIPSIBLINGS   = 0x04000000;
const u4 WS_CLIPCHILDREN   = 0x02000000;
const u4 WS_MAXIMIZE       = 0x01000000;
const u4 WS_CAPTION        = 0x00C00000; //WS_BORDER | WS_DLGFRAME
const u4 WS_BORDER         = 0x00800000;
const u4 WS_DLGFRAME       = 0x00400000;
const u4 WS_VSCROLL        = 0x00200000;
const u4 WS_HSCROLL        = 0x00100000;
const u4 WS_SYSMENU        = 0x00080000;
const u4 WS_THICKFRAME     = 0x00040000;
const u4 WS_GROUP          = 0x00020000;
const u4 WS_TABSTOP        = 0x00010000;

const u4 WS_MINIMIZEBOX    = 0x00020000;
const u4 WS_MAXIMIZEBOX    = 0x00010000;

const u4 WS_TILED          = WS_OVERLAPPED;
const u4 WS_ICONIC         = WS_MINIMIZE;
const u4 WS_SIZEBOX        = WS_THICKFRAME;

const u4 WS_OVERLAPPEDWINDOW = WS_OVERLAPPED
							| WS_CAPTION
							| WS_SYSMENU
							| WS_THICKFRAME
							| WS_MINIMIZEBOX
							| WS_MAXIMIZEBOX;

const u4 WS_TILEDWINDOW    = WS_OVERLAPPEDWINDOW;

const u4 WS_POPUPWINDOW = WS_POPUP
							| WS_BORDER
							| WS_SYSMENU;

const u4 WS_CHILDWINDOW = WS_CHILD;


HWND
__stdcall
CreateWindowExA(
    u4 dwExStyle,
    const char* pClassName,
    const char* pWindowName,
    u4 dwStyle,
    u4 X,
    u4 Y,
    u4 nWidth,
    u4 nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    void* pParam);

HWND
__stdcall
CreateWindowExW(
    u4 dwExStyle,
    const wchar* pClassName,
    const wchar* pWindowName,
    u4 dwStyle,
    u4 X,
    u4 Y,
    u4 nWidth,
    u4 nHeight,
    HWND hWndParent,
    HMENU hMenu,
    HINSTANCE hInstance,
    void* pParam);

//#ifdef UNICODE
//#define CreateWindowEx  CreateWindowExW
//#else
//#define CreateWindowEx  CreateWindowExA
//#endif

//#define CreateWindowA(pClassName, pWindowName, dwStyle, x, y,\
//nWidth, nHeight, hWndParent, hMenu, hInstance, pParam)\
//CreateWindowExA(0L, pClassName, pWindowName, dwStyle, x, y,\
//nWidth, nHeight, hWndParent, hMenu, hInstance, pParam)
//
//#define CreateWindowW(pClassName, pWindowName, dwStyle, x, y,\
//nWidth, nHeight, hWndParent, hMenu, hInstance, pParam)\
//CreateWindowExW(0L, pClassName, pWindowName, dwStyle, x, y,\
//nWidth, nHeight, hWndParent, hMenu, hInstance, pParam)

//#ifdef UNICODE
//#define CreateWindow  CreateWindowW
//#else
//#define CreateWindow  CreateWindowA
//#endif

//Show Window
const u4 SW_HIDE             = 0;
const u4 SW_SHOWNORMAL       = 1;
const u4 SW_NORMAL           = 1;
const u4 SW_SHOWMINIMIZED    = 2;
const u4 SW_SHOWMAXIMIZED    = 3;
const u4 SW_MAXIMIZE         = 3;
const u4 SW_SHOWNOACTIVATE   = 4;
const u4 SW_SHOW             = 5;
const u4 SW_MINIMIZE         = 6;
const u4 SW_SHOWMINNOACTIVE  = 7;
const u4 SW_SHOWNA           = 8;
const u4 SW_RESTORE          = 9;
const u4 SW_SHOWDEFAULT      = 10;
const u4 SW_FORCEMINIMIZE    = 11;
const u4 SW_MAX              = 11;

u4
__stdcall
ShowWindow(
    HWND hWnd,
    u4 nCmdShow);

u4
__stdcall
UpdateWindow(
    HWND hWnd);

HDC
__stdcall
BeginPaint(
    HWND hWnd,
    PAINTSTRUCT* pPaint);

u4
__stdcall
EndPaint(
    HWND hWnd,
    PAINTSTRUCT* pPaint);

typedef u4 COLORREF;

//TODO: convert (u1)(XXX) to u1(XXX)
//#define RGB(r,g,b)          ((COLORREF)(((u1)(r)|((u2)((u1)(g))<<8))|(((u4)(u1)(b))<<16)))

COLORREF RGB(u1 r, u1 g, u1 b);

COLORREF
__stdcall
SetTextColor(
	HDC hdc,
	COLORREF color);

COLORREF
__stdcall
SetBkColor(
	HDC hdc,
	COLORREF color);

HGDIOBJ
__stdcall
SelectObject(
	HDC hdc,
	HGDIOBJ h);

u4
__stdcall
Rectangle(
	HDC hdc,
	u4 left,
	u4 top,
	u4 right,
	u4 bottom);

HDC
__stdcall
GetDC(
    HWND hWnd);

i4
__stdcall
ReleaseDC(
    HWND hWnd,
    HDC hDC);

u4
__stdcall
TextOutA(
	HDC hdc,
	i4 x,
	i4 y,
	const char* pString,
	int c);

u4
__stdcall
TextOutW(
	HDC hdc,
	i4 x,
	i4 y,
	const wchar* pString,
	int c);

//#ifdef UNICODE
//#define TextOut  TextOutW
//#else
//#define TextOut  TextOutA
//#endif

typedef struct tagSIZE
{
    u4 cx;
    u4 cy;
} SIZE;

u4
__stdcall
GetTextExtentPoint32A(
    HDC hdc,
    const char* pString,
    int c,
    SIZE* psizl
    );

u4
__stdcall
GetTextExtentPoint32W(
    HDC hdc,
    const wchar* pString,
    int c,
    SIZE* psizl
    );

//#ifdef UNICODE
//#define GetTextExtentPoint32  GetTextExtentPoint32W
//#else
//#define GetTextExtentPoint32  GetTextExtentPoint32A
//#endif

//Logical Font
const u4 LF_FACESIZE = 32;

//typedef struct tagLOGFONTA
//{
//    u4 lfHeight;
//    u4 lfWidth;
//    u4 lfEscapement;
//    u4 lfOrientation;
//    u4 lfWeight;
//    u1 lfItalic;
//    u1 lfUnderline;
//    u1 lfStrikeOut;
//    u1 lfCharSet;
//    u1 lfOutPrecision;
//    u1 lfClipPrecision;
//    u1 lfQuality;
//    u1 lfPitchAndFamily;
//    char* lfFaceName[LF_FACESIZE];
//} LOGFONTA;

typedef struct tagLOGFONTW
{
    u4 lfHeight;
    u4 lfWidth;
    u4 lfEscapement;
    u4 lfOrientation;
    u4 lfWeight;
    u1 lfItalic;
    u1 lfUnderline;
    u1 lfStrikeOut;
    u1 lfCharSet;
    u1 lfOutPrecision;
    u1 lfClipPrecision;
    u1 lfQuality;
    u1 lfPitchAndFamily;
    wchar lfFaceName[LF_FACESIZE];
} LOGFONTW;

//Font Weights
const u4 FW_DONTCARE = 0;

//swapped around
const u4 CHARSET_DEFAULT = 1;
const u4 CHARSET_SHIFTJIS = 128;

const u4 OUT_DEFAULT_PRECIS = 0;
const u4 CLIP_DEFAULT_PRECIS = 0;

//swapped around
const u4 QUALITY_DEFAULT = 0;
const u4 QUALITY_DRAFT   = 1;
const u4 QUALITY_PROOF   = 2;
//(WINVER >= 0x0400)
const u4 QUALITY_NONANTIALIASED = 3;
const u4 QUALITY_ANTIALIASED    = 4;

//Font Families
const u4 FF_DONTCARE       = (0<<4);
//Don't care or don't know.
const u4 FF_ROMAN          = (1<<4);
//Variable stroke width, serifed.
//Times Roman, Century Schoolbook, etc.
const u4 FF_SWISS          = (2<<4);
//Variable stroke width, sans-serifed.
//Helvetica, Swiss, etc.
const u4 FF_MODERN         = (3<<4);
//Constant stroke width, serifed or sans-serifed.
//Pica, Elite, Courier, etc.
const u4 FF_SCRIPT         = (4<<4);
//Cursive, etc.
const u4 FF_DECORATIVE     = (5<<4);
//Old English, etc.

//HFONT
//__stdcall
//CreateFontIndirectA(
//	LOGFONTA* plf);

HFONT
__stdcall
CreateFontIndirectW(
	LOGFONTW* plf);

//#ifdef UNICODE
//#define CreateFontIndirect  CreateFontIndirectW
//#else
//#define CreateFontIndirect  CreateFontIndirectA
//#endif

//Virtual Keys
const u4 VK_BACK         = 0x08;
const u4 VK_TAB          = 0x09;

const u4 VK_CLEAR        = 0x0C;
const u4 VK_RETURN       = 0x0D;

const u4 VK_SHIFT        = 0x10;
const u4 VK_CONTROL      = 0x11;
const u4 VK_MENU         = 0x12;
const u4 VK_PAUSE        = 0x13;
const u4 VK_CAPITAL      = 0x14;

const u4 VK_ESCAPE       = 0x1B;

const u4 VK_SPACE        = 0x20;
const u4 VK_PRIOR        = 0x21;
const u4 VK_NEXT         = 0x22;
const u4 VK_END          = 0x23;
const u4 VK_HOME         = 0x24;
const u4 VK_LEFT         = 0x25;
const u4 VK_UP           = 0x26;
const u4 VK_RIGHT        = 0x27;
const u4 VK_DOWN         = 0x28;

const u4 VK_INSERT       = 0x2D;
const u4 VK_DELETE       = 0x2E;

const u4 VK_F1           = 0x70;
const u4 VK_F2           = 0x71;
const u4 VK_F3           = 0x72;
const u4 VK_F4           = 0x73;
const u4 VK_F5           = 0x74;
const u4 VK_F6           = 0x75;
const u4 VK_F7           = 0x76;
const u4 VK_F8           = 0x77;
const u4 VK_F9           = 0x78;
const u4 VK_F10          = 0x79;
const u4 VK_F11          = 0x7A;
const u4 VK_F12          = 0x7B;
const u4 VK_F13          = 0x7C;
const u4 VK_F14          = 0x7D;
const u4 VK_F15          = 0x7E;
const u4 VK_F16          = 0x7F;
const u4 VK_F17          = 0x80;
const u4 VK_F18          = 0x81;
const u4 VK_F19          = 0x82;
const u4 VK_F20          = 0x83;
const u4 VK_F21          = 0x84;
const u4 VK_F22          = 0x85;
const u4 VK_F23          = 0x86;
const u4 VK_F24          = 0x87;

//Edit Control Notification Codes
const u4 EN_SETFOCUS        = 0x0100;
const u4 EN_KILLFOCUS       = 0x0200;
const u4 EN_CHANGE          = 0x0300;
const u4 EN_UPDATE          = 0x0400;
const u4 EN_ERRSPACE        = 0x0500;
const u4 EN_MAXTEXT         = 0x0501;
const u4 EN_HSCROLL         = 0x0601;
const u4 EN_VSCROLL         = 0x0602;

//Window field offsets for GetWindowLong()
const u4 GWL_WNDPROC = -4;

u4
__stdcall
SetWindowLongW(
    HWND hWnd,
    i4 nIndex,
    u4 dwNewLong);

HWND
__stdcall
SetFocus(
    HWND hWnd);

//SetWindowPos Flags
const u4 SWP_NOZORDER      = 0x0004;

u4
__stdcall
SetWindowPos(
    HWND hWnd,
    HWND hWndInsertAfter,
    i4 X,
    i4 Y,
    i4 cx,
    i4 cy,
    u4 uFlags);

typedef void (__stdcall *TIMERPROC)(HWND, u4, u4, u4);

u4*
__stdcall
SetTimer(
    HWND hWnd,
    u4 nIDEvent,
    u4 uElapse,
    TIMERPROC lpTimerFunc);

u4
__stdcall
KillTimer(
    HWND hWnd,
    u4 uIDEvent);

}//extern "C" {
}//namespace wing {
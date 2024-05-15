#include "ui.h"
#include "win.h"

using namespace wing;
using namespace win;

//GLOBALS
UI gui;

bool UI::messageWaitFor() {
	return GetMessageW(&msg, null, 0, 0);
}

bool UI::messagePeek() {
	return wing::PeekMessageW(&msg, null, 0, 0, wing::PM_REMOVE);
}

void UI::messageINeedWmChar() {
	TranslateMessage(&msg);
}

void UI::messageProcess() {
	DispatchMessageW(&msg);
}

void UI::rect_full() {
	rect.top = 0;
	rect.left = 0;
	rect.right = width;
	rect.bottom = height;
}

void UI::invalidate_all() {
	//rect_full();
	//InvalidateRect(hWnd, &rect, false);
	InvalidateRect(hWnd, null, null);
}

void UI::init(const tchar* window_title, u4 starting_width, u4 starting_height, WNDPROC WndProc) {
	hdc = null;
	cur_x = 0;
	cur_y = 0;
	//font_height = 0;
	need_redraw = false;
	//hInstance = (HINSTANCE)0x400000;
	hInstance = GetModuleHandleW(null);

	width = starting_width;
	height = starting_height;

	WNDCLASSW wcex;

	//wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.pfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	//wcex.hIcon = 0;
	//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hIcon = LoadIconW(hInstance, IDI_ICON1);
	wcex.hCursor = LoadCursorW(null, IDC_ARROW);
	//wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.pszMenuName = 0;
	wcex.pszClassName = window_title;
	//wcex.hIconSm = 0;
	//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(107));

	u4 res = RegisterClassW(&wcex);
	if (res == 0) ErrorLazy();

	hWnd = CreateWindowExW(0, window_title, window_title, WS_OVERLAPPEDWINDOW,
		32, 32, width, height, null, null, hInstance, null);
	if (!hWnd) ErrorLazy();

	ShowWindow(hWnd, SW_SHOW);
	UpdateWindow(hWnd);

}

void UI::drawstart_wm_paint() {
	hdc = BeginPaint(hWnd, &ps);
	//>An application should not call BeginPaint except in response to a WM_PAINT message.
	//GetDC is used to draw anywhere, BeginPaint is only in WM_PAINT.
	cur_x = 0;
	cur_y = 0;

	SetTextColor(hdc, RGB(0xB0, 0xB0, 0xB0));
	SetBkColor(hdc, RGB(0, 0, 0));
}

void UI::finishWindow() {
	if (cur_y < height) {
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, 0, cur_y, width, height);
	}
}

void UI::drawend_wm_paint() {
	
	EndPaint(hWnd, &ps);
}

void UI::drawstart() {
	hdc = GetDC(hWnd);
	cur_x = 0;
	cur_y = 0;

	SetTextColor(hdc, RGB(0xB0, 0xB0, 0xB0));
	SetBkColor(hdc, RGB(0, 0, 0));
}

void UI::drawend() {
	ReleaseDC(hWnd, hdc);
}

void UI::text(const wchar* text, u4 text_len) {
	if (text_len == 0) return;
	TextOutW(hdc, cur_x, cur_y, text, text_len);
	SIZE size;
	GetTextExtentPoint32W(hdc, text, text_len, &size);
	cur_x += size.cx;
	//font_height = size.cy;
}

void UI::text(const wchar* text) {
	if (text == 0) return;
	u4 text_len = ustr::wcharLen(text);
	TextOutW(hdc, cur_x, cur_y, text, text_len);
	SIZE size;
	GetTextExtentPoint32W(hdc, text, text_len, &size);
	cur_x += size.cx;
	//font_height = size.cy;
}

void UI::numberu(u4 number) {
	wchar_t tempbuf[9];
	u4 strlen;
	wchar_t* numstr = ustr::NumberuToWstringSafe(number, tempbuf, 9, &strlen);
	text(numstr, strlen);
}

void UI::finishLine() {
	if (cur_y >= height) return;
	if (cur_x <= width) {
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, cur_x, cur_y, width, cur_y + font_height);
	}
	cur_x = 0;
	cur_y += font_height;
}

void UI::skipLine() {
	cur_x = 0;
	cur_y += font_height;
}

//void UI::text(const tchar* text) {
//	u4 text_len = lstrlen(text);
//	if (text_len == 0) return;
//	TextOut(hdc, cur_x, cur_y, text, text_len);
//	SIZE size;
//	GetTextExtentPoint32(hdc, text, text_len, &size);
//	cur_x += size.cx;
//	font_height = size.cy;
//}

void UI::textline(const tchar* text) {
	u4 used = ustr::tcharLen(text);
	if (used == 0) return;
	if (cur_y >= height) return;
	if (cur_x >= width) {
		//let me goto over variable declarations you bastard.
		cur_x = 0;
		cur_y += font_height;
		return;
	}
	TextOutW(hdc, cur_x, cur_y, text, used);
	SIZE size;
	GetTextExtentPoint32W(hdc, text, used, &size);
	cur_x += size.cx;
	font_height = size.cy;
	if (cur_x <= width) {
		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
		Rectangle(hdc, cur_x, cur_y, width, cur_y + font_height);
	}
	cur_x = 0;
	cur_y += font_height;
}

//void textline(Str* text) {
//	if (text->used == 0) return;
//	if (cur_y >= height) return;
//	if (cur_x >= width) {
//		//let me goto over variable declarations you bastard.
//		cur_x = 0;
//		cur_y += font_height;
//		return;
//	}
//	TextOut(hdc, cur_x, cur_y, text->arr, text->used);
//	SIZE size;
//	GetTextExtentPoint32(hdc, text->arr, text->used, &size);
//	cur_x += size.cx;
//	font_height = size.cy;
//	if (cur_x <= width) {
//		SelectObject(hdc, GetStockObject(BLACK_BRUSH));
//		Rectangle(hdc, cur_x, cur_y, width, cur_y + font_height);
//	}
//	cur_x = 0;
//	cur_y += font_height;
//}

//TODO: remove those
u4 UI::mouseLineGet(u4 lParam) {
	i2 yPos = util::i2high(lParam);
	return yPos / font_height;
}

u4 UI::mouseXGet(u4 lParam) {
	return util::i2low(lParam);
}

void UI::on_wm_size(u4 lParam) {
	width = util::u2low(lParam);
	height = util::u2high(lParam);
}

i2 UI::on_wm_mousewheel(u4 wParam) {
	i2 zDelta = util::i2high(wParam);
	zDelta = zDelta / -120;
	return zDelta;
}

void UI::fontInit() {
	LOGFONTW font_struct = {
		20, //height
		0, //width
		0,
		0, //escapement, orientation
		FW_DONTCARE, //width
		false, //italic
		false, //underline
		false, //strike out
		CHARSET_DEFAULT,
		OUT_DEFAULT_PRECIS, //outPrecision
		CLIP_DEFAULT_PRECIS, //clipPrecision
		QUALITY_NONANTIALIASED, //quality
		FF_DONTCARE, //pitch and family
		//TEXT("MS Gothic") //TEXT("‚l‚r ƒSƒVƒbƒN")
		L"Courier New" //TEXT("‚l‚r ƒSƒVƒbƒN")
		//LATER: japanese windows prior to 2000/xp wouldn't understand not localized font name.

	};

	font = CreateFontIndirectW(&font_struct);
	font_height = font_struct.lfHeight;

}

void UI::fontSet() {
	//THROW("not implemented");
	SelectObject(hdc, font);
}
#pragma once

#include "miniutil.h"
#include "wing.h"

//Only keep here various helper functions that you usually copypaste, don't bother with actually processing messages or doing callbacks.
class UI {
public:
	wing::HWND hWnd;
	u4 width;
	u4 height;
	u4 cur_x;
	u4 cur_y;
	u4 font_height;
	wing::HFONT font;
	bool need_redraw;
	wing::HINSTANCE hInstance;
	wing::HDC hdc;
	wing::PAINTSTRUCT ps;
	wing::RECT rect;

	wing::MSG msg;

	bool messageWaitFor();
	bool messagePeek();

	void messageINeedWmChar();

	void messageProcess();

	void rect_full();

	//fill rect with correct values before calling
	void invalidate_all();

	void init(const tchar* window_title, u4 starting_width, u4 starting_height, wing::WNDPROC WndProc);

	void drawstart_wm_paint();

	void finishWindow();

	void drawend_wm_paint();

	void drawstart();

	void drawend();

	void text(const wchar* text, u4 text_len);

	void text(const wchar* text);

	void numberu(u4 number);

	void finishLine();

	void skipLine();

	void textline(const tchar* text);

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

	u4 mouseLineGet(u4 lParam);

	u4 mouseXGet(u4 lParam);

	void on_wm_size(u4 lParam);

	i2 on_wm_mousewheel(u4 wParam);

	void fontInit();

	void fontSet();
};

extern UI gui;

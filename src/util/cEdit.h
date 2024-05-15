
//u4 __stdcall edit_procedure(wing::HWND wnd, u4 msg, u4 wParam, u4 lParam);

namespace nmain {

u4 __stdcall edit_procedure(wing::HWND wnd, u4 msg, u4 wParam, u4 lParam);

} //namespace nmain

class cEdit {
public:
	wing::HWND mHandle;
	bool mVisible;
	//bool mCtrlDown;
	//bool mShiftDown;
	u4 mLine;
	wing::WNDPROC m_edit_procedure_old;

	void Focus() {
		if (mVisible) wing::SetFocus(mHandle);
	}

	void Hide() {
		mVisible = false;
		wing::ShowWindow(mHandle, wing::SW_HIDE);
	}

	void Unhide() {
		mVisible = true;
		wing::ShowWindow(mHandle, wing::SW_SHOW);
		Focus();
	}

	void OnWmCreate(wing::HWND hWnd) {
		mHandle = wing::CreateWindowExW(0, L"EDIT", L"",
			//WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
			wing::WS_CHILD | wing::WS_VISIBLE,
			0, 0, 0x400, gui.font_height, hWnd, 0, wing::GetModuleHandleW(null), null);
		if (mHandle == null) ErrorLazy();
		
		wing::SendMessageW(mHandle, wing::WM_SETFONT, gui.font, 0);
		m_edit_procedure_old = (wing::WNDPROC)wing::SetWindowLongW(mHandle, wing::GWL_WNDPROC, (u4)nmain::edit_procedure);

		mVisible = false;
		//mCtrlDown = false;
		//mShiftDown = false;
		mLine = 0;
		Hide();
	}

	void OnWmSize() {
		u4 lineHeight = mLine * gui.font_height;
		wing::SetWindowPos(mHandle, null, 0, lineHeight, gui.width, gui.font_height, wing::SWP_NOZORDER);
	}

	void OnWM_CTLCOLOREDIT(u4 wParam) {
		wing::HDC hdcEdit = (wing::HDC)wParam;
		wing::SetTextColor(hdcEdit, wing::RGB(0xB0, 0xB0, 0xB0));
		wing::SetBkColor(hdcEdit, wing::RGB(0, 0, 0));
	}

	void Set(const wchar* text, u4 textLen) {
		wing::SendMessageW(mHandle, wing::WM_SETTEXT, 0, (u4)text);
		wing::SendMessageW(mHandle, wing::EM_SETSEL, textLen, textLen);
	}

	void Get(wchar* text, u4* textLen, u4 textCap) {
		//ErrorLazy(); //Not Done Yet
		u4 len = wing::SendMessageW(mHandle, wing::WM_GETTEXTLENGTH, 0, 0);
		if (len > textCap) len = textCap;
		*textLen = len;
		wing::SendMessageW(mHandle, wing::WM_GETTEXT, textCap + 1, (u4)text);
	}
};
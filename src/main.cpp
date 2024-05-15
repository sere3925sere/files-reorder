
#include "main.h"

#ifndef _UNICODE
#error "Please define _UNICODE"
#endif

#include "util/miniutil.h"
#include "util/win.h"
#include "util/wing.h"
#include "util/ui.h"
#include "util/MemorySimple.h"
#include "util/FileIterator.h"
#include "util/FileIteratorOptions.h"
#include "util/cEdit.h"

#include "util/ufile.h"

//GLOBAL

bytep gmemReentrancePoint;

cMenu gMenu;

void cMenu::Init() {
	mMode = 0;
	mSelectionMax = 0;
	mPathRoot = (wchar*)mem::Get(win::file::MAX_PATH);
	mFilenamesCap = 0x20000;
	mFilenamesLen = 0;
	mFilenames = (wchar**)mem::GetBig(mFilenamesCap*sizeof(wchar*));
	mFilenamesTime = (u8*)mem::GetBig(mFilenamesCap*sizeof(u8));
	mFilenamesSortedCap = 0x80;
	mFilenamesSortedLen = 0;
	mFilenamesSorted = (u4*)mem::Get(mFilenamesSortedCap*sizeof(u4));
	mFilenamesSortedNumber = (u4*)mem::Get(mFilenamesSortedCap*sizeof(u4));

	mSearchString = (wchar*)mem::Get(win::file::MAX_PATH * sizeof(wchar));
}

cEdit gEdit;

namespace nmain {


void registryRead() {
	u4 res;
	u4 ver;
	u4 bufsize;

	bufsize = sizeof(u4);
	res = win::reg::RegGetValueW(win::reg::HKEY_CURRENT_USER, L"Software\\aimino777\\files-reorder", L"VER", win::reg::RRF_RT_REG_DWORD, null, &ver, &bufsize);
	if (res != 0) goto fail;
	if (ver != 1) goto fail;

	bufsize = win::file::MAX_PATH;
	res = win::reg::RegGetValueW(win::reg::HKEY_CURRENT_USER, L"Software\\aimino777\\files-reorder", L"PATH", win::reg::RRF_RT_REG_SZ, null, gMenu.mPathRoot, &bufsize);
	if (res != 0) goto fail;

	gMenu.mRegistryUse = true;
	return;
fail:
	gMenu.mRegistryUse = false;
	return;
}

void registryWrite() {
	win::reg::HKEY key;
	u4 dwDisposition;
	u4 res;
	res = win::reg::RegCreateKeyExW(win::reg::HKEY_CURRENT_USER, L"Software\\aimino777\\files-reorder", 
          0, null, win::reg::REG_OPTION_NON_VOLATILE,
          win::reg::KEY_WRITE, null, &key, &dwDisposition);
	if (res != 0) ErrorLazy();

	u4 value = 1;
	res = win::reg::RegSetValueExW(key,      // subkey handle 
       L"VER",                               // value name 
       0,                                    // must be zero 
       win::reg::REG_DWORD,                  // value type 
       (byte*) &value,                       // pointer to value data 
	   sizeof(u4)                            // length of value data 
	   ); 
	if (res != 0) ErrorLazy();

	res = win::reg::RegSetValueExW(key,      // subkey handle 
       L"PATH",                             // value name 
       0,                                  // must be zero 
       win::reg::REG_SZ,                  // value type 
       (byte*) gMenu.mPathRoot,          // pointer to value data 
	   (ustr::wcharLen(gMenu.mPathRoot)    +1)*sizeof(wchar)
	                                   // length of value data 
	   ); 
	if (res != 0) ErrorLazy();

	win::reg::RegCloseKey(key);
}

void registryDelete() {
	u4 res = win::reg::RegDeleteKeyW(win::reg::HKEY_CURRENT_USER, L"Software\\aimino777\\files-reorder");
	if (res != 0) ErrorLazy();
}


const u4 ctLinesPerScreen = 30;

enum Mode {
	modeLoading,
	modeMain,
	modeOptions,
	modeEditFolderPath,
	modeHelp,
};

void main_fiStart(FileIterator::cFileIterator* fi) {

	fiInit(fi, gMenu.mPathRoot);

	FileIterator::cFileIteratorOptions* fio = fi->fio;

	fio->OnStart = FileIteratorOptions::FileIteratorGetFilelist_OnStart;
	fio->OnFile = FileIteratorOptions::FileIteratorGetFilelist_OnFile;
	fio->OnEnd = FileIteratorOptions::FileIteratorGetFilelist_OnEnd;
	fio->OnDraw = FileIteratorOptions::FileIteratorGetFilelist_OnDraw;
	fio->data = (FileIteratorOptions::cFileIteratorGetFilelist_Data*)mem::Get(sizeof(FileIteratorOptions::cFileIteratorGetFilelist_Data));

	fiStart(fi);
}

void menuLoading(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();
			gui.text(L"Loading...");
			gui.finishLine();

			gui.finishWindow();

			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.mSelection0++;
				if (gMenu.mSelection0 > gMenu.mSelectionMax) gMenu.mSelection0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.mSelection0--;
				if (gMenu.mSelection0 < 0) gMenu.mSelection0 = gMenu.mSelectionMax;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_ESCAPE) {
				wincore::ExitProcess(0);
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.mSelectionMax) linePos = -1;
			gMenu.mSelection0 = linePos;
			//to not redraw too often
			if (gMenu.mSelectionPrev != gMenu.mSelection0) {
				gMenu.mSelection0 = gMenu.mSelection0;
				gui.invalidate_all();
			}
			break;
		}
	}
}

void DrawOption(bool active) {
	if (active) gui.text(L">",1);
	else gui.text(L" ", 1);
}


void menuMain(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			if (gfi.error) {
				DrawOption(false);
				gui.text(L"Folder path doesn't exist.");
				gui.finishLine();
				DrawOption(false);
				gui.text(L"Press O for options.");
				gui.finishLine();
				DrawOption(false);
				gui.text(L"Press R or UP to refresh.");
				gui.finishLine();
			}

			else if (gMenu.mFilenamesLen == 0) {
				DrawOption(false);
				gui.text(L"Folder is empty.");
				gui.finishLine();
				DrawOption(false);
				gui.text(L"Press O for options.");
				gui.finishLine();
				DrawOption(false);
				gui.text(L"Press R or UP to refresh.");
				gui.finishLine();
			}
			
			for (u4 i = 0; ; i++) {
				u4 isk = i + gMenu.mLinesSkipped;
				if (isk >= gMenu.mFilenamesLen) break;
				if (i >= ctLinesPerScreen) break;
				DrawOption(gMenu.mSelection0 == i);
				gui.text(gMenu.mFilenames[isk]);
				gui.finishLine();
			}

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.mSelection0++;
				if (gMenu.mSelection0 > gMenu.mSelectionMax) gMenu.mSelection0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.mSelection0--;
				if (gMenu.mSelection0 < 0) {
					gMenu.mSelection0 = 0;
					gMenu.Reset();
				}
				gui.invalidate_all();
			}
			//TODO: change all if to else if
			if (wParam == L'R') {
				gMenu.Reset();
			}
			if (wParam == L'O') {
				gMenu.mMode = modeOptions;
				gMenu.mSelectionMax = 1;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_F1) {
				gMenu.mMode = modeHelp;
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				gMenu.MainOnEnter();
			}
			if (wParam == wing::VK_NEXT || wParam == wing::VK_RIGHT) {
				gMenu.mLinesSkipped += ctLinesPerScreen;
				if (gMenu.mLinesSkipped > gMenu.mFilenamesLen) {
					gMenu.mLinesSkipped -= ctLinesPerScreen;
				}
				gui.invalidate_all();
			}
			if (wParam == wing::VK_PRIOR  || wParam == wing::VK_LEFT) {
				gMenu.mLinesSkipped -= ctLinesPerScreen;
				if (gMenu.mLinesSkipped < 0) {
					gMenu.mLinesSkipped = 0;
					gMenu.mSelection0 = 0;
				}
				gui.invalidate_all();
			}
			if (wParam == wing::VK_ESCAPE) {
				wincore::ExitProcess(0);
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.mSelectionMax) linePos = -1;
			gMenu.mSelection0 = linePos;
			//to not redraw too often
			if (gMenu.mSelectionPrev != gMenu.mSelection0) {
				gMenu.mSelectionPrev = gMenu.mSelection0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			gMenu.MainOnEnter();
			break;
		}
	}
}

void OptionsOnEnter() {
	if (gMenu.mSelection0 == 0) {
		gMenu.mMode = modeEditFolderPath;
		gEdit.Set(gMenu.mPathRoot, ustr::wcharLen(gMenu.mPathRoot));
		gEdit.Unhide();
	} else if (gMenu.mSelection0 == 1) {
		gMenu.mRegistryUse = !gMenu.mRegistryUse;
		if (gMenu.mRegistryUse) {
			registryWrite();
		} else {
			registryDelete();
		}
	}
	gui.invalidate_all();
}

void menuOptions(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			DrawOption(gMenu.mSelection0 == 0);
			gui.text(L"Folder path:[");
			gui.text(gMenu.mPathRoot);
			gui.text(L"]");
			gui.finishLine();

			DrawOption(gMenu.mSelection0 == 1);
			gui.text(L"[");
			if (gMenu.mRegistryUse) gui.text(L"o");
			else gui.text(L" ");
			gui.text(L"] Save options to registry.");
			gui.finishLine();

			DrawOption(false);
			gui.text(L"Press Escape to go back.");
			gui.finishLine();

			DrawOption(false);
			gui.text(L"Options are stored in:");
			gui.finishLine();
			gui.text(L"HKEY_CURRENT_USER\\");
			gui.text(L"Software\\aimino777\\files-reorder");
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_DOWN) {
				gMenu.mSelection0++;
				if (gMenu.mSelection0 > gMenu.mSelectionMax) gMenu.mSelection0 = 0;
				gui.invalidate_all();
			}
			else if (wParam == wing::VK_UP) {
				gMenu.mSelection0--;
				if (gMenu.mSelection0 < 0) {
					gMenu.mSelection0 = gMenu.mSelectionMax;
				}
				gui.invalidate_all();
			}
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN) {
				OptionsOnEnter();
			}
			if (wParam == wing::VK_NEXT || wParam == wing::VK_RIGHT) {
				gMenu.mLinesSkipped += ctLinesPerScreen;
				if (gMenu.mLinesSkipped > gMenu.mFilenamesLen) {
					gMenu.mLinesSkipped -= ctLinesPerScreen;
				}
				gui.invalidate_all();
			}
			if (wParam == wing::VK_PRIOR  || wParam == wing::VK_LEFT) {
				gMenu.mLinesSkipped -= ctLinesPerScreen;
				if (gMenu.mLinesSkipped < 0) {
					gMenu.mLinesSkipped = 0;
					gMenu.mSelection0 = 0;
				}
				gui.invalidate_all();
			}
			if (wParam == wing::VK_ESCAPE || wParam == wing::VK_BACK) {
				gMenu.mMode = modeMain;
				gMenu.mSelectionMax = ctLinesPerScreen;
				// ^TODO: make something like SwitchToMain(), so that mSelectionMax can be calculated properly.
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.mSelectionMax) linePos = -1;
			gMenu.mSelection0 = linePos;
			//to not redraw too often
			if (gMenu.mSelectionPrev != gMenu.mSelection0) {
				gMenu.mSelectionPrev = gMenu.mSelection0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			OptionsOnEnter();
			break;
		}
	}
}

void menuHelp(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			gui.text(L"files-reorder version 2");
			gui.finishLine();
			gui.text(L"https://gitlab.com/aimino777/files-reorder");
			gui.finishLine();
			gui.text(L"Controls:");
			
			gui.text(L"ESC to go back.");
			gui.finishLine();
			gui.text(L"UP and DOWN to go up and down that list.");
			gui.finishLine();
			gui.text(L"RIGHT, LEFT, PAGE DOWN and PAGE UP to flip pages.");
			gui.finishLine();
			gui.text(L"ENTER or mouse click to sort selected file.");
			gui.finishLine();
			gui.text(L"R to refresh file list.");
			gui.finishLine();
			gui.text(L"UP when on the first page and on top of the list to also refresh file list.");
			gui.finishLine();
			gui.text(L"O for options.");
			gui.finishLine();
			gui.text(L"F1 for help.");
			gui.finishLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_SPACE || wParam == wing::VK_RETURN || 
				wParam == wing::VK_ESCAPE || wParam == wing::VK_BACK)
			{
				gMenu.mMode = modeMain;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_MOUSEMOVE:
		{
			i2 yPos = i2(lParam >> 16);
			i2 linePos = yPos / gui.font_height;
			if (linePos > gMenu.mSelectionMax) linePos = -1;
			gMenu.mSelection0 = linePos;
			//to not redraw too often
			if (gMenu.mSelectionPrev != gMenu.mSelection0) {
				gMenu.mSelectionPrev = gMenu.mSelection0;
				gui.invalidate_all();
			}
			break;
		}
	case wing::WM_LBUTTONUP:
		{
			OptionsOnEnter();
			break;
		}
	}
}

void menuEditFolderPath(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	switch (message)
	{
	case wing::WM_PAINT:
		{
			gui.drawstart_wm_paint();
			gui.fontSet();

			gui.skipLine();

			gui.finishWindow();
			gui.drawend_wm_paint();
			break;
		}
	case wing::WM_KEYDOWN:
		{
			if (wParam == wing::VK_ESCAPE) {
				gEdit.Hide();
				gMenu.mMode = modeOptions;
				gui.invalidate_all();
			}
			break;
		}
	}
}


void menu(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam) {
	if (gMenu.mMode == modeLoading) return menuLoading(hWnd, message, wParam, lParam);
	else if (gMenu.mMode == modeMain) return menuMain(hWnd, message, wParam, lParam);
	else if (gMenu.mMode == modeOptions) return menuOptions(hWnd, message, wParam, lParam);
	else if (gMenu.mMode == modeEditFolderPath) return menuEditFolderPath(hWnd, message, wParam, lParam);
	else if (gMenu.mMode == modeHelp) return menuHelp(hWnd, message, wParam, lParam);
	
}

void FilenamesSortSwap(i4 x) {
	wchar* tw = gMenu.mFilenames[x];
	gMenu.mFilenames[x] = gMenu.mFilenames[x+1];
	gMenu.mFilenames[x+1] = tw;

	u8 t = gMenu.mFilenamesTime[x];
	gMenu.mFilenamesTime[x] = gMenu.mFilenamesTime[x+1];
	gMenu.mFilenamesTime[x+1] = t;

}

void FilenamesSort() {
	if (gMenu.mFilenamesLen <= 1) return;
	i4 imax = gMenu.mFilenamesLen - 2;
	i4 i = 0;
	while (true) {
		i4 x = i;
		while (true) {
			if (gMenu.mFilenamesTime[x+1] > gMenu.mFilenamesTime[x]) {
				FilenamesSortSwap(x);
			} else {
				break;
			}
			x--;
			if (x < 0) break;
		}
		i++;
		if (i > imax) break;
	}

}

bool StartsWith(wchar* str, wchar* strsub) {
	wchar* cur = str;
	wchar* cursub = strsub;
	while (true) {
		if (*cur == 0) ErrorLazy(); //TODO: never happens for us
		if (*cursub == 0) return true;
		if (*cur != *cursub) return false;
		cur++;
		cursub++;
	}
}

u4 SubstringNumber(const wchar* str, u4 strlen) {
	u4 res = 0;
	const wchar* cur = str + strlen;
	while (true) {
		if (*cur >= L'0' && *cur <= L'9') {
			res = res*10 + (*cur - L'0');
		} else {
			return res;
		}
		cur++;
	}
}

//returns true on error
bool FilenamesSearch() {
	//gMenu.mFilenames[gMenu.mFilenamesLen] = null;

	if (gMenu.mFilenamesLen <= 1) return true;
	//paranoia
	if (gMenu.mSearchString[0] == 0) ErrorLazy();

	u4 strsublen = ustr::wcharLen(gMenu.mSearchString);

	i4 imax = gMenu.mFilenamesLen;
	i4 i = 0;
	while (true) {
		if (StartsWith(gMenu.mFilenames[i], gMenu.mSearchString)) {
			u4 len = gMenu.mFilenamesSortedLen;
			gMenu.mFilenamesSorted[len] = i;
			wchar* filename = gMenu.mFilenames[i];
			gMenu.mFilenamesSortedNumber[len] = SubstringNumber(filename, strsublen);
			gMenu.mFilenamesSortedLen = len + 1;
			if (gMenu.mFilenamesSortedLen >= gMenu.mFilenamesSortedCap) {
				util::Alert(L"More than mFilenamesSortedCap files, how rare.");
				// ^ TODO: I don't expect to have more than 100 files to sort
				return true;
			}
		}
		i++;
		if (i >= imax) break;
	}
	return false;
}

void FilenamesSortedSortSwap(i4 x) {
	u4 tw = gMenu.mFilenamesSorted[x];
	gMenu.mFilenamesSorted[x] = gMenu.mFilenamesSorted[x+1];
	gMenu.mFilenamesSorted[x+1] = tw;

	u4 t = gMenu.mFilenamesSortedNumber[x];
	gMenu.mFilenamesSortedNumber[x] = gMenu.mFilenamesSortedNumber[x+1];
	gMenu.mFilenamesSortedNumber[x+1] = t;
}

void FilenamesSortedSort() {
	if (gMenu.mFilenamesSortedLen <= 1) return;
	i4 imax = gMenu.mFilenamesSortedLen - 2;
	i4 i = 0;
	while (true) {
		i4 x = i;
		while (true) {
			if (gMenu.mFilenamesSortedNumber[x+1] < gMenu.mFilenamesSortedNumber[x]) {
				FilenamesSortedSortSwap(x);
			} else {
				break;
			}
			x--;
			if (x < 0) break;
		}
		i++;
		if (i > imax) break;
	}
}

void FilenamesReorder() {
	if (gMenu.mFilenamesSortedLen < 2) return;
	ufile::cFile file;
	wchar buf[win::file::MAX_PATH];
	ustr::wcharCopy(gMenu.mPathRoot, buf);

	win::file::FILETIME filetime;
	u8 time_u8 = gMenu.mFilenamesTime[gMenu.mFilenamesSorted[0]];
	
	
	u4 rootlen = ustr::wcharLen(gMenu.mPathRoot);
	wchar* buf2 = buf + rootlen;
	for (u4 i = 0; i < gMenu.mFilenamesSortedLen; i++) {
		u4 filenameIndex = gMenu.mFilenamesSorted[i];
		wchar* filename = gMenu.mFilenames[filenameIndex];
		ustr::wcharCopy(filename, buf2);
		
		{
			u4 i = 8;
			errorCode err;
			goto labelOpenFileMidLoop;
			while (true) {
				win::Sleep(1000);
labelOpenFileMidLoop:
				err = file.OpenForReadWrite(buf);
				if (!err || !--i) break;
			}
		}
		
		u4* time_u4p = (u4*)&time_u8;
		filetime.dwLowDateTime = *time_u4p;
		filetime.dwHighDateTime = *(time_u4p+1);
		time_u8 += 10000000; //1 second
		
		file.SetTime(&filetime, &filetime, &filetime);
		file.Close();
	}
}

//Crops search string to a number.
//Crops it to empty string on failure.
void SearchStringCrop() {
	wchar* curstart = gMenu.mSearchString;
	//paranoia
	if (curstart[0] == 0) ErrorLazy();
	wchar* cur = curstart + ustr::wcharLen(curstart);
	cur--;
	//skipping file extension
	while (true) {
		if (cur == curstart) goto fail;
		if (*cur == L'.') break;
		cur--;
	}
	//skipping number
	cur--;
	if (cur == curstart) goto fail;
	if (!(*cur >= L'0' && *cur <= L'9')) goto fail;
	while (true) {
		cur--;
		if (cur == curstart) goto fail;
		if (!(*cur >= L'0' && *cur <= L'9')) break;
	}
	if (*cur != 'p') goto fail;
	cur--;
	if (cur == curstart) goto fail;
	if (*cur != '_') goto fail;
	cur++;
	cur++;
	*cur = 0;
	return;
fail:
	*curstart = 0;
	return;
}

void DoTheThing() {
	if (gMenu.mProcessing) return;
	if (gMenu.mMode != nmain::modeMain) return;
	gMenu.mProcessing = true;
	u4 index = gMenu.mSelection0 + gMenu.mLinesSkipped * ctLinesPerScreen;
	if (index > gMenu.mFilenamesLen) goto exit;
	ustr::wcharCopy(gMenu.mFilenames[index], gMenu.mSearchString);
	SearchStringCrop();
	if (gMenu.mSearchString[0] == 0) goto exit;

	bool err;
	err = FilenamesSearch();
	if (err) goto exit;
	if (gMenu.mFilenamesSortedLen <= 1) goto exit;

	FilenamesSortedSort();

	while (gui.messagePeek())
    {
		if (gEdit.mVisible) gui.messageINeedWmChar();
		gui.messageProcess();
    }

	FilenamesReorder();

	while (gui.messagePeek())
    {
		if (gEdit.mVisible) gui.messageINeedWmChar();
		gui.messageProcess();
    }

	gMenu.Reset();

	while (gui.messagePeek())
    {
		if (gEdit.mVisible) gui.messageINeedWmChar();
		gui.messageProcess();
    }
exit:
	gMenu.mProcessing = false;
}

u4 __stdcall edit_procedure(wing::HWND wnd, u4 msg, u4 wParam, u4 lParam)
{
	//return 0; if you don't want to pass it further to def proc
	switch (msg)
	{
	case wing::WM_KEYDOWN:
		switch (wParam)
		{
		case wing::VK_RETURN:
			u4 stringLen;
			gEdit.Get(gMenu.mPathRoot, &stringLen, win::file::MAX_PATH-2); //paranoia
			if (stringLen != 0 && 
				gMenu.mPathRoot[stringLen-1] != L'\\' && 
				gMenu.mPathRoot[stringLen-1] != L'/')
			{
				gMenu.mPathRoot[stringLen] = L'\\';
				stringLen++;
				gMenu.mPathRoot[stringLen] = 0;
			}
			
			if (gMenu.mNotificationFolderUpdate != win::INVALID_HANDLE_VALUE) win::file::FindCloseChangeNotification(gMenu.mNotificationFolderUpdate);
			gMenu.mNotificationFolderUpdate = win::file::FindFirstChangeNotificationW(gMenu.mPathRoot, false, win::file::FILE_NOTIFY_CHANGE_FILE_NAME);
			gMenu.Reset();
			gEdit.Hide();
			gMenu.mMode = modeOptions;
			gui.invalidate_all();
			break;
		case wing::VK_ESCAPE:
			gEdit.Hide();
			gMenu.mMode = modeOptions;
			gui.invalidate_all();
			break;
		}
		break;
	case wing::WM_CHAR:
		//stopping the beeping sound
		if (wParam == wing::VK_RETURN) return 0;
		if (wParam == wing::VK_ESCAPE) return 0;
		//I get this (0x7f) if I press ctrl-backspace
		if (wParam == wing::VK_F16) return 0;
		break;
	}
	return wing::CallWindowProcW(gEdit.m_edit_procedure_old, wnd, msg, wParam, lParam);
}

u4 __stdcall WndProc(wing::HWND hWnd, u4 message, u4 wParam, u4 lParam)
{
	menu(hWnd, message, wParam, lParam);
    switch (message)
    {
	case wing::WM_CTLCOLOREDIT:
    {
		gEdit.OnWM_CTLCOLOREDIT(wParam);
        return null;
    }
	case wing::WM_SETFOCUS:
    {
		if (gEdit.mVisible) gEdit.Focus();
        return 0;
    }
	case wing::WM_CREATE:
		{
			gEdit.OnWmCreate(hWnd);
			break;
		}
	case wing::WM_SIZE:
		{
			gui.on_wm_size(lParam);

			gEdit.OnWmSize();

			break;
		}
    case wing::WM_DESTROY:
        wincore::ExitProcess(0);
        break;
    case wing::WM_ERASEBKGND:
		{
			return 1; //to avoid filling the whole window with black on every WM_PAINT and reduce flickering
			break;
		}
	case wing::WM_TIMER:
		{
			wing::KillTimer(gui.hWnd, 1);
			gMenu.mSelection0 = 0;
			gMenu.mLinesSkipped = 0;
			gMenu.Reset();
			DoTheThing();
			break;
		}
    default:
        return wing::DefWindowProcW(hWnd, message, wParam, lParam);
    }
    return 0;
}

u4 main() {
    __asm {
        finit
    }

	mem::MemInitGranular(&gmem0, 0, 0x02000000, 0x00020000);

	gMenu.Init();

	//registryDelete();
	registryRead();

	gui.fontInit();

	gui.init(t("files-reorder, press F1 for help"), 0x280, 0x280, WndProc);

	while (gui.messagePeek()) {
		gui.messageProcess();
	}

	gmemReentrancePoint = gmem0.used;

	main_fiStart(&gfi);

	FilenamesSort();

	gMenu.mMode = modeMain;
	gMenu.mSelectionMax = ctLinesPerScreen;
	gui.invalidate_all();

	gMenu.mNotificationFolderUpdate = win::file::FindFirstChangeNotificationW(gMenu.mPathRoot, false, win::file::FILE_NOTIFY_CHANGE_FILE_NAME);

	while (true) {
		switch (win::MsgWaitForMultipleObjects(1, &gMenu.mNotificationFolderUpdate, false, win::INFINITE, win::QS_ALLINPUT))
		{
		case win::WAIT_OBJECT_0:
			//Do something with event
			wing::SetTimer(gui.hWnd, 1, 1000, null);
			win::file::FindNextChangeNotification(gMenu.mNotificationFolderUpdate);
			break;
		case win::WAIT_OBJECT_0 + 1:

			while (gui.messagePeek())
			{
				if (gEdit.mVisible) gui.messageINeedWmChar();
				gui.messageProcess();
			}
			break;
		}
	}

	wincore::ExitProcess(0);
	
    return 0;
}

} //namespace nmain

int __stdcall wWinMain(
	u4 hInstance,
	u4 hPrevInstance,
	u4 lpCmdLine,
	u4 nShowCmd
)
{
	return nmain::main();
}

void cMenu::MainOnEnter() {
	nmain::DoTheThing();
}

void cMenu::Reset() {
	mFilenamesLen = 0;
	mFilenamesSortedLen = 0;
	gmem0.used = gmemReentrancePoint;

	gfi.error = false;

	nmain::main_fiStart(&gfi);

	nmain::FilenamesSort();

	gMenu.mSelectionMax = nmain::ctLinesPerScreen;

	gui.invalidate_all();
}

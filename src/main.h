#pragma once

#include "util/miniutil.h"

class cMenu {
public:
	bool mProcessing;
	u4 mMode;
	i4 mSelection0;
	i4 mSelectionMax;
	i4 mSelectionPrev;
	wchar* mPathRoot;
	u4      mFilenamesCap;
	u4      mFilenamesLen;
	wchar** mFilenames;
	u8*     mFilenamesTime;
	u4  mFilenamesSortedCap;
	u4  mFilenamesSortedLen;
	u4* mFilenamesSorted;
	u4* mFilenamesSortedNumber;
	i4 mLinesSkipped;
	//u4 mTimeBaseLow;
	//u4 mTimeBaseHigh;
	//u8 mTimeBase;
	wchar* mSearchString;
	wincore::HANDLE mNotificationFolderUpdate;
	bool mRegistryUse;

	
	void Init();
	//void Back();
	void MainOnEnter();
	void Reset();
};

extern cMenu gMenu;

//TODO: remove
//class Menu {
//public:
//	u4 mode;
//	i4 sel0;
//	i4 sel1;
//	i4 selprev;
//	i4 selMax;
//	u4 language;
//	u4 fixWhat;
//	u4 option0;
//	bool convert;
//	wchar* path;
//	wchar* pathLog;
//	wchar* fileExtensions;
//	u4 fileExtensionsLen;
//	u4 fileExtensionsCap;
//};
//
//extern Menu gMenu;
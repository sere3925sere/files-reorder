
#include "FileIteratorOptions.h"
#include "FileIterator.h"
#include "MemorySimple.h"
#include "../main.h"
#include "ufile.h"
#include "ui.h"
#include "miniutil.h"
//#include "uiid.h"

using namespace LogWriter;

namespace FileIteratorOptions {

void FileIteratorGetFilelist_OnStart(FileIterator::cFileIterator* fi) {

}

void FilenameAdd(const wchar* filenameIn, u4 filenameLen, u8* fileTime) {
	wchar* filenameOut = (wchar*)mem::Get((filenameLen+2)*sizeof(wchar));
	u2* lenOut = (u2*)filenameOut;
	*lenOut = filenameLen;
	filenameOut++;
	ustr::wcharCopy(filenameIn, filenameOut);
	u4 i = gMenu.mFilenamesLen;
	gMenu.mFilenamesTime[i] = *fileTime;
	gMenu.mFilenames[i] = filenameOut;
	gMenu.mFilenamesLen = i+1;
}

void FileIteratorGetFilelist_OnFile(FileIterator::cFileIterator* fi) {
	cFileIteratorGetFilelist_Data* data = (cFileIteratorGetFilelist_Data*)fi->fio->data;
	data->filesTotal++;

	wchar* filenameIn = fi->findFileData.cFileName;
	u4 filenameLen = ustr::wcharLen(filenameIn);

	u4 lo = fi->findFileData.ftCreationTime.dwLowDateTime;
	u4 hi = fi->findFileData.ftCreationTime.dwHighDateTime;
	u8 time = (u8(hi) << 32) | lo;

	FilenameAdd(filenameIn, filenameLen, &time);
}


void FileIteratorGetFilelist_OnEnd(FileIterator::cFileIterator* fi) {

}


void FileIteratorGetFilelist_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc) {

}

} //namespace FileIteratorOptions
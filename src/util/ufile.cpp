#include "ufile.h"

errorCode ufile::cFile::OpenForRead(wchar const* filepath) {
	u4 desiredAccess;
	u4 shareMode;
	u4 creationDistribution;
	desiredAccess = win::file::GENERIC_READ;
	shareMode = win::file::FILE_SHARE_READ;
	creationDistribution = win::file::OPEN_EXISTING;
	win::HANDLE file = win::file::CreateFileW(filepath, desiredAccess, shareMode, null, creationDistribution, 0, null);
	mFile = file;
	if (file == win::INVALID_HANDLE_VALUE) return true;
	return false;
}

errorCode ufile::cFile::OpenForWrite(wchar const* filepath) {
	u4 desiredAccess;
	u4 shareMode;
	u4 creationDistribution;
	desiredAccess = win::file::GENERIC_WRITE;
	shareMode = 0;
	creationDistribution = win::file::OPEN_ALWAYS;
	win::HANDLE file = win::file::CreateFileW(filepath, desiredAccess, shareMode, null, creationDistribution, 0, null);
	mFile = file;
	if (file == win::INVALID_HANDLE_VALUE) return true;
	//if (file == win::file::INVALID_HANDLE_VALUE) Error(0x14);
	return false;
}

errorCode ufile::cFile::OpenForReadWrite(wchar const* filepath) {
	u4 desiredAccess;
	u4 shareMode;
	u4 creationDistribution;
	desiredAccess = win::file::GENERIC_READ | win::file::GENERIC_WRITE;
	shareMode = win::file::FILE_SHARE_READ;
	creationDistribution = win::file::OPEN_EXISTING;
	win::HANDLE file = win::file::CreateFileW(filepath, desiredAccess, shareMode, null, creationDistribution, 0, null);
	mFile = file;
	if (file == win::INVALID_HANDLE_VALUE) return true;
	return false;
}

u4 ufile::cFile::Write(bytep buffer, u4 bufferLen, errorCode* e) {
	*e = 0;
	u4 numberOfBytesWritten;
	u4 res = win::file::WriteFile(mFile, buffer, bufferLen, &numberOfBytesWritten, null);
	//if (res == 0) Error(0x10);
	if (res == 0) *e = 0x10;
	return numberOfBytesWritten;
}

u4 ufile::cFile::Read(bytep buffer, u4 bufferLen, errorCode* e) {
	*e = 0;
	u4 numberOfBytesRead;
	u4 res = win::file::ReadFile(mFile, buffer, bufferLen, &numberOfBytesRead, null);
	//if (res == 0) Error(0x11);
	if (res == 0) *e = 0x11;
	return numberOfBytesRead;
}

void ufile::cFile::Trim() {
	win::file::SetEndOfFile(mFile);
}

void ufile::cFile::Close() {
	win::CloseHandle(mFile);
}

void ufile::cFile::GoToStart() {
	win::file::SetFilePointer(mFile, 0, 0, win::file::FILE_BEGIN);
}

void ufile::cFile::SetTime(win::file::FILETIME* pCreationTime, win::file::FILETIME* pLastAccessTime, win::file::FILETIME* pLastWriteTime) {
	win::file::SetFileTime(mFile, pCreationTime, pLastAccessTime, pLastWriteTime);
}

errorCode ufile::FileWrite(wchar const* filepath, bytep buffer, u4 bufferLen) {
	ufile::cFile file;
	errorCode e;
	e = file.OpenForWrite(filepath);
	if (e) return e;
	file.Write(buffer, bufferLen, &e);
	if (e) ErrorLazy();
	file.Trim();
	file.Close();
	return false;
}

#pragma once

#include "miniutil.h"

#include "win.h"

namespace ufile {

	class cFile
	{
	public:
		win::HANDLE mFile;
		
		//returns false on success
		errorCode OpenForRead(wchar const* filepath);
		//returns false on success
		errorCode OpenForWrite(wchar const* filepath);
		errorCode OpenForReadWrite(wchar const* filepath);
		u4 Read(bytep buffer, u4 bufferLen, errorCode* e);
		u4 Write(bytep buffer, u4 bufferLen, errorCode* e);
		void Trim();
		void Close();
		void GoToStart();
		void SetTime(win::file::FILETIME* pCreationTime, win::file::FILETIME* pLastAccessTime, win::file::FILETIME* pLastWriteTime);
	};

	errorCode FileWrite(wchar const* filepath, bytep buffer, u4 bufferLen);

}

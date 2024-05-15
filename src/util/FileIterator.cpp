#include "FileIterator.h"
#include "MemorySimple.h"
#include "miniutil.h"

//GLOBALS
class FileIterator::cFileIterator gfi;

namespace FileIterator {

void fiIterate(cFileIterator* fi, wchar_t* pathend) {
	wchar_t* pathend_prev = pathend;
    //wstring_copy(L"*", pathend);
	//ustr::wcharCopyMove
	ustr::wcharCopyMove(L"*", pathend);

	win::HANDLE searchHandle = FindFirstFileW(fi->g_filepath, &fi->findFileData);
	if (searchHandle == win::INVALID_HANDLE_VALUE) {
		//LogWriter::writerWrite(fi->w, uiid::uiidErrorFolderDoesntExist);
		fi->error = true;
		return;
	}
	if (fi->findFileData.cFileName[0] == L'.') goto next_file;
    while (true) {
        pathend = pathend_prev;
        //wstring_copy(fi->findFileData.cFileName, pathend);
        //*pathend = 0;
		ustr::wcharCopyMove(fi->findFileData.cFileName, pathend);

		if (fi->findFileData.dwFileAttributes & win::file::FILE_ATTRIBUTE_DIRECTORY) {
            *pathend++ = L'/';
            *pathend = 0;

            fiIterate(fi, pathend);
			if (fi->error) goto exit;
        }
        else {
			fi->fio->OnFile(fi);

			if (fi->error) goto exit;

			{
				//TODO
				wing::MSG msg;
				if (wing::PeekMessageW(&msg, null, 0, 0, wing::PM_REMOVE)) {
					if (msg.message == wing::WM_QUIT) wincore::ExitProcess(0);
					wing::DispatchMessageW(&msg);
				}
			}
			fi->fio->OnDraw(fi, null);
			//fiDrawProgress(fi, null);
			
        }

    next_file:
        u4 res = FindNextFileW(searchHandle, &fi->findFileData);
        if (res == 0) break;
        if (fi->findFileData.cFileName[0] == L'.') goto next_file;
    }
exit:
	win::file::FindClose(searchHandle);
}

void fiInit(cFileIterator* fi, wchar_t* path) {
	cFileIteratorOptions* fio = (cFileIteratorOptions*)mem::Get(sizeof(cFileIteratorOptions));
	fi->fio = fio;

	fi->cur = fi->g_filepath;
	//wstring_copy(path, fi->cur);
	ustr::wcharCopyMove(path, fi->cur);
}

void fiStart(cFileIterator* fi) {

	//paranoia
	if (*(fi->cur-1) != L'/' && *(fi->cur-1) != L'\\') *fi->cur++ = L'/';

	fi->fio->OnStart(fi);

	fiIterate(fi, fi->cur);

	fi->fio->OnEnd(fi);

}

} //namespace

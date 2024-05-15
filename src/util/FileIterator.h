#pragma once

#include "miniutil.h"
#include "win.h"
#include "wing.h"

//forward declarations
namespace LogWriter {
class cLogWriter;
}
//forward declarations end

namespace FileIterator {

class cFileIterator;

class cFileIteratorOptions {
public:
	void (*OnStart)(cFileIterator*);
	void (*OnFile)(cFileIterator*);
	void (*OnEnd)(cFileIterator*);
	void (*OnDraw)(cFileIterator*, wing::HDC);
	void* data;
};


class cFileIterator {
public:
	wchar_t* cur;
	u4 tick;
	static const u4 tickdelay = 1000;
	bytep filebuf1;
	u4 filebuf1size;
	bytep filebuf2;
	bool done;
	bool error;
	bool logFileWritten;
	LogWriter::cLogWriter* w;
	cFileIteratorOptions* fio;

	win::file::WIN32_FIND_DATAW findFileData;
	wchar_t g_filepath[win::file::MAX_PATH];
};

void fiInit(cFileIterator* fi, wchar_t* path);

void fiStart(cFileIterator* fi);

}

extern class FileIterator::cFileIterator gfi;
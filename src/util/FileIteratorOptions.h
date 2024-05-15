#pragma once

#include "miniutil.h"
#include "wing.h"

#include "FileIterator.h"

namespace FileIteratorOptions {

class cFileIteratorGetFilelist_Data {
public:
	u4 filesTotal;
};

void FileIteratorGetFilelist_OnStart(FileIterator::cFileIterator* fi);
void FileIteratorGetFilelist_OnFile(FileIterator::cFileIterator* fi);
void FileIteratorGetFilelist_OnEnd(FileIterator::cFileIterator* fi);
void FileIteratorGetFilelist_OnDraw(FileIterator::cFileIterator* fi, wing::HDC ahdc);

} //namespace
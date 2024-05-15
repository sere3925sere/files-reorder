#pragma once

#include "miniutil.h"

namespace mem {

class cMemorySimple {
public:
	u4 base;
	bytep used;
	bytep allocated;
	u4 reserved;
	u4 sizeGranularity;
#if _DEBUG
	bool memgetLocked;
#endif
};

bytep Get(cMemorySimple* mem, u4 size);
bytep Get(u4 size);

void MemInit(cMemorySimple* mem, u4 intended_base, u4 size);

#if _DEBUG
void memgetLock();
#endif

void MemInitGranular(cMemorySimple* mem, u4 intended_base, u4 size, u4 sizeGranularity);
bytep memgetBig(u4 size);
bytep GetBig(u4 size);
bool memgetChunk();

} //namespace

extern mem::cMemorySimple gmem0;

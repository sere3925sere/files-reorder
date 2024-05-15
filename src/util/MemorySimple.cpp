#include "MemorySimple.h"

#include "win.h"

using namespace win;
using namespace win::memory;
using namespace mem;

//GLOBALS
cMemorySimple gmem0;

//bytep mem::Get(cMemorySimple* mem, u4 size) {
//#if _DEBUG
//	if (mem->memgetLocked) Error(0x4);
//#endif
//	bytep res = mem->used;
//	mem->used = mem->used + size;
//	if (mem->used > mem->allocated) {
//		ErrorLazy();
//	}
//	util::u1memzero(res, size);
//	//memset(res, 0, size); //paranoia?
//	return res;
//}

bytep mem::Get(u4 size) {
#if _DEBUG
	if (gmem0.memgetLocked) Error(0x4);
#endif
	bytep res = gmem0.used;
	gmem0.used = gmem0.used + size;
	if (gmem0.used > gmem0.allocated) {
		mem::memgetChunk();
		//ErrorLazy();
	}
	util::u1memzero(res, size);
	//memset(res, 0, size); //paranoia?
	return res;
}



#if _DEBUG
void mem::memgetLock() {
	gmem0.memgetLocked = true;
}
#endif

void MemInit(cMemorySimple* mem, u4 intended_base, u4 size) {
	
try_again:

	char* base = (char*)VirtualAlloc(
		(bytep)intended_base,
		size,
		MEM_RESERVE,
		PAGE_NOACCESS);

	if (base == 0) {
		intended_base += size;
		if (intended_base >= 0x70000000) ErrorLazy(); //("VirtualAlloc really failed");
		goto try_again;
	}

	VirtualAlloc(
		(bytep)base,
		size,
		MEM_COMMIT,
		PAGE_READWRITE);

	mem->base = u4(base);
	mem->used = (bytep)base;
	mem->allocated = bytep(base) + size;
	mem->reserved = size;
	return;

}

void mem::MemInitGranular(cMemorySimple* mem, u4 intended_base, u4 size, u4 sizeGranularity) {
	mem->sizeGranularity = sizeGranularity;
	
try_again:

	char* base = (char*)VirtualAlloc(
		(bytep)intended_base,
		size,
		MEM_RESERVE,
		PAGE_NOACCESS);

	if (base == 0) {
		intended_base += size;
		if (intended_base >= 0x70000000) ErrorLazy();//("VirtualAlloc really failed");
		goto try_again;
	}

	VirtualAlloc(
		(bytep)base,
		sizeGranularity,
		MEM_COMMIT,
		PAGE_READWRITE);

	mem->base = u4(base);
	mem->used = (bytep)base;
	mem->allocated = bytep(base) + sizeGranularity;
	//^ Where next MEM_COMMIT should happen
	mem->reserved = size - sizeGranularity;
	//^ How much we have in reserve
	return;

}

//TODO: delete this later
//bytep mem::memgetBig(u4 size) {
//	if (gmem0.reserved < size) Error(0x5);
//
//	VirtualAlloc(
//		(bytep)gmem0.allocated,
//		size,
//		MEM_COMMIT,
//		PAGE_READWRITE);
//
//	bytep res = gmem0.used;
//	gmem0.used += size;
//	gmem0.allocated += size;
//	gmem0.reserved -= size;
//	return res;
//}

//unlike memgetBig, this one will not recommit an already committed memory
bytep mem::GetBig(u4 size) {
	if (gmem0.reserved < size) Error(0x5);

	//TODO: < or <=?
	if (gmem0.allocated + gmem0.reserved < gmem0.used + size) {
		ErrorLazy(); //TODO: not done yet
	}
	//something like this?

	VirtualAlloc(
		(bytep)gmem0.allocated,
		size,
		MEM_COMMIT,
		PAGE_READWRITE);

	bytep res = gmem0.used;
	gmem0.used += size;
	gmem0.allocated += size;
	gmem0.reserved -= size;
	return res;
}

//returns true on error
bool mem::memgetChunk() {
	if (gmem0.reserved < gmem0.sizeGranularity) {
		Error(0x7);
		//return true;
	}
	VirtualAlloc(
		(bytep)gmem0.allocated,
		gmem0.sizeGranularity,
		MEM_COMMIT,
		PAGE_READWRITE);

	//gmem3.used += gmem3.sizeGranularity;
	gmem0.allocated += gmem0.sizeGranularity;
	gmem0.reserved -= gmem0.sizeGranularity;
	return false;
}
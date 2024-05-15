#include "wing.h"

using namespace wing;

//#define RGB(r,g,b)          ((COLORREF)(((u1)(r)|((u2)((u1)(g))<<8))|(((u4)(u1)(b))<<16)))

COLORREF wing::RGB(u1 r, u1 g, u1 b) {
	u4 res = r | u2(g << 8) | u4(b<< 16);
	return (COLORREF)res;
}
#include "SymbolParameters.h"

SymbolParams GetParamsSymbol(_In_ HWND hWnd)
{
	SymbolParams	size;
	TEXTMETRIC		tm;
	memset(&tm, 0, sizeof(TEXTMETRIC));
	HDC				hdc;
	memset(&hdc, 0, sizeof(HDC));
	hdc = GetDC(hWnd);

	GetTextMetrics(hdc, &tm);

	size.cxChar = tm.tmAveCharWidth;
	size.cyChar = tm.tmHeight + tm.tmExternalLeading;
	size.cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * size.cxChar / 2 + 3;
	size.cyHeightButton = 7 * size.cyChar / 4;

	ReleaseDC(hWnd, hdc);

	return size;
}
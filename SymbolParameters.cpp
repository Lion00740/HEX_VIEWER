#include "SymbolParameters.h"

SymbolParams GetParamsSymbol(_In_ HWND hWnd)
{
	SymbolParams	size;
	TEXTMETRIC		tm;
	HDC				hdc;

	ZeroMemory(&size, sizeof(SymbolParams));
	ZeroMemory(&tm, sizeof(TEXTMETRIC));
	ZeroMemory(&hdc, sizeof(HDC));

	hdc = GetDC(hWnd);

	GetTextMetrics(hdc, &tm);

	size.cxChar = tm.tmAveCharWidth;
	size.cyChar = tm.tmHeight + tm.tmExternalLeading;
	size.cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * size.cxChar / 2 + 3;
	size.cyHeightButton = 7 * size.cyChar / 4;

	ReleaseDC(hWnd, hdc);

	return size;
}
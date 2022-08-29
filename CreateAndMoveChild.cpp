#include "CreateAndMoveChild.h"

void CreateChildWindow(_In_ HWND hWnd, _Inout_ hWndChildWindows* hWndStruct)
{
	// Инициализация дочерних окон
	for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
	{
		hWndStruct[i].hWndChild = CreateWindowEx(
			WS_EX_COMPOSITED,
			L"HEX_VIEWER_CHILD",
			NULL,
			WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | WS_BORDER,
			0, 0, 0, 0,
			hWnd,
			(HMENU)(0 + i),
			(HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWndStruct[i].hWndButton = CreateWindowEx(
			0,
			L"button", 
			L">>",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER,
			0, 0, 0, 0,
			hWnd,
			(HMENU)(2 + i),
			(HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
			NULL
		);

		hWndStruct[i].hWndEdit = CreateWindowEx(
			0,
			L"edit",
			L"",
			WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT,
			0, 0, 0, 0,
			hWnd,
			(HMENU)(4 + i),
			(HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE),
			NULL
		);
	}
}

void MoveChildWindow(_In_ HWND hWnd, _Inout_ hWndChildWindows* hWndStruct, _In_ SymbolParams SizeSymbol)
{
	RECT	rect;
	memset(&rect, 0, sizeof(RECT));
	INT		cxBlock = 0;
	INT		cyBlock = 0;

	GetClientRect(hWnd, &rect);

	cxBlock = rect.right / 2;
	cyBlock = rect.bottom;

	for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
	{
		MoveWindow(hWndStruct[i].hWndChild, cxBlock * i, SizeSymbol.cyHeightButton, cxBlock, cyBlock - SizeSymbol.cyHeightButton, TRUE);

		MoveWindow(hWndStruct[i].hWndButton, cxBlock * (i + 1) - 4 * SizeSymbol.cxCaps, 0, 4 * SizeSymbol.cxCaps, SizeSymbol.cyHeightButton, TRUE);

		MoveWindow(hWndStruct[i].hWndEdit, cxBlock * i, 0, cxBlock - 4 * SizeSymbol.cxCaps, SizeSymbol.cyHeightButton, TRUE);
	}
}
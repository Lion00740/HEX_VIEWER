#include "Scroll.h"

void ScrollEventsControl(_In_ WPARAM wParam, _In_ HWND hWnd, _Inout_ hWndChildWindows* hWndStruct)
{
	UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	RECT				rect;
	memset(&rect, 0, sizeof(RECT));
	VScroll* Vscroll = &UserData->Vscroll;

	GetClientRect(hWnd, &rect);

	switch (LOWORD(wParam))
	{
	case SB_LINEUP:
		Vscroll->iVscrollInc = -1;
		break;
	case SB_LINEDOWN:
		Vscroll->iVscrollInc = 1;
		break;
	case SB_PAGEUP:
		Vscroll->iVscrollInc = min(-1, -(rect.bottom - UserData->SizeSymbol.cyHeightButton) / UserData->SizeSymbol.cyChar);
		break;
	case SB_PAGEDOWN:
		Vscroll->iVscrollInc = max(1, (rect.bottom - UserData->SizeSymbol.cyHeightButton) / UserData->SizeSymbol.cyChar);
		break;
	case SB_THUMBTRACK:
		Vscroll->iVscrollInc = HIWORD(wParam) - Vscroll->iVscrollPos;
		break;
	default:
		Vscroll->iVscrollInc = 0;
	}

	Vscroll->iVscrollInc = max(-Vscroll->iVscrollPos, min(Vscroll->iVscrollInc, Vscroll->iVscrollMax - Vscroll->iVscrollPos));

	if (Vscroll->iVscrollInc != 0)
	{

		Vscroll->iVscrollPos += Vscroll->iVscrollInc;
		for (INT i = 0; i < 2; i++)
		{
			ScrollWindow(hWndStruct[i].hWndChild, 0, -UserData->SizeSymbol.cyChar * Vscroll->iVscrollInc, NULL, NULL);
			SetScrollPos(hWndStruct[i].hWndChild, SB_VERT, Vscroll->iVscrollPos, TRUE);
			UpdateWindow(hWndStruct[i].hWndChild);
		}
	}
}

void SetScrollBySize(_In_ HWND hWnd, _In_ INT cyClient, _Inout_ UserDataStruct* UserData)
{
	UserData->Vscroll.iVscrollMax = max(0, int(UserData->ullMaxNumLines - cyClient / UserData->SizeSymbol.cyChar));
	UserData->Vscroll.iVscrollPos = min(UserData->Vscroll.iVscrollPos, UserData->Vscroll.iVscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, UserData->Vscroll.iVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, UserData->Vscroll.iVscrollPos, TRUE);
}
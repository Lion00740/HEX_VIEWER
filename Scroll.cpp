#include "Scroll.h"
#include "FileHandler.h"

void ScrollEventsControl(_In_ WPARAM wParam, _In_ HWND hWnd, _Inout_ hWndChildWindows* hWndStruct)
{
	UserDataStruct*		UserData	= (UserDataStruct*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	RECT				rect;
	VScroll*			Vscroll		= &UserData->Vscroll;

	ZeroMemory(&rect, sizeof(RECT));

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
	{
		SCROLLINFO slif;
		ZeroMemory(&slif, sizeof(slif));
		slif.cbSize = sizeof(SCROLLINFO);
		slif.fMask = SIF_TRACKPOS;
		
		// TODO ERRROR

		/*for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
		{
			GetScrollInfo(hWndStruct[i].hWndChild, SB_VERT, &slif);
		}*/
		GetScrollInfo(hWndStruct[0].hWndChild, SB_VERT, &slif);
		
		Vscroll->iVscrollInc = slif.nTrackPos - Vscroll->iVscrollPos;
		if (Vscroll->iVscrollInc > 0) {
			printf("");
		}
		break;
	}
	default:
		Vscroll->iVscrollInc = 0;
	}

	Vscroll->iVscrollInc = max(-Vscroll->iVscrollPos, min(Vscroll->iVscrollInc, Vscroll->iVscrollMax - Vscroll->iVscrollPos));

	if (Vscroll->iVscrollInc != 0)
	{

		Vscroll->iVscrollPos += Vscroll->iVscrollInc;
		UserData->ullIncrement = (ULONGLONG)Vscroll->iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE;
		for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
		{
			ScrollWindow(hWndStruct[i].hWndChild, 0, -UserData->SizeSymbol.cyChar * Vscroll->iVscrollInc, NULL, NULL);
			SetScrollPos(hWndStruct[i].hWndChild, SB_VERT, Vscroll->iVscrollPos, TRUE);
			UpdateWindow(hWndStruct[i].hWndChild);
		}
	}

	for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
	{
		if (UserData->FileBuf[i].ullSizeBuffer > (ULONGLONG)UserData->FileBuf[i].uiGranularity * 2 
			/*&& Vscroll->iVscrollPos < (UserData->FileBuf[i].ullSizeBuffer / NUMBER_OF_SYMBOLS_PER_LINE)*/)
		{
			MoveToMap(&UserData->FileBuf[i], UserData->ullIncrement, *Vscroll);
		}
	}

	if (Vscroll->iVscrollInc < 0 || Vscroll->iVscrollInc > Vscroll->iVscrollMax)
	{
		printf("");
	}
}

void SetScrollBySize(_In_ HWND hWnd, _In_ INT cyClient, _Inout_ UserDataStruct* UserData)
{
	UserData->Vscroll.iVscrollMax = max(0, int(UserData->ullMaxNumLines - cyClient / UserData->SizeSymbol.cyChar));
	UserData->Vscroll.iVscrollPos = min(UserData->Vscroll.iVscrollPos, UserData->Vscroll.iVscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, UserData->Vscroll.iVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, UserData->Vscroll.iVscrollPos, TRUE);
}
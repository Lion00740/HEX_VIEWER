#include "PaintText.h"

void PaintAllLine(_In_ HWND hWnd, _In_ UserDataStruct UserData, _In_ INT ID)
{
	LPCSTR			TextBuffer		= UserData.FileBuf[ID].lpcBuffer + ((ULONGLONG)UserData.Vscroll.iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	ULONGLONG		ullCount		= -1 + ((ULONGLONG)UserData.Vscroll.iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	PAINTSTRUCT		ps;
	memset(&ps, 0, sizeof(PAINTSTRUCT));
	HDC				hdc;
	memset(&hdc, 0, sizeof(HDC));
	RECT			rect;
	memset(&rect, 0, sizeof(RECT));

	hdc = BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rect);
	
	INT				iPaintBeg		= max(-1, UserData.Vscroll.iVscrollPos + rect.top / UserData.SizeSymbol.cyChar - 1);
	INT				iPaintEnd		= min(int(UserData.ullMaxNumLines), UserData.Vscroll.iVscrollPos + rect.bottom / UserData.SizeSymbol.cyChar);

	// вывод всех строк 
	for (INT iteration = iPaintBeg; iteration < iPaintEnd; iteration++)
	{
		INT y = UserData.SizeSymbol.cyChar * (iteration - UserData.Vscroll.iVscrollPos + 1);

		PaintOffset(hdc, UserData, iteration, y);
		
		PaintLine(hdc, &TextBuffer, &ullCount, UserData, ID, y);
	}

	EndPaint(hWnd, &ps);
}

VOID PaintLine(_In_ HDC hdc, _In_ LPCSTR* TextBuffer, _Inout_ ULONGLONG* ullCount, _In_ UserDataStruct UserData, _In_ INT ID, _In_ INT y)
{
	SymbolParams SizeSymbol = UserData.SizeSymbol;

	for (INT i = 0; i < NUMBER_OF_SYMBOLS_PER_LINE; i++)
	{
		(*ullCount)++;
		if (*ullCount >= UserData.FileBuf[ID].ullSizeBuffer)
		{
			break;
		}
		
		INT iSymbol = (UCHAR)**TextBuffer;
		for (INT j = 0; j < NUMBER_OF_WINDOW; j++) // цикл отвечающий за сравнение файлов
		{
			if (*ullCount > UserData.FileBuf[j].ullSizeBuffer)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				break;
			}
			INT iAnotherSymbol = (UCHAR)*(UserData.FileBuf[j].lpcBuffer + *ullCount);
			if (iAnotherSymbol != iSymbol) 
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				break;
			}
		}

		CHAR	cHexBuf[SIZE_HEX_BUF]	= { 0 };
		INT		iLenght					= sprintf(cHexBuf, "%02X", iSymbol);

		TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * i, y, cHexBuf, iLenght); // вывод 16-ричного представления

		if (iswprint(iSymbol) == 0) // вывод символов
		{
			TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * i - SizeSymbol.cxCaps, y, (LPCSTR)".", 1);
		}
		else
		{
			TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * i - SizeSymbol.cxCaps, y, (LPCSTR)&iSymbol, 1);
		}

		SetTextColor(hdc, RGB(0, 0, 0));
		(*TextBuffer)++;
	}
}

void PaintOffset(_In_ HDC hdc, _In_ UserDataStruct UserData, _In_ INT iteration, _In_ INT y) // функция для подсчета смещения и его вывода
{
	CHAR	cOffsetBuf[SIZE_OFFSET_BUF]		= { 0 };
	INT		iLenght							= sprintf(cOffsetBuf, "%08X :", (iteration + 1) * NUMBER_OF_SYMBOLS_PER_LINE);

	TextOutA(hdc, 0, y, cOffsetBuf, iLenght);
	TextOutA(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * NUMBER_OF_SYMBOLS_PER_LINE, y, (LPCSTR)"|", 1);
}
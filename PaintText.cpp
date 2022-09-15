#include "PaintText.h"

void PaintAllLine(_In_ HWND hWnd, _In_ UserDataStruct UserData, _In_ INT ID)
{
	PBYTE			TextBuffer				= UserData.FileBuf[ID].pbBuffer;
	ULONGLONG		ullNumberPrintSimbol	= -1 + ((ULONGLONG)UserData.Vscroll.iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	PAINTSTRUCT		ps;
	HDC				hdc;
	RECT			rect;

	ZeroMemory(&ps, sizeof(PAINTSTRUCT));
	ZeroMemory(&hdc, sizeof(HDC));
	ZeroMemory(&rect, sizeof(RECT));
	
	hdc = BeginPaint(hWnd, &ps);
	GetClientRect(hWnd, &rect);
	
	INT				iPaintBeg		= max(-1, UserData.Vscroll.iVscrollPos + rect.top / UserData.SizeSymbol.cyChar - 1);
	INT				iPaintEnd		= min(INT(UserData.ullMaxNumLines) - 1, UserData.Vscroll.iVscrollPos + rect.bottom / UserData.SizeSymbol.cyChar);

	// вывод всех строк 
	for (INT iteration = iPaintBeg; iteration < iPaintEnd; iteration++)
	{
		INT y = UserData.SizeSymbol.cyChar * (iteration - UserData.Vscroll.iVscrollPos + 1);

		PaintOffset(hdc, UserData, ullNumberPrintSimbol / NUMBER_OF_SYMBOLS_PER_LINE, y);
		
		PaintLine(hdc, TextBuffer, &ullNumberPrintSimbol, UserData, ID, y, UserData.ullIncrement);

		UserData.ullIncrement += NUMBER_OF_SYMBOLS_PER_LINE;
	}

	EndPaint(hWnd, &ps);
}

VOID PaintLine(_In_ HDC hdc, _In_ PBYTE TextBuffer, _Inout_ ULONGLONG* ullCount, _In_ UserDataStruct UserData, _In_ INT ID, _In_ INT y, ULONGLONG increment)
{
	SymbolParams SizeSymbol = UserData.SizeSymbol;

	for (ULONGLONG i = 0; i < NUMBER_OF_SYMBOLS_PER_LINE; i++)
	{
		(*ullCount)++;
		if (*ullCount >= UserData.FileBuf[ID].ullSizeBuffer)
		{
			break;
		}

		// TODO Условие ловушка

		if ((increment + UserData.FileBuf->ullCurrentOffset + i) > UserData.FileBuf->uiGranularity) 
		{
			printf("");
		}
		
		USHORT iSymbol = (TCHAR)TextBuffer[i + increment - UserData.FileBuf->ullCurrentOffset]; // здесь надо будет проверить
		
		for (INT j = 0; j < NUMBER_OF_WINDOW; j++) // Цикл отвечающий за сравнение файлов.
		{
			if (*ullCount > UserData.FileBuf[j].ullSizeBuffer)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				break;
			}

			if (i + increment - UserData.FileBuf->ullCurrentOffset > UserData.FileBuf->uiGranularity) 
			{
				printf("");
			}

			USHORT iAnotherSymbol = (TCHAR)UserData.FileBuf[j].pbBuffer[i + increment - UserData.FileBuf->ullCurrentOffset];
			if (iAnotherSymbol != iSymbol)
			{
				SetTextColor(hdc, RGB(255, 0, 0));
				break;
			}
		}

		TCHAR	cHexBuf[SIZE_HEX_BUF]	= { 0 };
		INT		iLenght					= swprintf(cHexBuf, SIZE_HEX_BUF, L"%02X", iSymbol);

		TextOut( // Вывод 16-ричного представления.
			hdc, 
			SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (INT)i,
			y, 
			cHexBuf, 
			iLenght
		); 

		// Вывод символов.
		if (iswprint(iSymbol) == 0)
		{
			TextOut( // Выводит точку если символ является не читаемым.
				hdc, 
				SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * (INT)i - SizeSymbol.cxCaps, 
				y, 
				L".", 
				1
			);
		}
		else
		{
			TextOut( // Выводит сам символ если он является читаемым.
				hdc, 
				SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * (INT)i - SizeSymbol.cxCaps, 
				y,
				(LPCWSTR)&iSymbol, 
				1
			);
		}
		SetTextColor(hdc, RGB(0, 0, 0));
	}
}

void PaintOffset(_In_ HDC hdc, _In_ UserDataStruct UserData, _In_ INT iteration, _In_ INT y) // функция для подсчета смещения и его вывода
{
	TCHAR	cOffsetBuf[SIZE_OFFSET_BUF]		= { 0 };
	INT		iLenght							= swprintf(cOffsetBuf, SIZE_OFFSET_BUF, L"%09X :", (iteration + 1) * NUMBER_OF_SYMBOLS_PER_LINE);
	
	TextOut(hdc, 0, y, cOffsetBuf, iLenght);
	TextOut(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * NUMBER_OF_SYMBOLS_PER_LINE, y, L"|", 1);
}
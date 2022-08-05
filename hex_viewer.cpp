#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define CURSOR_CLIKED_MENU 1
#define CURSOR_CLIKED_EXIT 2
#define NUMBER_OF_SYMBOLS_PER_LINE 16
#define SIZE_HEX_BUF 3
#define SIZE_OFFSET_BUF 11

LPCSTR g_lpcBuffer;
UINT64 g_ullSizelpcBuffer;

struct TextSize
{
	int cxChar, cyChar, cxCaps;
};

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void WndMenu(HWND hWnd);
bool ReadFromFiles(LPWSTR path);
bool OpenFile(HWND hWnd, UINT64* ullNumLines);
void PaintText(HWND hWnd, int cyClient, TextSize SizeSymbol, int iVscrollPos, UINT64 ullNumLines);
void SetScrollBySize(HWND hWnd, int cyClient, int cyChar, int* iVscrollPos, int* iVscrollMax, UINT64* ullNumLines);
TextSize GetSizeSymbol(HWND hWnd);

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int iCmdShow)
{
	HWND hWnd;
	MSG msg;
	WCHAR AppName[] = L"HEX_VIEWER";
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance = hInst;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = AppName;

	if (!RegisterClassEx(&wcex)) 
	{
		return EXIT_FAILURE;
	}

	wcex.lpfnWndProc = ChildWndProc;
	wcex.cbWndExtra = sizeof(WORD);
	wcex.hIcon = NULL;
	wcex.lpszClassName = L"HEX_VIEWER_CHILD";
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
	{
		return EXIT_FAILURE;
	}

	hWnd = CreateWindow(AppName, L"HEX_VIEWER", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wcex.hInstance, NULL);

	if (!hWnd) 
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HWND hWndChild, hWndButton;
	static TextSize SizeSymbol;
	HDC hdc;
	int cxBlock, cyBlock;
	switch (iMsg) 
	{
	case WM_CREATE:

		SizeSymbol = GetSizeSymbol(hWnd);
		
		hWndChild = CreateWindow(L"HEX_VIEWER_CHILD", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)5, (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE), NULL);

		hWndButton = CreateWindow(L"button", L">>", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, (HMENU)1, ((LPCREATESTRUCT)lParam)->hInstance, NULL);

		if (!hWndChild || !hWndButton)
		{
			return EXIT_FAILURE;
		}

		WndMenu(hWnd);

		return 0;
	case WM_SIZE:

		cxBlock = LOWORD(lParam) / 2;
		cyBlock = HIWORD(lParam);

		MoveWindow(hWndButton, cxBlock, 0, 4 * SizeSymbol.cxCaps, 7 * SizeSymbol.cyChar / 4, TRUE);
		MoveWindow(hWndChild, 0, 60, cxBlock, cyBlock - 60, TRUE);

		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		/*case CURSOR_CLIKED_MENU:
			if (!OpenFile(hWnd, &ullNumLines))
			{
				break;
			}
			SetScrollBySize(hWnd, cyClient, SizeSymbol.cyChar, &iVscrollPos, &iVscrollMax, &ullNumLines);
			break;*/
		case CURSOR_CLIKED_EXIT:
			if (g_lpcBuffer != NULL)
			{
				UnmapViewOfFile(g_lpcBuffer);
				g_lpcBuffer = NULL;
			}
			PostQuitMessage(0);
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static TextSize SizeSymbol;
	int iVscrollInc;
	static int cxChar, cyChar, cxCaps, cyClient, iVscrollMax, iVscrollPos;
	static UINT64 ullNumLines;

	//GetWindowLong

	switch (iMsg)
	{
	case WM_CREATE:
		
		SizeSymbol = GetSizeSymbol(hWnd);
		
		return 0;
	case WM_SIZE:

		cyClient = HIWORD(lParam);

		SetScrollBySize(hWnd, cyClient, SizeSymbol.cyChar, &iVscrollPos, &iVscrollMax, &ullNumLines);

		return 0;
	case WM_VSCROLL:
		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			iVscrollInc = -1;
			break;
		case SB_LINEDOWN:
			iVscrollInc = 1;
			break;
		case SB_PAGEUP:
			iVscrollInc = min(-1, -cyClient / SizeSymbol.cyChar);
			break;
		case SB_PAGEDOWN:
			iVscrollInc = max(1, cyClient / SizeSymbol.cyChar);
			break;
		case SB_THUMBTRACK:
			iVscrollInc = HIWORD(wParam) - iVscrollPos;
			break;
		default:
			iVscrollInc = 0;
		}

		iVscrollInc = max(-iVscrollPos, min(iVscrollInc, iVscrollMax - iVscrollPos));

		if (iVscrollInc != 0)
		{
			iVscrollPos += iVscrollInc;
			ScrollWindow(hWnd, 0, -SizeSymbol.cyChar * iVscrollInc, NULL, NULL);
			//SetScrollPos flag = erase
			SetScrollPos(hWnd, SB_VERT, iVscrollPos, TRUE);
			UpdateWindow(hWnd);
		}

		return 0;
	case WM_PAINT:
		if (g_lpcBuffer != NULL)
		{
			PaintText(hWnd, cyClient, SizeSymbol, iVscrollPos, ullNumLines);
		}
		return 0;
	case WM_DESTROY:
		if (g_lpcBuffer != NULL)
		{
			UnmapViewOfFile(g_lpcBuffer);
			g_lpcBuffer = NULL;
		}
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
	return 0;
}

void WndMenu(HWND hWnd)
{
	HMENU RootMenu = CreateMenu();
	HMENU SubMenu = CreatePopupMenu();

	AppendMenu(RootMenu, MF_POPUP, (UINT_PTR)SubMenu, L"File");
	AppendMenu(SubMenu, MF_STRING, CURSOR_CLIKED_MENU, L"Open file...");
	AppendMenu(SubMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(SubMenu, MF_STRING, CURSOR_CLIKED_EXIT, L"Exit");

	SetMenu(hWnd, RootMenu);
}

bool OpenFile(HWND hWnd, UINT64* ullNumLines) 
{
	if (g_lpcBuffer != NULL) 
	{
		UnmapViewOfFile(g_lpcBuffer);
		g_lpcBuffer = NULL;
	}

	RECT rect;
	OPENFILENAMEW OpFileName;
	WCHAR FileName[MAX_PATH] = {0};

	ZeroMemory(&OpFileName, sizeof(OpFileName));
	OpFileName.lStructSize = sizeof(OpFileName);
	OpFileName.hwndOwner = hWnd;
	OpFileName.lpstrFile = FileName;
	OpFileName.nMaxFile = sizeof(FileName);
	OpFileName.lpstrFilter = L"*.*";
	OpFileName.lpstrFileTitle = NULL;
	OpFileName.nMaxFileTitle = 0;
	OpFileName.lpstrInitialDir = L"E:\\Work\\HEX_VIEWER";
	OpFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameW(&OpFileName))
	{
		if (!ReadFromFiles(FileName))
		{
			MessageBoxW(hWnd, L"File could not open!", L"ERROR", MB_OK);
			return false;
		}
	}
	long double ldNumLines = ceill(long double(g_ullSizelpcBuffer) / NUMBER_OF_SYMBOLS_PER_LINE);
	*ullNumLines = (UINT64)ldNumLines;
	GetClientRect(hWnd, &rect);
	InvalidateRect(hWnd, &rect, TRUE);
	return true;
}

bool ReadFromFiles(LPWSTR path)
{
	HANDLE FileToRead = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	if (FileToRead == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	HANDLE hFileMap = CreateFileMapping(FileToRead, NULL, PAGE_READONLY, 0, 0, NULL);

	if (hFileMap == NULL)
	{
		CloseHandle(FileToRead);
		return false;
	}

	PVOID MappedMemory = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, NULL);
	g_lpcBuffer = (LPCSTR)MappedMemory;

	LARGE_INTEGER liSize;
	GetFileSizeEx(FileToRead, &liSize);
	g_ullSizelpcBuffer = liSize.QuadPart;

	CloseHandle(hFileMap);
	CloseHandle(FileToRead);
	return true;
}

void PaintText(HWND hWnd, int cyClient, TextSize SizeSymbol, int iVscrollPos, UINT64 ullNumLines)
{
	LPCSTR TextBuffer = g_lpcBuffer + ((UINT64)iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	HDC hdc;
	PAINTSTRUCT ps;
	int y;
	UINT64 ullCount = -1 + ((UINT64)iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	char cHexBuf[SIZE_HEX_BUF] = { 0 };
	char cOffsetBuf[SIZE_OFFSET_BUF] = { 0 };
	hdc = BeginPaint(hWnd, &ps);
	RECT rect;
	GetClientRect(hWnd, &rect);
	
	int iPaintBeg = max(-1, iVscrollPos + rect.top / SizeSymbol.cyChar - 1);
	int iPaintEnd = min(int(ullNumLines), iVscrollPos + rect.bottom / SizeSymbol.cyChar);

	for (int iteration = iPaintBeg; iteration < iPaintEnd; iteration++)
	{
		y = SizeSymbol.cyChar * (iteration - iVscrollPos + 1);
		int iLenght = sprintf(cOffsetBuf, "%08X :", (iteration + 1) * NUMBER_OF_SYMBOLS_PER_LINE);

		TextOutA(hdc, 0, y, cOffsetBuf, iLenght);
		for (int i = 0; i < NUMBER_OF_SYMBOLS_PER_LINE; i++)
		{
			ullCount++; // ���� � ��������� ������ ��� 16 �������� ������� ������ �����
			if (ullCount == g_ullSizelpcBuffer)
			{
				break;
			}
			int iSymbol = (unsigned char)*TextBuffer;
			int iLenght = sprintf(cHexBuf, "%02X", iSymbol);
			TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * i, y, cHexBuf, iLenght);

			if (iswprint(iSymbol) == 0)
			{
				TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * i - SizeSymbol.cxCaps, y, (LPCSTR)".", 1);
			}
			else
			{
				TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + SizeSymbol.cxCaps * i - SizeSymbol.cxCaps, y, TextBuffer, 1);
			}
			TextBuffer++;
		}
		TextOutA(hdc, SizeSymbol.cxChar * SIZE_OFFSET_BUF + SizeSymbol.cxChar * SIZE_HEX_BUF * NUMBER_OF_SYMBOLS_PER_LINE, y, (LPCSTR)"|", 1);
		
		if (ullCount == g_ullSizelpcBuffer)
		{
			break;
		}
	}
	EndPaint(hWnd, &ps);
}

void SetScrollBySize(HWND hWnd, int cyClient, int cyChar, int* iVscrollPos, int* iVscrollMax, UINT64* ullNumLines)
{
	*iVscrollMax = max(0, int(*ullNumLines - cyClient / cyChar));
	*iVscrollPos = min(*iVscrollPos, *iVscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, *iVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, *iVscrollPos, TRUE);
}

TextSize GetSizeSymbol(HWND hWnd)
{
	TextSize size;
	TEXTMETRIC tm;
	HDC hdc;

	hdc = GetDC(hWnd);

	GetTextMetrics(hdc, &tm);
	size.cxChar = tm.tmAveCharWidth;
	size.cyChar = tm.tmHeight + tm.tmExternalLeading;
	size.cxCaps = (tm.tmPitchAndFamily & 1 ? 3 : 2) * size.cxChar / 2 + 3;

	ReleaseDC(hWnd, hdc);

	return size;
}

// ��������� ������ � ������ ������ 
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define NUMBER_OF_SYMBOLS_PER_LINE 16
#define SIZE_HEX_BUF 3
#define SIZE_OFFSET_BUF 11
#define NUMBER_OF_WINDOW 2

struct FileBuffer
{
	LPCSTR lpcBuffer = NULL;
	UINT64 ullSizeBuffer = 0;
	UINT64 ullNumLines = 0;
};

struct hWndChildWindow
{
	HWND hWndChild;
	HWND hWndButton;
	HWND hWndEdit;
};

struct TextSize
{
	int cxChar = 0, cyChar = 0, cxCaps = 0;
	int nHeightButton = 0;
};

struct VScroll
{
	int iVscrollMax = 0;
	int iVscrollPos = 0;
	int iVscrollInc = 0;
};

struct UserDataStruct
{
	TextSize SizeSymbol;
	VScroll Vscroll;
	FileBuffer FileBuf[NUMBER_OF_WINDOW];
	UINT64 ullMaxNumLines = 0;
};

LRESULT CALLBACK	WndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
LRESULT CALLBACK	ChildWndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam);
BOOL				ReadFromFiles(LPWSTR path, FileBuffer *FileBuf);
BOOL				OpenFileParams(HWND hWnd, FileBuffer *FileBuf, HWND hWndEdit);
void				PaintText(HWND hWnd, UserDataStruct UserData, int ID);
void				SetScrollBySize(HWND hWnd, int cyClient, UserDataStruct* UserData);
TextSize			GetSizeSymbol(HWND hWnd);
FileBuffer			FileInfo(HANDLE hFileMap, HANDLE FileToRead);
void				ClearBuffer(FileBuffer* FileBuf);
void				CreateChildWindow(HWND hWnd, hWndChildWindow* hWndStruct);
void				MoveChildWindow(HWND hWnd, hWndChildWindow* hWndStruct, TextSize SizeSymbol);

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
	wcex.cbWndExtra = sizeof(DWORD);
	wcex.hIcon = NULL;
	wcex.lpszClassName = L"HEX_VIEWER_CHILD";
	wcex.hIconSm = NULL;

	if (!RegisterClassEx(&wcex))
	{
		return EXIT_FAILURE;
	}

	hWnd = CreateWindowEx(0, AppName, L"HEX_VIEWER", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wcex.hInstance, NULL);

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

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static UserDataStruct UserData;
	static hWndChildWindow hWndStruct[NUMBER_OF_WINDOW];
	
	switch (iMsg) 
	{
	case WM_CREATE:
		
		UserData.SizeSymbol = GetSizeSymbol(hWnd);
		
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)&UserData);

		CreateChildWindow(hWnd, hWndStruct);

		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case 2:
			
			if (!OpenFileParams(hWnd, &UserData.FileBuf[0], hWndStruct[0].hWndEdit))
			{
				break;
			}

			SendMessage(hWndStruct[0].hWndChild, WM_COMMAND, wParam, lParam);
			break;
		case 3:
			
			if (!OpenFileParams(hWnd, &UserData.FileBuf[1], hWndStruct[1].hWndEdit))
			{
				break;
			}

			SendMessage(hWndStruct[1].hWndChild, WM_COMMAND, wParam, lParam);
			break;
		}

		return 0;
	case WM_SIZE: 
	{
		MoveChildWindow(hWnd, hWndStruct, UserData.SizeSymbol);
		
		return 0; 
	}
	case WM_VSCROLL: 
	{
		RECT rect;
		VScroll* Vscroll = &UserData.Vscroll;

		GetClientRect(hWndStruct[0].hWndChild, &rect);

		switch (LOWORD(wParam))
		{
		case SB_LINEUP:
			Vscroll->iVscrollInc = -1;
			break;
		case SB_LINEDOWN:
			Vscroll->iVscrollInc = 1;
			break;
		case SB_PAGEUP:
			Vscroll->iVscrollInc = min(-1, -rect.bottom / UserData.SizeSymbol.cyChar);
			break;
		case SB_PAGEDOWN:
			Vscroll->iVscrollInc = max(1, rect.bottom / UserData.SizeSymbol.cyChar);
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
			for (int i = 0; i < 2; i++)
			{
				ScrollWindow(hWndStruct[i].hWndChild, 0, -UserData.SizeSymbol.cyChar * Vscroll->iVscrollInc, NULL, NULL);
				SetScrollPos(hWndStruct[i].hWndChild, SB_VERT, Vscroll->iVscrollPos, TRUE);
				UpdateWindow(hWndStruct[i].hWndChild);
			}
		}

		return 0; 
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
	return 0;
}

LRESULT CALLBACK ChildWndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{	
	switch (iMsg)
	{
	case WM_CREATE:
		
		return 0;
	case WM_COMMAND:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		//int ID = GetWindowLong(hWnd, GWL_ID);
		int cyClient = HIWORD(lParam);

		UserData->Vscroll.iVscrollPos = 0;
		UserData->ullMaxNumLines = max(UserData->FileBuf[0].ullNumLines, UserData->FileBuf[1].ullNumLines);
		
		SetScrollBySize(hWnd, cyClient, UserData);
		
		return 0; 
	}
	case WM_SIZE:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		//int ID = GetWindowLong(hWnd, GWL_ID);
		int cyClient = HIWORD(lParam);

		SetScrollBySize(hWnd, cyClient, UserData);
		
		return 0;
	}
	case WM_VSCROLL:

		SendMessage(GetParent(hWnd), WM_VSCROLL, wParam, lParam);

		return 0;
	case WM_PAINT:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		int ID = GetWindowLong(hWnd, GWL_ID);
		int cyClient = HIWORD(lParam);
		if (UserData->FileBuf[0].lpcBuffer != NULL && UserData->FileBuf[1].lpcBuffer != NULL)
		{
			//SetScrollBySize(hWnd, cyClient, UserData);
			PaintText(hWnd, *UserData, ID);
		}

		return 0;
	}
	case WM_DESTROY: 
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);

		for (int i = 0; i < NUMBER_OF_WINDOW; i++) 
		{
			ClearBuffer(&UserData->FileBuf[i]);
		}

		PostQuitMessage(0);
		return 0; 
	}
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
	return 0;
}

BOOL OpenFileParams(HWND hWnd, FileBuffer * FileBuf, HWND hWndEdit)
{
	ClearBuffer(FileBuf);

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
		SetWindowText(hWndEdit, OpFileName.lpstrFile);
		if (!ReadFromFiles(FileName, FileBuf))
		{
			MessageBoxW(hWnd, L"File could not open!", L"ERROR", MB_OK);
			return false;
		}
	}
	else 
	{
		SetWindowText(hWndEdit, OpFileName.lpstrFile);
	}
	
	return true;
}

BOOL ReadFromFiles(LPWSTR path, FileBuffer * FileBuf)
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

	*FileBuf = FileInfo(hFileMap, FileToRead);

	CloseHandle(hFileMap);
	CloseHandle(FileToRead);
	return true;
}

void PaintText(HWND hWnd, UserDataStruct UserData, int ID)
{
	LPCSTR TextBuffer = UserData.FileBuf[ID].lpcBuffer + ((UINT64)UserData.Vscroll.iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	HDC hdc;
	PAINTSTRUCT ps;
	UINT64 ullCount = -1 + ((UINT64)UserData.Vscroll.iVscrollPos * NUMBER_OF_SYMBOLS_PER_LINE);
	
	hdc = BeginPaint(hWnd, &ps);
	RECT rect;
	GetClientRect(hWnd, &rect);
	
	int iPaintBeg = max(-1, UserData.Vscroll.iVscrollPos + rect.top / UserData.SizeSymbol.cyChar - 1);
	int iPaintEnd = min(int(UserData.FileBuf[ID].ullNumLines), UserData.Vscroll.iVscrollPos + rect.bottom / UserData.SizeSymbol.cyChar);

	for (int iteration = iPaintBeg; iteration < iPaintEnd; iteration++)
	{
		int y;
		char cOffsetBuf[SIZE_OFFSET_BUF] = { 0 };
		y = UserData.SizeSymbol.cyChar * (iteration - UserData.Vscroll.iVscrollPos + 1);
		int iLenght = sprintf(cOffsetBuf, "%08X :", (iteration + 1) * NUMBER_OF_SYMBOLS_PER_LINE);

		TextOutA(hdc, 0, y, cOffsetBuf, iLenght);
		TextOutA(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * NUMBER_OF_SYMBOLS_PER_LINE, y, (LPCSTR)"|", 1);
		for (int i = 0; i < NUMBER_OF_SYMBOLS_PER_LINE; i++)
		{
			char cHexBuf[SIZE_HEX_BUF] = { 0 };
			ullCount++; // если в последней строке все 16 символов выводит лишний офсет
			if (ullCount == UserData.FileBuf[ID].ullSizeBuffer)
			{
				break;
			}
			int iSymbol = (unsigned char)*TextBuffer;
			int iLenght = sprintf(cHexBuf, "%02X", iSymbol);
			TextOutA(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * i, y, cHexBuf, iLenght);

			if (iswprint(iSymbol) == 0)
			{
				TextOutA(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + UserData.SizeSymbol.cxCaps * i - UserData.SizeSymbol.cxCaps, y, (LPCSTR)".", 1);
			}
			else
			{
				TextOutA(hdc, UserData.SizeSymbol.cxChar * SIZE_OFFSET_BUF + UserData.SizeSymbol.cxChar * SIZE_HEX_BUF * (NUMBER_OF_SYMBOLS_PER_LINE + 1) + UserData.SizeSymbol.cxCaps * i - UserData.SizeSymbol.cxCaps, y, TextBuffer, 1);
			}
			TextBuffer++;
		}
		
		if (ullCount == UserData.FileBuf[ID].ullSizeBuffer)
		{
			break;
		}
	}
	EndPaint(hWnd, &ps);
}

void SetScrollBySize(HWND hWnd, int cyClient, UserDataStruct* UserData)
{
	UserData->Vscroll.iVscrollMax = max(0, int(UserData->ullMaxNumLines - cyClient / UserData->SizeSymbol.cyChar));
	UserData->Vscroll.iVscrollPos = min(UserData->Vscroll.iVscrollPos, UserData->Vscroll.iVscrollMax);

	SetScrollRange(hWnd, SB_VERT, 0, UserData->Vscroll.iVscrollMax, FALSE);
	SetScrollPos(hWnd, SB_VERT, UserData->Vscroll.iVscrollPos, TRUE);
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
	size.nHeightButton = 7 * size.cyChar / 4;

	ReleaseDC(hWnd, hdc);

	return size;
}

FileBuffer FileInfo(HANDLE hFileMap, HANDLE FileToRead)
{
	FileBuffer FileBuf;
	PVOID MappedMemory = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, NULL);
	FileBuf.lpcBuffer = (LPCSTR)MappedMemory;

	LARGE_INTEGER liSize;
	GetFileSizeEx(FileToRead, &liSize);
	FileBuf.ullSizeBuffer = liSize.QuadPart;

	long double ldNumLines = ceill(long double(FileBuf.ullSizeBuffer) / NUMBER_OF_SYMBOLS_PER_LINE);
	FileBuf.ullNumLines = (UINT64)ldNumLines;
	return FileBuf;
}

void ClearBuffer(FileBuffer *FileBuf)
{
	if (FileBuf->lpcBuffer != NULL)
	{
		UnmapViewOfFile(FileBuf->lpcBuffer);
		FileBuf->lpcBuffer = NULL;
		FileBuf->ullNumLines = 0;
		FileBuf->ullSizeBuffer = 0;
	}
}

void CreateChildWindow(HWND hWnd, hWndChildWindow* hWndStruct)
{
	for (int i = 0; i < NUMBER_OF_WINDOW; i++)
	{
		hWndStruct[i].hWndChild = CreateWindowEx(WS_EX_COMPOSITED, L"HEX_VIEWER_CHILD", NULL, WS_CHILDWINDOW | WS_VISIBLE | WS_VSCROLL | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)(0 + i), (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE), NULL);

		hWndStruct[i].hWndButton = CreateWindowEx(0, L"button", L">>", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_BORDER, 0, 0, 0, 0, hWnd, (HMENU)(2 + i), (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE), NULL);

		hWndStruct[i].hWndEdit = CreateWindowEx(0, L"edit", L"", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT, 0, 0, 0, 0, hWnd, (HMENU)(4 + i), (HINSTANCE)GetWindowLong(hWnd, GWLP_HINSTANCE), NULL);
	}
	
}

void MoveChildWindow(HWND hWnd, hWndChildWindow* hWndStruct, TextSize SizeSymbol)
{
	RECT rect;
	GetClientRect(hWnd, &rect);
	int cxBlock = rect.right / 2;
	int cyBlock = rect.bottom;

	for (int i = 0; i < NUMBER_OF_WINDOW; i++) 
	{
		MoveWindow(hWndStruct[i].hWndChild, cxBlock * i, SizeSymbol.nHeightButton, cxBlock, cyBlock - SizeSymbol.nHeightButton, TRUE);

		MoveWindow(hWndStruct[i].hWndButton, cxBlock * (i + 1) - 4 * SizeSymbol.cxCaps, 0, 4 * SizeSymbol.cxCaps, SizeSymbol.nHeightButton, TRUE);

		MoveWindow(hWndStruct[i].hWndEdit, cxBlock * i, 0, cxBlock - 4 * SizeSymbol.cxCaps, SizeSymbol.nHeightButton, TRUE);
	}
}


// SAL аннотации
// исправить ошибку с пустым файлом    
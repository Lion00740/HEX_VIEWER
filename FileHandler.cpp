#include "FileHandler.h"

BOOL OpenFileParams(_In_ HWND hWnd, _Inout_ FileData* FileBuf, _In_ HWND hWndEdit)
{
	ClearBuffer(FileBuf);

	OPENFILENAME	OpFileName;
	TCHAR			FileName[MAX_PATH] = { 0 };

	ZeroMemory(&OpFileName, sizeof(OPENFILENAME));
	ZeroMemory(&OpFileName, sizeof(OpFileName));

	OpFileName.lStructSize = sizeof(OpFileName);
	OpFileName.hwndOwner = hWnd;
	OpFileName.lpstrFile = FileName;
	OpFileName.nMaxFile = sizeof(FileName);
	OpFileName.lpstrFilter = L"*.*";
	OpFileName.lpstrFileTitle = NULL;
	OpFileName.nMaxFileTitle = 0;
	OpFileName.lpstrInitialDir = L"C:\\";
	OpFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileName(&OpFileName))
	{
		SetWindowText(hWndEdit, OpFileName.lpstrFile);

		if (!ReadFromFiles(FileName, FileBuf))
		{
			MessageBox(hWnd, L"File could not open!", L"ERROR", MB_OK);
			return FALSE;
		}
	}
	else
	{
		SetWindowText(hWndEdit, L"");
	}

	return TRUE;
}

BOOL ReadFromFiles(_In_ LPWSTR path, _Inout_ FileData* FileBuf)
{
	HANDLE hFileToRead = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileToRead == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	
	FileBuf->hFileMap = CreateFileMapping(hFileToRead, NULL, PAGE_READONLY, 0, 0, NULL);

	if (FileBuf->hFileMap == NULL)
	{
		CloseHandle(hFileToRead);
		return FALSE;
	}

	FileInfo(hFileToRead, FileBuf);
	
	CloseHandle(hFileToRead);

	MoveToMap(FileBuf, NULL, {0});
		
	return TRUE;
}

void FileInfo(_In_ HANDLE hFileToRead, _Out_ FileData* FileBuf)
{
	SYSTEM_INFO synf;
	DWORD		dwFileSizeHigh	= 0;

	ZeroMemory(&synf, sizeof(SYSTEM_INFO));

	GetSystemInfo(&synf);

	FileBuf->uiGranularity		=	synf.dwAllocationGranularity;
	FileBuf->uiBottomOffset		=	FileBuf->uiGranularity;

	FileBuf->ullSizeBuffer		=	GetFileSize(hFileToRead, &dwFileSizeHigh);
	FileBuf->ullSizeBuffer		+=	(((ULONGLONG)dwFileSizeHigh) << 32);

	FileBuf->ullNumLines		=	FileBuf->ullSizeBuffer + NUMBER_OF_SYMBOLS_PER_LINE - 1;
	FileBuf->ullNumLines		/=	NUMBER_OF_SYMBOLS_PER_LINE;
}

void MoveToMap(FileData* FileBuf, ULONGLONG ullFileOffSet, VScroll Vscroll) 
{
	ULONG ullBytesInBlock = FileBuf->uiGranularity * 2;

	if (ullFileOffSet % FileBuf->uiGranularity != 0) 
	{
		ullFileOffSet -= ullFileOffSet % FileBuf->uiGranularity;
	}

	FileBuf->ullCurrentOffset = ullFileOffSet;

	if ((Vscroll.iVscrollPos > FileBuf->uiBottomOffset / NUMBER_OF_SYMBOLS_PER_LINE || Vscroll.iVscrollPos < FileBuf->uiTopOffset / NUMBER_OF_SYMBOLS_PER_LINE)
		|| Vscroll.iVscrollPos < (FileBuf->ullSizeBuffer / NUMBER_OF_SYMBOLS_PER_LINE)) 
	{
		if (FileBuf->pbBuffer != NULL) 
		{
			UnmapViewOfFile(FileBuf->pbBuffer);
			FileBuf->pbBuffer = NULL;
		}

		if (ullFileOffSet + ullBytesInBlock > FileBuf->ullSizeBuffer) 
		{
			ullBytesInBlock = FileBuf->ullSizeBuffer - ullFileOffSet;
		}

		FileBuf->pbBuffer = (PBYTE)MapViewOfFile(
			FileBuf->hFileMap,
			FILE_MAP_READ,
			(DWORD)(ullFileOffSet >> 32),
			(DWORD)(ullFileOffSet & 0xFFFFFFFF),
			ullBytesInBlock
		);
		
		FileBuf->uiTopOffset = FileBuf->ullCurrentOffset;

		FileBuf->uiBottomOffset = FileBuf->ullCurrentOffset + FileBuf->uiGranularity;
	}

	if (FileBuf->pbBuffer == NULL)
	{
		CloseHandle(FileBuf->hFileMap);
		FileBuf->hFileMap = NULL;
	}
}

void ClearBuffer(_Inout_ FileData* FileBuf)
{
	if (FileBuf->pbBuffer != NULL)
	{
		UnmapViewOfFile(FileBuf->pbBuffer);
	}
	CloseHandle(FileBuf->hFileMap);
	ZeroMemory(FileBuf, sizeof(FileData));
}
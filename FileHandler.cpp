#include "FileHandler.h"

BOOL OpenFileParams(_In_ HWND hWnd, _Inout_ FileData* FileBuf, _In_ HWND hWndEdit)
{
	ClearBuffer(FileBuf);

	OPENFILENAMEW	OpFileName;
	memset(&OpFileName, 0, sizeof(OPENFILENAMEW));
	WCHAR			FileName[MAX_PATH] = { 0 };

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
		SetWindowTextW(hWndEdit, OpFileName.lpstrFile);
		if (!ReadFromFiles(FileName, FileBuf))
		{
			MessageBoxW(hWnd, L"File could not open!", L"ERROR", MB_OK);
			return FALSE;
		}
	}
	else
	{
		SetWindowTextW(hWndEdit, OpFileName.lpstrFile);
	}

	return TRUE;
}

BOOL ReadFromFiles(_In_ LPWSTR path, _Inout_ FileData* FileBuf)
{
	HANDLE hFileToRead = CreateFileW(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (hFileToRead == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}

	HANDLE hFileMap = CreateFileMappingW(hFileToRead, NULL, PAGE_READONLY, 0, 0, NULL);

	if (hFileMap == NULL)
	{
		CloseHandle(hFileToRead);
		return FALSE;
	}

	*FileBuf = FileInfo(hFileMap, hFileToRead);

	CloseHandle(hFileMap);
	CloseHandle(hFileToRead);
	return TRUE;
}

FileData FileInfo(_In_ HANDLE hFileMap, _In_ HANDLE FileToRead)
{
	FileData		FileBuf;
	PVOID			MappedMemory = MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
	LARGE_INTEGER	liSize;
	memset(&liSize, 0, sizeof(LARGE_INTEGER));

	GetFileSizeEx(FileToRead, &liSize);

	FileBuf.lpcBuffer = (LPCSTR)MappedMemory;
	FileBuf.ullSizeBuffer = liSize.QuadPart;
	long double ldNumLines = ceill(long double(FileBuf.ullSizeBuffer) / NUMBER_OF_SYMBOLS_PER_LINE);
	FileBuf.ullNumLines = (UINT64)ldNumLines;

	return FileBuf;
}

void ClearBuffer(_Inout_ FileData* FileBuf)
{
	if (FileBuf->lpcBuffer != NULL)
	{
		UnmapViewOfFile(FileBuf->lpcBuffer);
		FileBuf->lpcBuffer = NULL;
		FileBuf->ullNumLines = 0;
		FileBuf->ullSizeBuffer = 0;
	}
}
#pragma once
#include "Macros.h"

/// <summary>
/// ��������� ��� �������� ������ �����.
/// </summary>
struct FileData
{
	PBYTE		pbBuffer			= NULL;		// ����� � ������� �������� ����.
	ULONGLONG	ullSizeBuffer		= 0;		// ������ �����.
	ULONGLONG	ullNumLines			= 0;		// ����� ����� � �����.
	ULONGLONG	uiBottomOffset		= 0;		//
	ULONGLONG	uiTopOffset			= 0;		//
	UINT		uiGranularity		= 0;		// ������������� (������ 64 ��).
	HANDLE		hFileMap			= 0;		// HANDLE ����������� �����.
	ULONGLONG	ullCurrentOffset	= 0;		// 
};

/// <summary>
/// ��������� ��� �������� �������� ����.
/// </summary>
struct hWndChildWindows
{
	HWND hWndChild	= NULL;		// ���������� ���� ������.
	HWND hWndButton = NULL;		// ���������� ������.
	HWND hWndEdit	= NULL;		// ���������� ���� ��������������.
};

/// <summary>
/// ��������� ��� �������� ���������� ������ �������.
/// </summary>
struct SymbolParams
{
	INT cxChar			= 0;	// ������� ������ �������.
	INT cyChar			= 0;	// ������ �������.
	INT cxCaps			= 0;	// ������ �������.
	INT cyHeightButton	= 0;	// ������ ������ (����� 7/4 �� ������ �������).
};

/// <summary>
/// ��������� ��� �������� ������ �������.
/// </summary>
struct VScroll
{
	INT iVscrollMax = 0;	// ������������ ������� �������.
	INT iVscrollPos = 0;	// ������� ������� �������.
	INT iVscrollInc = 0;	// ��������� �������.
};

/// <summary>
/// ��������� ��� �������� ���� ������ ���������.
/// </summary>
struct UserDataStruct
{
	SymbolParams	SizeSymbol;										// ������ �������.
	VScroll			Vscroll;										// ������ �������.
	FileData		FileBuf[NUMBER_OF_WINDOW];						// ������ �������� ���������� � ����� ��� ������� ����.
	ULONGLONG		ullMaxNumLines				= 0;				// ������������ ������� ��� ������� �� ��������.
	ULONGLONG		ullIncrement				= 0;				// 
};
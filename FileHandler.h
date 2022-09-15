#pragma once
#include "AllStruct.h"

/// <summary>
///		������� ��� �������� ����� � �������� ��� ��������.
/// </summary>
/// <param name="path">
///		���� �����.
/// </param>
/// <param name="FileBuf">
///		��������� ��� �������� ������ �����.
/// </param>
/// <returns>
///		���������� FALSE, ���� ���� �� ����� ��������� ��� �������� � ���������,
///					TRUE, ���� ��� ���������.
/// </returns>

BOOL
ReadFromFiles(	_In_		LPWSTR		path,
				_Inout_		FileData*	FileBuf
);

/// <summary>
///		��������� ��������� ��� ������ ������ � ��������� edit.
/// </summary>
/// <param name="hWnd">
///		���������� ������������� ����.
/// </param>
/// <param name="FileBuf">
///		��������� ��� �������� �����.
/// </param>
/// <param name="hWndEdit">
///		���������� ���� ��������������.
/// </param>
/// <returns>
///		���������� FALSE, ���� ������� ReadFromFiles ������� FALSE,
///					True, ���� ��� ���������.
/// </returns>

BOOL
OpenFileParams(	_In_		HWND		hWnd,
				_Inout_		FileData*	FileBuf,
				_In_		HWND		hWndEdit
);

/// <summary>
///		������� ������� ��������� ��������� FileData.
/// </summary>
/// <param name="hFileMap">
///		HANDLE ����������� �����.
/// </param>
/// <param name="FileToRead">
///		HANDLE ���������� �����.
/// </param>
/// <returns>
///		���������� ��������� � ������� �����.
/// </returns>

void
FileInfo(_In_ HANDLE FileToRead,
		 _Out_ FileData* FileBuf
);

/// <summary>
///		������� ��� ������������ �� ������� ������������ �������.
/// </summary>
/// <param name="FileBuf">
///		��������� ��� �������� ������ �����.
/// </param>
/// <param name="ullFileOffSet">
///		�������� � �����.
/// </param>
/// <param name="Vscroll">
///		��������� ��� �������� ������ �������.
/// </param>

void MoveToMap(FileData* FileBuf, ULONGLONG ullFileOffSet, VScroll Vscroll);

/// <summary>
///		������� ��� ������� ��������� FileData.
/// </summary>
/// <param name="FileBuf">
///		��������� ������� ������ � ���� ������ �����.
/// </param>

VOID
ClearBuffer(_Inout_ FileData* FileBuf);
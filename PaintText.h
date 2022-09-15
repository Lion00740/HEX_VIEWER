#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "AllStruct.h"

/// <summary>
///		������� ��� ������ ���� ����� �� ����� ������.
/// </summary>
/// <param name="hWnd">
///		���������� ��������� ����.
/// </param>
/// <param name="UserData">
///		���������, ������� ������ � ���� ������ � �����, ��������, �������.
/// </param>
/// <param name="ID">
///		ID ��������� ����.
/// </param>

VOID				
PaintAllLine(_In_ HWND				hWnd,
			 _In_ UserDataStruct	UserData,
			 _In_ INT				ID
);

/// <summary>
///		����� �� ����� ����� ������ (16-���� ������������� �������� � ���� �������).
/// </summary>
/// <param name="hdc">
///		
/// </param>
/// <param name="TextBuffer">
///		
/// </param>
/// <param name="ullCount">
///		
/// </param>
/// <param name="UserData">
///		���������, ������� ������ � ���� ������ � �����, ��������, �������.
/// </param>
/// <param name="ID">
///		ID ��������� ����.
/// </param>
/// <param name="y">
///		������� ������ ������������ ���������.
/// </param>
VOID				
PaintLine(_In_		HDC				hdc,
		  _In_		PBYTE			TextBuffer,
		  _Inout_	ULONGLONG*		ullCount, 
		  _In_		UserDataStruct	UserData,
		  _In_		INT				ID, 
		  _In_		INT				y, 
					ULONGLONG		increment
);

/// <summary>
///		����� �� ����� �������� � ������� '|'.
/// </summary>
/// <param name="hdc">
///		
/// </param>
/// <param name="UserData">
///		���������, ������� ������ � ���� ������ � �����, ��������, �������.
/// </param>
/// <param name="iteration">
///		
/// </param>
/// <param name="y">
///		������� ������ ������������ ���������.
/// </param>
VOID				
PaintOffset(_In_ HDC			hdc, 
			_In_ UserDataStruct UserData, 
			_In_ INT			iteration, 
			_In_ INT			y
);
#pragma once
#include "AllStruct.h"

/// <summary>
///		������� ��� ��������� ������� �������.
/// </summary>
/// <param name="wParam">
///		
/// </param>
/// <param name="hWnd">
///		���������� ������������� ����.
/// </param>
/// <param name="hWndStruct">
///		��������� ��� �������� �������� ����.
/// </param>

VOID
ScrollEventsControl(_In_	WPARAM				wParam,
	_In_	HWND				hWnd,
	_Inout_ hWndChildWindows* hWndStruct
);

/// <summary>
///		������� ��� ������� ������� ������� ������������ ������� ����.
/// </summary>
/// <param name="hWnd">
///		���������� ��������� ����.
/// </param>
/// <param name="cyClient">
///		������ ���������� ������� �� ���������.
/// </param>
/// <param name="UserData">
///		���������, ������� ������ � ���� ������ � �����, ��������, �������.
/// </param>

VOID
SetScrollBySize(_In_		HWND			hWnd,
	_In_		INT				cyClient,
	_Inout_		UserDataStruct* UserData
);
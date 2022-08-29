#pragma once
#include "AllStruct.h"

/// <summary>
///		������� ��� �������� �������� ����.
/// </summary>
/// <param name="hWnd">
///		���������� ������������� ����.
/// </param>
/// <param name="hWndStruct">
///		��������� � ������� �������� �������� ����.
/// </param>

VOID
CreateChildWindow(_In_		HWND				hWnd,
				  _Inout_	hWndChildWindows*	hWndStruct
);

/// <summary>
///		������� ��� ����������� �������� ����.
/// </summary>
/// <param name="hWnd">
///		���������� ������������� ����.
/// </param>
/// <param name="hWndStruct">
///		��������� � ������� �������� �������� ����.
/// </param>
/// <param name="SizeSymbol">
///		��������� � ������� �������� ��������� ������ �������.
/// </param>

VOID
MoveChildWindow(_In_	HWND				hWnd,
				_Inout_ hWndChildWindows*	hWndStruct,
				_In_	SymbolParams		SizeSymbol
);
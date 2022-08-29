#pragma once
#include "AllStruct.h"

/// <summary>
///		Функция для создания дочерних окон.
/// </summary>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <param name="hWndStruct">
///		Структура в которой хранятся дочерние окна.
/// </param>

VOID
CreateChildWindow(_In_		HWND				hWnd,
				  _Inout_	hWndChildWindows*	hWndStruct
);

/// <summary>
///		Функция для перемещения дочерних окон.
/// </summary>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <param name="hWndStruct">
///		Структура в которой хранятся дочерние окна.
/// </param>
/// <param name="SizeSymbol">
///		Структура в которой хранятся параметры одного символа.
/// </param>

VOID
MoveChildWindow(_In_	HWND				hWnd,
				_Inout_ hWndChildWindows*	hWndStruct,
				_In_	SymbolParams		SizeSymbol
);
#pragma once
#include "AllStruct.h"
#include "Scroll.h"
#include "CreateAndMoveChild.h"
#include "PaintText.h"
#include "FileHandler.h"
#include "SymbolParameters.h"

/// <summary>
///		Процедура которая обрабатывает сообщения родительского окна.
/// </summary>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <param name="iMsg">
///		
/// </param>
/// <param name="wParam">
///		
/// </param>
/// <param name="lParam">
///		
/// </param>
/// <remarks>
///		
/// </remarks>
/// <returns>
/// 
/// </returns>

LRESULT
CALLBACK
WndProc(_In_		HWND	hWnd,
		_In_opt_	UINT	iMsg,
		_In_		WPARAM	wParam,
		_In_		LPARAM	lParam
);

/// <summary>
///		Процедура которая обрабатывает сообщения дочерних окон.
/// </summary>
/// <param name="hWnd">
/// 
/// </param>
/// <param name="iMsg">
/// 
/// </param>
/// <param name="wParam">
/// 
/// </param>
/// <param name="lParam">
/// 
/// </param>
/// <returns>
/// 
/// </returns>

LRESULT
CALLBACK
ChildWndProc(_In_		HWND	hWnd,
			 _In_opt_	UINT	iMsg,
			 _In_		WPARAM	wParam,
			 _In_		LPARAM	lParam
);
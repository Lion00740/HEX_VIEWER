#pragma once
#include "AllStruct.h"

/// <summary>
///		Функция для обработки событий скролла.
/// </summary>
/// <param name="wParam">
///		
/// </param>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <param name="hWndStruct">
///		Структура для хранения дочерних окон.
/// </param>

VOID
ScrollEventsControl(_In_	WPARAM				wParam,
	_In_	HWND				hWnd,
	_Inout_ hWndChildWindows* hWndStruct
);

/// <summary>
///		Функция для расчета позиции скролла относительно размера окна.
/// </summary>
/// <param name="hWnd">
///		Дескриптор дочернего окна.
/// </param>
/// <param name="cyClient">
///		Размер клиентской области по вертекали.
/// </param>
/// <param name="UserData">
///		Структура, которая хранит в себе данные о файле, символах, скролле.
/// </param>

VOID
SetScrollBySize(_In_		HWND			hWnd,
	_In_		INT				cyClient,
	_Inout_		UserDataStruct* UserData
);
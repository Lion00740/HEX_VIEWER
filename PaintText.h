#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "AllStruct.h"

/// <summary>
///		Функция для вывода всех строк до конца экрана.
/// </summary>
/// <param name="hWnd">
///		Дескриптор дочернего окна.
/// </param>
/// <param name="UserData">
///		Структура, которая хранит в себе данные о файле, символах, скролле.
/// </param>
/// <param name="ID">
///		ID дочернего окна.
/// </param>

VOID				
PaintAllLine(_In_ HWND				hWnd,
			 _In_ UserDataStruct	UserData,
			 _In_ INT				ID
);

/// <summary>
///		Вывод на экран одной строки (16-чное представление символов и сами символы).
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
///		Структура, которая хранит в себе данные о файле, символах, скролле.
/// </param>
/// <param name="ID">
///		ID дочернего окна.
/// </param>
/// <param name="y">
///		Позиция строки относительно вертикали.
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
///		Вывод на экран смещения и символа '|'.
/// </summary>
/// <param name="hdc">
///		
/// </param>
/// <param name="UserData">
///		Структура, которая хранит в себе данные о файле, символах, скролле.
/// </param>
/// <param name="iteration">
///		
/// </param>
/// <param name="y">
///		Позиция строки относительно вертикали.
/// </param>
VOID				
PaintOffset(_In_ HDC			hdc, 
			_In_ UserDataStruct UserData, 
			_In_ INT			iteration, 
			_In_ INT			y
);
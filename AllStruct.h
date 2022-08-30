#pragma once
#include "Macros.h"

/// <summary>
/// Структура для хранения данных файла.
/// </summary>
struct FileData
{
	LPCSTR lpcBuffer = NULL;		// Буфер в котором хранится файл.
	ULONGLONG ullSizeBuffer = 0;		// Размер файла.
	ULONGLONG ullNumLines = 0;		// Число строк в файле.
};

/// <summary>
/// Структура для хранения дочерних окон.
/// </summary>
struct hWndChildWindows
{
	HWND hWndChild;		// Дескриптор окна вывода.
	HWND hWndButton;	// Дескриптор кнопки.
	HWND hWndEdit;		// Дескриптор окна редактирования.
};

/// <summary>
/// Структура для хранения параметров одного символа.
/// </summary>
struct SymbolParams
{
	INT cxChar = 0;	// Средняя ширина символа.
	INT cyChar = 0;	// Высота символа.
	INT cxCaps = 0;	// Ширина символа.
	INT cyHeightButton = 0;	// Высота кнопки (равна 7/4 от высоты символа).
};

/// <summary>
/// Структура для хранения данных скролла.
/// </summary>
struct VScroll
{
	INT iVscrollMax = 0;	// Максимальная позиция скролла.
	INT iVscrollPos = 0;	// Текущая позиция скролла.
	INT iVscrollInc = 0;	// Инкримент скролла.
};

/// <summary>
/// Структура для хранения всех данных программы.
/// </summary>
struct UserDataStruct
{
	SymbolParams	SizeSymbol;						// Данные символа.
	VScroll			Vscroll;						// Данные скролла.
	FileData		FileBuf[NUMBER_OF_WINDOW];		// Массив хранящий информацию о файле для каждого окна.
	ULONGLONG		ullMaxNumLines = 0;				// Максимальная позиция для каждого из скроллов.
	UINT			ullGranularity = 0;				// Гранулярность (всегда 64 Кб)
};
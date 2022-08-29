#pragma once
#include "AllStruct.h"

/// <summary>
///		Функция для открытия файла и создания его проекции.
/// </summary>
/// <param name="path">
///		Путь файла.
/// </param>
/// <param name="FileBuf">
///		Структура для хранения данных файла.
/// </param>
/// <returns>
///		Возвращает FALSE, если файл не может открыться или проблемы с проекцией,
///					TRUE, если все открылось.
/// </returns>

BOOL
ReadFromFiles(_In_		LPWSTR		path,
	_Inout_	FileData* FileBuf
);

/// <summary>
///		Открывает проводник для выбора файлов и заполняет edit.
/// </summary>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <param name="FileBuf">
///		Структура для хранения файла.
/// </param>
/// <param name="hWndEdit">
///		Дескриптор окна редактирования.
/// </param>
/// <returns>
///		Возвращает FALSE, если функция ReadFromFiles вернула FALSE,
///					True, если все открылось.
/// </returns>

BOOL
OpenFileParams(_In_		HWND		hWnd,
	_Inout_	FileData* FileBuf,
	_In_		HWND		hWndEdit
);

/// <summary>
///		Функция которая заполняет структуру FileData.
/// </summary>
/// <param name="hFileMap">
///		HANDLE смапленного файла.
/// </param>
/// <param name="FileToRead">
///		HANDLE считанного файла.
/// </param>
/// <returns>
///		Возвращает структуру с данными файла.
/// </returns>

FileData
FileInfo(_In_ HANDLE hFileMap,
	_In_ HANDLE FileToRead
);

/// <summary>
///		Функция для очистки структуры FileData.
/// </summary>
/// <param name="FileBuf">
///		Структура которая хранит в себе данные файла.
/// </param>

VOID
ClearBuffer(_Inout_ FileData* FileBuf);
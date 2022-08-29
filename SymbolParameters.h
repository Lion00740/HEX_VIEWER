#pragma once
#include "AllStruct.h"

/// <summary>
///		Функция для получения параметров одного символа.
/// </summary>
/// <param name="hWnd">
///		Дескриптор родительского окна.
/// </param>
/// <returns>
///		Возвращает структуру хранящую параметры одного символа.
/// </returns>

SymbolParams
GetParamsSymbol(_In_ HWND hWnd);
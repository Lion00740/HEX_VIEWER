#include "WindowsProcedure.h"

INT WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int iCmdShow)
{
	HWND		hWnd;
	memset(&hWnd, 0, sizeof(HWND));
	MSG			msg;
	memset(&msg, 0, sizeof(MSG));
	WCHAR		AppName[] = L"HEX_VIEWER";
	WNDCLASSEX	wcex;
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize			= sizeof(WNDCLASSEX);
	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW + 1);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hIconSm		= LoadIcon(NULL, IDI_APPLICATION);
	wcex.hInstance		= hInst;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= AppName;

	if (!RegisterClassEx(&wcex))
	{
		return EXIT_FAILURE;
	}

	wcex.lpfnWndProc	= ChildWndProc;
	wcex.cbWndExtra		= sizeof(DWORD);
	wcex.hIcon			= NULL;
	wcex.lpszClassName	= L"HEX_VIEWER_CHILD";
	wcex.hIconSm		= NULL;

	if (!RegisterClassEx(&wcex))
	{
		return EXIT_FAILURE;
	}

	hWnd = CreateWindowExW(
		0, 
		AppName, 
		L"HEX_VIEWER", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
		NULL, 
		NULL, 
		wcex.hInstance, 
		NULL
	);

	if (!hWnd)
	{
		return EXIT_FAILURE;
	}

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// в функции не больше 7 параметров
// success
// создать дочернее окно которое будет в себе содержать все остальные дочерние окна, в том числе окно вывода текста (для того чтобы сделать сколько угодно окошек)
// переименовать функции, переменные
// вывести ullCount за пределы функции 
// сделать TextBuffer по индексу 
// добавить новые функции 
// переименовать ullcount
// исправить проблему с отрисовкой лишнего буфера
// исправить проблемы со скролом
// исправить проблему с перерисовкой
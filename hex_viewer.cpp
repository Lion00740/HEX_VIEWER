#include "WindowsProcedure.h"

INT WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR lpCmdLine, _In_ int iCmdShow)
{
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(hPrevInst);

	HWND		hWnd;
	MSG			msg;
	WNDCLASSEX	wcex;
	TCHAR		AppName[] = L"HEX_VIEWER";
	
	ZeroMemory(&hWnd, sizeof(HWND));
	ZeroMemory(&msg, sizeof(MSG));
	ZeroMemory(&wcex, sizeof(WNDCLASSEX));

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

	hWnd = CreateWindowEx(
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

// success
// ������� �������� ���� ������� ����� � ���� ��������� ��� ��������� �������� ����, � ��� ����� ���� ������ ������ (��� ���� ����� ������� ������� ������ ������)
// ������������� �������, ����������
// ������� ullCount �� ������� �������
// ��������� �������� �� �������
// ��������� �������� � ������������
// ������� �������� (����� ������� �������, ����� � ��� ���� errorlockup) or visual assist (���� �������� ����)
// scrollwinodow (� ����� ���� ��������� ����������, ����� ��� ��� ��������)
// ������ �������� ���������� ������, ������ ��� (�� ����� ��� ��� ���)
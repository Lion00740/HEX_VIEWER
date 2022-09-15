#include "WindowsProcedure.h"

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	static UserDataStruct	UserData;
	static hWndChildWindows hWndStruct[NUMBER_OF_WINDOW];

	switch (iMsg)
	{
	case WM_CREATE:
		
		UserData.SizeSymbol = GetParamsSymbol(hWnd);

		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG)&UserData);

		CreateChildWindow(hWnd, hWndStruct);

		return 0;
	case WM_COMMAND:
		switch (wParam)
		{
		case CLICKED_ON_LEFT_BUTTON:

			if (!OpenFileParams(hWnd, &UserData.FileBuf[0], hWndStruct[0].hWndEdit))
			{
				break;
			}

			SendMessage(hWndStruct[0].hWndChild, WM_COMMAND, wParam, lParam);
			break;
		case CLICKED_ON_RIGHT_BUTTON:

			if (!OpenFileParams(hWnd, &UserData.FileBuf[1], hWndStruct[1].hWndEdit))
			{
				break;
			}

			SendMessage(hWndStruct[1].hWndChild, WM_COMMAND, wParam, lParam);
			break;
		}

		return 0;
	case WM_SIZE:
	{
		MoveChildWindow(hWnd, hWndStruct, UserData.SizeSymbol);

		return 0;
	}
	case WM_VSCROLL:
	{
		ScrollEventsControl(wParam, hWnd, hWndStruct);

		return 0;
	}
	case WM_DESTROY:

		for (INT i = 0; i < NUMBER_OF_WINDOW; i++)
		{
			ClearBuffer(&UserData.FileBuf[i]);
		}

		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
}

LRESULT CALLBACK ChildWndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_COMMAND:
	{
		UserDataStruct*		UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		if (UserData->FileBuf[0].pbBuffer != NULL && UserData->FileBuf[1].pbBuffer != NULL)
		{
			RECT rect;

			ZeroMemory(&rect, sizeof(RECT));

			GetClientRect(hWnd, &rect);

			UserData->Vscroll.iVscrollPos = 0;
			UserData->ullMaxNumLines = max(UserData->FileBuf[0].ullNumLines, UserData->FileBuf[1].ullNumLines);

			//SetScrollBySize(hWnd, rect.bottom, UserData);
		}
		// Отправить событие перирисовки
		return 0;
	}
	case WM_SIZE:
	{
		UserDataStruct*		UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		INT					cyClient = HIWORD(lParam);

		SetScrollBySize(hWnd, cyClient, UserData);

		return 0;
	}
	case WM_VSCROLL:

		SendMessage(GetParent(hWnd), WM_VSCROLL, wParam, lParam);

		return 0;
	case WM_PAINT:
	{
		UserDataStruct*		UserData =	(UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		INT					ID		 =	GetWindowLong(hWnd, GWL_ID);
		RECT rect;

		ZeroMemory(&rect, sizeof(RECT));

		
		if (UserData->FileBuf[0].pbBuffer != NULL && UserData->FileBuf[1].pbBuffer != NULL) // работает когда все файлы выбраны
		{
			SetScrollBySize(hWnd, rect.bottom, UserData);
			PaintAllLine(hWnd, *UserData, ID);
		}

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
}

LRESULT CALLBACK OutputTextWndProc(_In_ HWND hWnd, _In_opt_ UINT iMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (iMsg)
	{
	case WM_COMMAND:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		if (UserData->FileBuf[0].pbBuffer != NULL && UserData->FileBuf[1].pbBuffer != NULL)
		{
			RECT rect;

			ZeroMemory(&rect, sizeof(RECT));

			GetClientRect(hWnd, &rect);

			UserData->Vscroll.iVscrollPos = 0;
			UserData->ullMaxNumLines = max(UserData->FileBuf[0].ullNumLines, UserData->FileBuf[1].ullNumLines);

			//SetScrollBySize(hWnd, rect.bottom, UserData);
		}
		// Отправить событие перирисовки
		return 0;
	}
	case WM_SIZE:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		INT					cyClient = HIWORD(lParam);

		SetScrollBySize(hWnd, cyClient, UserData);

		return 0;
	}
	case WM_VSCROLL:

		SendMessage(GetParent(hWnd), WM_VSCROLL, wParam, lParam);

		return 0;
	case WM_PAINT:
	{
		UserDataStruct* UserData = (UserDataStruct*)GetWindowLongPtr(GetParent(hWnd), GWLP_USERDATA);
		INT					ID = GetWindowLong(hWnd, GWL_ID);
		RECT rect;

		ZeroMemory(&rect, sizeof(RECT));


		if (UserData->FileBuf[0].pbBuffer != NULL && UserData->FileBuf[1].pbBuffer != NULL) // работает когда все файлы выбраны
		{
			SetScrollBySize(hWnd, rect.bottom, UserData);
			PaintAllLine(hWnd, *UserData, ID);
		}

		return 0;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, iMsg, wParam, lParam);
	}
}
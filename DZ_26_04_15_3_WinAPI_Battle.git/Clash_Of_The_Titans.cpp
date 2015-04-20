#include<Windows.h>
#include<tchar.h>
#include<string>

#define ID_combo1 1001
#define ID_combo2 1001
#define ID_button1 1010
#define ID_list1 1020

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"MyApp";
HINSTANCE hinst;
RECT desktop, cr;
std::wstring str1, str2, res = L"";

HWND combo1, combo2, button1, list1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hinstance;
	wcex.hIcon = LoadIcon(hinstance, MAKEINTRESOURCE(IDI_APPLICATION));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	/*wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);*/
	wcex.hbrBackground = CreateSolidBrush(RGB(10, 128, 256));
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WindowsClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, L"Call faild!", L"MyApp", NULL);
		return 1;
	}

	hinst = hinstance;
	GetWindowRect(GetDesktopWindow(), &desktop);
	HWND hWnd = CreateWindow(
		WindowsClass,
		Title,
		WS_OVERLAPPEDWINDOW,
		desktop.right / 4,
		desktop.bottom / 6,
		desktop.right / 2,
		desktop.bottom / 2,
		NULL,
		NULL,
		hinst,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"Create window faild!", L"MyApp", NULL);
		return 1;
	}
	SetTimer(hWnd, 1, 100, NULL);
	ShowWindow(hWnd, nCmdShow);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{

	case WM_COMMAND:
	{


					   if (LOWORD(wParam) == ID_combo1 && HIWORD(wParam) == CBN_SELENDOK) {


						   auto cur_sel1 = SendMessage(combo1, CB_GETCURSEL, 0, 0);
						   auto combo1_text = SendMessage(combo1, CB_GETLBTEXT, cur_sel1, (LPARAM)str1.c_str());
						   res = str1.c_str();
					   }

					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK) {

						   auto cur_sel2 = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   auto combo2_text = SendMessage(combo2, CB_GETLBTEXT, cur_sel2, (LPARAM)str2.c_str());
						   res += L" ";
						   res += str2.c_str();
					   }

	}

	case BN_CLICKED:

		if (LOWORD(wParam) == ID_button1){
			SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res.c_str());
		}

		break;

	case WM_CREATE:

		GetClientRect(hWnd, &cr);

		combo1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			cr.right / 5,
			cr.bottom / 5,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_combo1,
			hinst,
			NULL);

		SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"Чем бить?");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"Рукой");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"Ногой");

		combo2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			cr.right / 5,
			cr.bottom / 2,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_combo2,
			hinst,
			NULL);

		SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Куда бить?");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в голову");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в корпус");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в ноги");

		button1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"button",
			L"Ударить!!!",
			WS_CHILD | WS_VISIBLE,
			cr.right / 2 - 8 * 6,
			cr.bottom - 50,
			80,
			30,
			hWnd,
			(HMENU)ID_button1,
			hinst, NULL);

		list1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"listbox",
			L"",
			WS_CHILD | WS_VISIBLE | LBS_STANDARD | WS_VSCROLL | LBS_SORT,
			cr.right / 2,
			cr.bottom / 5,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_list1,
			hinst,
			NULL);



		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}

	return 0;
}
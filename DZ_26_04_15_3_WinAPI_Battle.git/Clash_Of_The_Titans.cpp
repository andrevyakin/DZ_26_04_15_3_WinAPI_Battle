#include<Windows.h>
#include<tchar.h>
#include<cstdlib>
#include<string>

#define ID_combo1 1001
#define ID_combo2 1001
#define ID_button1 1010
#define ID_list1 1020
#define ID_list2 1021
#define ID_edit1 1030
#define ID_edit2 1031

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"БИТВА ТИТАНОВ";
HINSTANCE hinst;
RECT desktop, cr;
std::wstring str1, str2, res;
bool move = true;				//Чей ход

HWND combo1, combo2, button1, list1, list2, edit1, edit2;

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

						   LRESULT cur_sel1 = SendMessage(combo1, CB_GETCURSEL, 0, 0);
						   LRESULT combo1_text = SendMessage(combo1, CB_GETLBTEXT, cur_sel1, (LPARAM)str1.c_str());
						   res = str1.c_str();
					   }

					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK) {

						   LRESULT cur_sel2 = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   LRESULT combo2_text = SendMessage(combo2, CB_GETLBTEXT, cur_sel2, (LPARAM)str2.c_str());
						   res += L" ";
						   res += str2.c_str();
					   }

	}

	case BN_CLICKED:

		if (LOWORD(wParam) == ID_button1){
			if (move){	//Ход игрока
				if (str1.c_str()[0] == L'\0' && str2.c_str()[0] == L'\0')
					MessageBox(hWnd, L"Не выбрано чем и куда бить!", L"Ошибка!", MB_OK);
				else if (str1.c_str()[0] == '\0' && str2.c_str()[0] != '\0')
					MessageBox(hWnd, L"Не выбрано чем бить!", L"Ошибка!", MB_OK);
				else if (str1.c_str()[0] != '\0' && str2.c_str()[0] == '\0')
					MessageBox(hWnd, L"Не выбрано куда бить!", L"Ошибка!", MB_OK);
				else{
					move = false;
					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res.c_str());														//показать текст в списке
					ShowWindow(combo1, SW_HIDE);																					//скрыть верхний комбобокс
					GetClientRect(hWnd, &cr);																						//узнать параметры клиенсткого окна
					SetWindowPos(combo2, HWND_TOP, cr.right / 7, cr.bottom / 2.5, cr.right / 5, cr.bottom / 2, SWP_SHOWWINDOW);		//сдвинуть нижний комбобокс
					SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Блокировать удар");												//изменить текст в комбобоксе (подсказку)
					SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Блокировать");													//изменить текс на кнопке

				}
			}
			else{	//ход компьютера
				move = true;
			}
		}

		break;

	case WM_CREATE:

		GetClientRect(hWnd, &cr);

		combo1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			cr.right / 7,
			cr.bottom / 5,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_combo1,
			hinst,
			NULL);

		SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"Чем бить?");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"Рукой ");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"Ногой ");

		combo2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			cr.right / 7,
			cr.bottom / 2,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_combo2,
			hinst,
			NULL);

		SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Куда бить?");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в голову");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в грудь");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"в живот");

		button1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"button",
			L"Ударить!!!",
			WS_CHILD | WS_VISIBLE,
			cr.right / 2 - 8 * 6,
			cr.bottom - 50,
			100,
			30,
			hWnd,
			(HMENU)ID_button1,
			hinst, NULL);

		list1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"listbox",
			L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL,
			cr.right / 7*3,
			cr.bottom / 5,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_list1,
			hinst,
			NULL);

		list2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"listbox",
			L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL,
			cr.right / 7*5,
			cr.bottom / 5,
			cr.right / 5,
			cr.bottom / 2,
			hWnd,
			(HMENU)ID_list1,
			hinst,
			NULL);

		edit1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"edit",
			L"Ваше мощное тело",
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			cr.right / 7 * 3,
			cr.bottom / 8,
			cr.right / 5,
			20,
			hWnd,
			(HMENU)ID_edit1,
			hinst,
			NULL);

		edit2 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"edit",
			L"Слабая тушка противника",
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			cr.right / 7 * 5,
			cr.bottom / 8,
			cr.right / 5,
			20,
			hWnd,
			(HMENU)ID_edit2,
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
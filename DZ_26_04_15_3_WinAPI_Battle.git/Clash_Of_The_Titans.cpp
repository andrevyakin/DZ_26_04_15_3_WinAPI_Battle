#include<Windows.h>
#include<tchar.h>
#include<cstdlib>
#include<string>
#include<ctime>

#define ID_combo1 1001
#define ID_combo2 1001
#define ID_button1 1010
#define ID_list1 1020
#define ID_list2 1021
#define ID_edit1 1030
#define ID_edit2 1031
#define ID_edit3 1032
#define ID_edit4 1034

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"БИТВА ТИТАНОВ";
HINSTANCE hinst;
RECT desktop, cr;
std::wstring str1, str2, res;
bool move = true;										//Чей ход
int kick[2], block, health[2] = { 100, 100 };			//Удар, блок, здоровье.

HWND combo1, combo2, button1, list1, list2, edit1, edit2, edit3, edit4;

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
						   kick[0] = cur_sel1;
					   }

					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK) {

						   LRESULT cur_sel2 = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   LRESULT combo2_text = SendMessage(combo2, CB_GETLBTEXT, cur_sel2, (LPARAM)str2.c_str());
						   res += L" ";
						   res += str2.c_str();
						   kick[1] = cur_sel2;
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
					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res.c_str());																	//показать текст в списке
					ShowWindow(combo1, SW_HIDE);																								//скрыть верхний комбобокс
					GetClientRect(hWnd, &cr);																									//узнать параметры клиенсткого окна
					SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 3.75, cr.bottom / 2.5, cr.right / 9 * 1.5, cr.bottom, SWP_SHOWWINDOW);		//сдвинуть нижний комбобокс
					SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Блокировать удар");															//изменить текст в комбобоксе (подсказку)
					SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Блокировать");																//изменить текс на кнопке

					//Блокирование удара компом
					srand((unsigned)time(NULL));
					int block = rand() % 3;

					if (kick[1] == block) //Удар блокирован
					{
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)L"Удар блокирован");
						if (kick[0])
						{
							health[0] -= 5;
							wchar_t temp[20] = L"Здоровье ", tempInt[10];
							_itow_s(health[0], tempInt, 10);
							wcscat_s(temp, tempInt);
							wcscat_s(temp, L"%");
							SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
						}
					}
					else
					{
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)L"Удар пропущен");
						health[1] -= kick[0] ? 15 : 10;
						wchar_t temp[20] = L"Здоровье ", tempInt[10];
						_itow_s(health[1], tempInt, 10);
						wcscat_s(temp, tempInt);
						wcscat_s(temp, L"%");
						SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
 					}
				}
			}
			else{	//ход компьютера
				move = true;
				kick[0] = rand() % 2;
				kick[1] = rand() % 3;

				if (kick[0])
					res = L"Ногой ";
				else
					res = L"Рукой ";

				switch (kick[1])
				{
				case 0:
					res += L"в голову";
					break;
				case 1:
					res += L"в грудь";
					break;
				case 2:
					res += L"в живот";
					break;
				}
				SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res.c_str());
				LRESULT cur_sel2 = SendMessage(combo2, CB_GETCURSEL, 0, 0);
				LRESULT combo2_text = SendMessage(combo2, CB_GETLBTEXT, cur_sel2, (LPARAM)str2.c_str());
				if (kick[1] == cur_sel2)
				{
					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)L"Удар блокирован");
					if (kick[0])
					{
						health[1] -= 5;
						wchar_t temp[20] = L"Здоровье ", tempInt[10];
						_itow_s(health[1], tempInt, 10);
						wcscat_s(temp, tempInt);
						wcscat_s(temp, L"%");
						SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
					}
				}
				else
				{
					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)L"Удар пропущен");
					health[0] -= kick[0] ? 15 : 10;
					wchar_t temp[20] = L"Здоровье ", tempInt[10];
					_itow_s(health[0], tempInt, 10);
					wcscat_s(temp, tempInt);
					wcscat_s(temp, L"%");
					SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
				}
				ShowWindow(combo1, SW_NORMAL);																								//показать верхний комбобокс
				SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"Чем бить?");
				GetClientRect(hWnd, &cr);																									//узнать параметры клиенсткого окна
				SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 4, cr.bottom / 2.5, cr.right / 9, cr.bottom, SWP_SHOWWINDOW);					//сдвинуть нижний комбобокс на место
				SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Куда бить?");	
				SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Уарить!!!");																	//изменить текс на кнопке
			}
		}

		if (health[0] <= 0){
			MessageBox(hWnd, L"Вы проиграли!", L"Конец игры", MB_OK);
			PostQuitMessage(0);
			break;
		}

		if (health[1] <= 0){
			MessageBox(hWnd, L"Поздравляю! Вы победили!", L"Конец игры", MB_OK);
			PostQuitMessage(0);
			break;
		}

		break;

	case WM_CREATE:

		GetClientRect(hWnd, &cr);

		combo1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"combobox",
			L"",
			WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
			cr.right / 9 * 4,
			cr.bottom / 8 - 5,
			cr.right / 9,
			cr.bottom,
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
			cr.right / 9 * 4,
			cr.bottom / 2.5,
			cr.right / 9,
			cr.bottom,
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
			cr.right / 9 * 4,
			cr.bottom / 1.3,
			cr.right / 9,
			30,
			hWnd,
			(HMENU)ID_button1,
			hinst, NULL);

		list1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"listbox",
			L"",
			WS_CHILD | WS_VISIBLE | WS_VSCROLL,
			cr.right / 9,
			cr.bottom / 5,
			cr.right / 9 * 2,
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
			cr.right / 9 * 6,
			cr.bottom / 5,
			cr.right / 9 * 2,
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
			cr.right / 9,
			cr.bottom / 8,
			cr.right / 9 * 2,
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
			cr.right / 9 * 6,
			cr.bottom / 8,
			cr.right / 9 * 2,
			20,
			hWnd,
			(HMENU)ID_edit2,
			hinst,
			NULL);

		edit3 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"edit",
			L"Здоровье 100%",
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			cr.right / 9,
			cr.bottom / 1.3 + 5,
			cr.right / 9 * 2,
			20,
			hWnd,
			(HMENU)ID_edit3,
			hinst,
			NULL);

		edit4 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"edit",
			L"Здоровье 100%",
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			cr.right / 9 * 6,
			cr.bottom / 1.3 + 5,
			cr.right / 9 * 2,
			20,
			hWnd,
			(HMENU)ID_edit4,
			hinst,
			NULL);

		MessageBox(hWnd, L"Удачный удар рукой - 10%  здороья противника;\n\nУдачный удар ногой - 15%  здоровья противника;\n\nНеудачный (блокированный) удар ногой - 5%  здоровья НАПАДАЮЩЕГО.", L"Правила игры", MB_OK);

		
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
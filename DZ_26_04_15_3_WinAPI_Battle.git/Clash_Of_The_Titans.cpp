#include<Windows.h>
#include<tchar.h>
#include<ctime>
#include <fstream>

#define ID_combo1 1001
#define ID_combo2 1002
#define ID_button1 1010
#define ID_button2 1011
#define ID_button3 1012
#define ID_button4 1013
#define ID_list1 1020
#define ID_list2 1021
#define ID_edit1 1030
#define ID_edit2 1031
#define ID_edit3 1032
#define ID_edit4 1034
#define ID_edit5 1035
#define ID_edit6 1036
#define ID_edit6 1037

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"- = Б  И  Т  В  А      Т  И  Т  А  Н  О  В = -";
HINSTANCE hinst;
RECT desktop, cr;
wchar_t str1[20], str2[20], res[50], temp[200], tempInt[20], name[50];
bool move = true;															//Чей ход
int kick[2], block, health[2] = { 100, 100 }, repeat[3], score[2], record;	//Удар, блок, здоровье, повторить удар/блок, счет
LRESULT cur_sel, count;														//Позиция курсора, счетчик

HWND combo1, combo2, button1, button2, button3, button4, list1, list2, edit1, edit2, edit3, edit4, edit5, edit6, edit7;

class Player
{
public:
	Player(){};
	~Player(){};
	errno_t SaveToFile()
	{
		wchar_t arr[300];
		wchar_t temp[20];
		wcscpy_s(arr, sizeof (name), name);
		wcscat_s(arr, sizeof(L" "), L" ");
		_itow_s(score, temp, 10);
		wcscat_s(arr, sizeof(temp), temp);
		wcscat_s(arr, sizeof(L" "), L" ");
		_itow_s(record, temp, 10);
		wcscat_s(arr, sizeof(temp), temp);
		wcscat_s(arr, sizeof(L"\n"), L"\n");

		FILE* file;
		errno_t err;
		err = _wfopen_s(&file, L"record.txt", L"a");

		if (!err)
		{
			fputws(arr, file);

			fclose(file);
		}
		return err;
	}
	errno_t ReadFromFile()
	{}

	void SetName(wchar_t * _name)
	{
		wcscpy_s(name, sizeof(_name), _name);
	}

	void SetScore(int * _score)
	{
		score = _score[0] + _score[1];
	}

	void SetRecord(int _record)
	{
		record = _record;
	}

private:
	wchar_t name[51];
	int score;
	int record;
};


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
		960,
		740,
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
					   //Получить имя игрока
					   SendMessage(edit5, WM_GETTEXT, sizeof(name), (LPARAM)name);

					   //удар=====================================================================
					   //Чем бить
					   if (LOWORD(wParam) == ID_combo1 && HIWORD(wParam) == CBN_SELENDOK && move)
					   {
						   cur_sel = SendMessage(combo1, CB_GETCURSEL, 0, 0);
						   SendMessage(combo1, CB_GETLBTEXT, cur_sel, (LPARAM)str1);
						   count = SendMessage(list1, LB_GETCOUNT, 0, 0);
						   _itow_s(count + 1, temp, 10);
						   wcscpy_s(res, temp);
						   wcscat_s(res, L". ");
						   wcscat_s(res, str1);
						   kick[0] = repeat[0] = cur_sel;
					   }
					   //Куда бить
					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK && move)
					   {
						   cur_sel = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   SendMessage(combo2, CB_GETLBTEXT, cur_sel, (LPARAM)str2);
						   wcscat_s(res, L" ");
						   wcscat_s(res, str2);
						   kick[1] = repeat[1] = cur_sel;
					   }

					   //Блок=====================================================================
					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK && !move)
					   {
						   cur_sel = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   SendMessage(combo2, CB_GETLBTEXT, cur_sel, (LPARAM)str2);
						   block = repeat[2] = cur_sel;
					   }
	}

	case BN_CLICKED:

		if (LOWORD(wParam) == ID_button3)
			MessageBox(hWnd, L"Удачный удар рукой - 10%  здороья противника;\n\nУдачный удар ногой - 15%  здоровья противника;\n\nНеудачный (блокированный) удар ногой - 5%  здоровья НАПАДАЮЩЕГО.", L"Правила игры", MB_OK | MB_ICONASTERISK);

		if (LOWORD(wParam) == ID_button4)



			MessageBox(hWnd, L"", L"Таблица рекордов", MB_OK | MB_ICONASTERISK);

		if (LOWORD(wParam) == ID_button2)
		{
			if (move)
			{
				_itow_s(count++, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". ");
				kick[0] = repeat[0];
				kick[0] ? wcscpy_s(str1, L"Ногой") : wcscpy_s(str1, L"Рукой");
				wcscat_s(res, str1);

				kick[1] = repeat[1];
				switch (kick[1])
				{
				case 0:
					wcscpy_s(str2, L" в голову");
					break;
				case 1:
					wcscpy_s(str2, L" в грудь");
					break;
				case 2:
					wcscpy_s(str2, L" в живот");
					break;
				}
				wcscat_s(res, str2);
			}
			else
			{
				block = repeat[2];
				switch (block)
				{
				case 0:
					wcscpy_s(str2, L" в голову");
					break;
				case 1:
					wcscpy_s(str2, L" в грудь");
					break;
				case 2:
					wcscpy_s(str2, L" в живот");
					break;
				}
			}

			wParam = ID_button1;

		}

		if (LOWORD(wParam) == ID_button1 && move)
		{
			if (name[0] == L'\0' || !wcscmp(name, L"Введите имя игрока"))
			{
				MessageBox(hWnd, L"Введите имя игрока", L"Ошибка!", MB_OK | MB_ICONHAND);
				break;
			}
			else
			{
				Player Name;
				Name.SetName(name);
				SendMessage(edit5, EM_SETREADONLY, TRUE, 0);
			}
			//Удар ==========================================================================================================================================================
			if (move){	//Защита от дурака
				if (str1[0] == L'\0' && str2[0] == L'\0')
					MessageBox(hWnd, L"Не выбрано чем и куда бить!", L"Ошибка!", MB_OK | MB_ICONHAND);
				else if (str1[0] == '\0' && str2[0] != '\0')
					MessageBox(hWnd, L"Не выбрано чем бить!", L"Ошибка!", MB_OK | MB_ICONHAND);
				else if (str1[0] != '\0' && str2[0] == '\0')
					MessageBox(hWnd, L"Не выбрано куда бить!", L"Ошибка!", MB_OK | MB_ICONHAND);
				else //Если все выбрано корректно
				{
					//Формирую строку "Чем и куда ударил" для записи в окно игрока
					_itow_s(count + 1, temp, 10);
					wcscpy_s(res, temp);
					wcscat_s(res, L". ");
					wcscat_s(res, str1);
					wcscat_s(res, L" ");
					wcscat_s(res, str2);

					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);																			//добавить текст в список
					SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);															//автопрокрутка к последнему элементу
					ShowWindow(combo1, SW_HIDE);																								//скрыть верхний комбобокс
					GetClientRect(hWnd, &cr);																									//узнать параметры клиенсткого окна
					SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 3.75, cr.bottom / 2.5, cr.right / 9 * 1.5, cr.bottom, SWP_SHOWWINDOW);		//сдвинуть нижний комбобокс
					SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Блокировать удар");															//изменить текст в комбобоксе (подсказку)
					SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Блокировать");																//изменить текс на кнопке

					//Блокирование удара компом============================================================================================================================
					//Выбираю что блокировать
					srand((unsigned)time(NULL));
					int block = rand() % 3;

					if (kick[1] == block) //Удар блокирован
					{
						count = SendMessage(list2, LB_GETCOUNT, 0, 0);
						_itow_s(count + 1, temp, 10);
						wcscpy_s(res, temp);
						wcscat_s(res, L". Удар блокирован");
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
						SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
						if (kick[0]) //Если игрок бил ногой - уменьшаю его здоровье на 5%. Иначе будет дамажить все время ногой :))
						{
							health[0] -= 5;
							wcscpy_s(temp, L"Здоровье ");
							_itow_s(health[0], tempInt, 10);
							wcscat_s(temp, tempInt);
							wcscat_s(temp, L"%");
							SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
						}
					}
					else //Усли убар пропущен
					{
						count = SendMessage(list2, LB_GETCOUNT, 0, 0);
						_itow_s(count + 1, temp, 10);
						wcscpy_s(res, temp);
						wcscat_s(res, L". Удар пропущен (защищал ");
						switch (block)
						{
						case 0:
							wcscat_s(res, L"голову)");
							break;
						case 1:
							wcscat_s(res, L"грудь)");
							break;
						case 2:
							wcscat_s(res, L"живот)");
							break;

						}
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
						SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
						health[1] -= kick[0] ? 15 : 10;
						record += kick[0] ? 15 : 10;
						Player Record;
						Record.SetRecord(record);
						wcscpy_s(temp, L"Здоровье ");
						_itow_s(health[1], tempInt, 10);
						wcscat_s(temp, tempInt);
						wcscat_s(temp, L"%");
						SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
						wcscpy_s(temp, L"Ваш рекорд: ");
						_itow_s(record, tempInt, 10);
						wcscat_s(temp, tempInt);
						SendMessage(edit6, WM_SETTEXT, 0, (LPARAM)temp);
					}
					str1[0] = str2[0] = res[0] = temp[0] = tempInt[0] = '\0';
					move = false;	//Переход хода
					wParam = 0;		//Обнуляю wParam, чтобы вернуть управление  WM_COMMAND
					SendMessage(button2, WM_SETTEXT, 0, (LPARAM)L"Повторить блок!");
				}
			}
		}
		//Блок==================================================================================================================
		if (LOWORD(wParam) == ID_button1 && !move){

			if (str2[0] == '\0')
				MessageBox(hWnd, L"Не выбрано что блокировать", L"Ошибка!", MB_OK | MB_ICONHAND);

			else
			{
				//Формирую удар компа
				kick[0] = rand() % 2;	//Чем бить
				kick[1] = rand() % 3;	//Куда бить

				count = SendMessage(list2, LB_GETCOUNT, 0, 0);
				_itow_s(count + 1, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". ");

				//Формирую строку для окна компа (чем и куда ударил)
				if (kick[0])
					wcscat_s(res, L"Ногой ");
				else
					wcscat_s(res, L"Рукой ");

				switch (kick[1])
				{
				case 0:
					wcscat_s(res, L"в голову");
					break;
				case 1:
					wcscat_s(res, L"в грудь");
					break;
				case 2:
					wcscat_s(res, L"в живот");
					break;
				}
				SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
				SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);

			}

			if (kick[1] == block && str2[0] != '\0')
			{
				count = SendMessage(list1, LB_GETCOUNT, 0, 0);
				_itow_s(count + 1, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". Удар блокирован");
				SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);
				SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
				if (kick[0])
				{
					health[1] -= 5;
					wcscpy_s(temp, L"Здоровье ");
					_itow_s(health[1], tempInt, 10);
					wcscat_s(temp, tempInt);
					wcscat_s(temp, L"%");
					SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
					record += 5;
					wcscpy_s(temp, L"Ваш рекорд: ");
					_itow_s(record, tempInt, 10);
					wcscat_s(temp, tempInt);
					SendMessage(edit6, WM_SETTEXT, 0, (LPARAM)temp);
				}
			}
			else if (kick[1] != block && str2[0] != '\0')
			{
				count = SendMessage(list1, LB_GETCOUNT, 0, 0);
				_itow_s(count + 1, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". Удар пропущен (защищал ");
				switch (cur_sel)
				{
				case 0:
					wcscat_s(res, L"голову)");
					break;
				case 1:
					wcscat_s(res, L"грудь)");
					break;
				case 2:
					wcscat_s(res, L"живот)");
					break;
				}
				SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);
				SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
				health[0] -= kick[0] ? 15 : 10;
				wcscpy_s(temp, L"Здоровье ");
				_itow_s(health[0], tempInt, 10);
				wcscat_s(temp, tempInt);
				wcscat_s(temp, L"%");
				SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
			}

			if (str2[0] != '\0' && health[0] > 0 && health[1] > 0)
			{
				ShowWindow(combo1, SW_NORMAL);																								//показать верхний комбобокс
				SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"Чем бить?");
				GetClientRect(hWnd, &cr);																									//узнать параметры клиенсткого окна
				SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 4, cr.bottom / 2.5, cr.right / 9, cr.bottom, SWP_SHOWWINDOW);					//сдвинуть нижний комбобокс на место
				SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Куда бить?");
				SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Ударить!!!");																	//изменить текс на кнопке
				str1[0] = str2[0] = res[0] = temp[0] = tempInt[0] = '\0';
				move = true;
				wParam = 0;
				EnableWindow(button2, TRUE);																								//активировать кнопку "Повторить"
				SendMessage(button2, WM_SETTEXT, 0, (LPARAM)L"Повторить удар!");
			}

			if (health[0] <= 0 || health[1] <= 0)																							//Конец игры
			{
				int Quit = MessageBox(hWnd, health[1] > 0 ? L"Вы проиграли!\n\nХотите сыграть еще?" : L"Поздравляю! Вы победили!\n\nХотите сыграть еще?", L"Конец игры", MB_YESNO);
				if (Quit == IDNO)
				{
					Player Save;
					Save.SaveToFile();
					PostQuitMessage(0);
					break;
				}
				else
				{
					SendMessage(list1, LB_RESETCONTENT, 0, 0);
					SendMessage(list2, LB_RESETCONTENT, 0, 0);
					SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)L"Здоровье 100%");
					SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)L"Здоровье 100%");
					ShowWindow(combo1, SW_NORMAL);																								//показать верхний комбобокс
					SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"Чем бить?");
					GetClientRect(hWnd, &cr);																									//узнать параметры клиенсткого окна
					SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 4, cr.bottom / 2.5, cr.right / 9, cr.bottom, SWP_SHOWWINDOW);					//сдвинуть нижний комбобокс на место
					SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"Куда бить?");
					SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"Ударить!!!");																	//изменить текс на кнопке
					SendMessage(button2, WM_SETTEXT, 0, (LPARAM)L"Повторить удар!");
					EnableWindow(button2, FALSE);
					health[1] > 0 ? score[1]++ : score[0]++;
					Player Score;
					Score.SetScore(score);
					_itow_s(score[0], temp, 10);
					_itow_s(score[1], tempInt, 10);
					wcscpy_s(res, L"Счет ");
					wcscat_s(res, temp);
					wcscat_s(res, L" : ");
					wcscat_s(res, tempInt);
					SendMessage(edit7, WM_SETTEXT, 0, (LPARAM)res);
					health[0] = health[1] = 100;
					str1[0] = str2[0] = res[0] = temp[0] = tempInt[0] = '\0';
					move = true;
					count = 0;
					wParam = 0;
					message = WM_COMMAND;
					break;
				}
			}
		}
		break;

	case WM_CREATE:
	{
					  GetClientRect(hWnd, &cr);

					  combo1 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"combobox",
						  L"",
						  WS_CHILD | WS_VISIBLE | CBS_DROPDOWN,
						  cr.right / 9 * 4,
						  cr.bottom / 5,
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

					  button2 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"button",
						  L"Повторить удар!",
						  WS_CHILD | WS_VISIBLE,
						  cr.right / 8,
						  cr.bottom / 1.15,
						  cr.right / 9 * 2,
						  30,
						  hWnd,
						  (HMENU)ID_button2,
						  hinst, NULL);

					  EnableWindow(button2, FALSE);

					  button3 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"button",
						  L"Правила",
						  WS_CHILD | WS_VISIBLE,
						  cr.right / 9 * 4,
						  cr.bottom / 1.15,
						  cr.right / 9,
						  30,
						  hWnd,
						  (HMENU)ID_button3,
						  hinst, NULL);

					  button4 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"button",
						  L"Таблица рекордов",
						  WS_CHILD | WS_VISIBLE,
						  cr.right / 9.3 * 6,
						  cr.bottom / 1.15,
						  cr.right / 9 * 2,
						  30,
						  hWnd,
						  (HMENU)ID_button4,
						  hinst, NULL);

					  list1 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"listbox",
						  L"",
						  WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY,
						  cr.right / 12,
						  cr.bottom / 5,
						  cr.right / 6.5 * 2,
						  cr.bottom / 2,
						  hWnd,
						  (HMENU)ID_list1,
						  hinst,
						  NULL);

					  list2 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"listbox",
						  L"",
						  WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_READONLY,
						  cr.right / 10 * 6,
						  cr.bottom / 5,
						  cr.right / 8 * 2.5,
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
						  cr.right / 8,
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
						  L"Хилая тушка противника",
						  WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
						  cr.right / 9.3 * 6,
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
						  cr.right / 8,
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
						  cr.right / 9.3 * 6,
						  cr.bottom / 1.3 + 5,
						  cr.right / 9 * 2,
						  20,
						  hWnd,
						  (HMENU)ID_edit4,
						  hinst,
						  NULL);

					  edit5 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"edit",
						  L"Введите имя игрока",
						  WS_CHILD | WS_VISIBLE | ES_CENTER,
						  cr.right / 8,
						  cr.bottom / 14,
						  cr.right / 9 * 2,
						  20,
						  hWnd,
						  (HMENU)ID_edit5,
						  hinst,
						  NULL);

					  edit6 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"edit",
						  L"Ваш рекорд: 0",
						  WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
						  cr.right / 9.3 * 6,
						  cr.bottom / 14,
						  cr.right / 9 * 2,
						  20,
						  hWnd,
						  (HMENU)ID_edit6,
						  hinst,
						  NULL);

					  edit7 = CreateWindowEx(
						  WS_EX_CLIENTEDGE,
						  L"edit",
						  L"Счет 0 : 0",
						  WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
						  cr.right / 9 * 4,
						  cr.bottom / 14,
						  cr.right / 9,
						  20,
						  hWnd,
						  (HMENU)ID_edit6,
						  hinst,
						  NULL);

					  break;
	}
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
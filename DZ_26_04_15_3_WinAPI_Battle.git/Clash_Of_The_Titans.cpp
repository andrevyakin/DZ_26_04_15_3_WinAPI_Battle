#include<Windows.h>
#include<tchar.h>
#include<ctime>

#define ID_combo1 1001
#define ID_combo2 1002
#define ID_button1 1010
#define ID_list1 1020
#define ID_list2 1021
#define ID_edit1 1030
#define ID_edit2 1031
#define ID_edit3 1032
#define ID_edit4 1034

static TCHAR WindowsClass[] = L"win32app";
static TCHAR Title[] = L"�  �  �  �  �      �  �  �  �  �  �  �";
HINSTANCE hinst;
RECT desktop, cr;
wchar_t str1[20], str2[20], res[50], temp[20], tempInt[20];
bool move = true;													//��� ���
int kick[2], block, health[2] = { 100, 100 };						//����, ����, ��������
LRESULT cur_sel, count;												//������� �������, �������

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
		960,
		540,
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
					   //����=====================================================================
					   if (LOWORD(wParam) == ID_combo1 && HIWORD(wParam) == CBN_SELENDOK && move)
					   {
						   cur_sel = SendMessage(combo1, CB_GETCURSEL, 0, 0);
						   SendMessage(combo1, CB_GETLBTEXT, cur_sel, (LPARAM)str1);
						   count = SendMessage(list1, LB_GETCOUNT, 0, 0);
						   _itow_s(count + 1, temp, 10);
						   wcscpy_s(res, temp);
						   wcscat_s(res, L". ");
						   wcscat_s(res, str1);
						   kick[0] = cur_sel;
					   }

					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK && move)
					   {
						   cur_sel = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   SendMessage(combo2, CB_GETLBTEXT, cur_sel, (LPARAM)str2);
						   wcscat_s(res, L" ");
						   wcscat_s(res, str2);
						   kick[1] = cur_sel;
					   }

					   //����=====================================================================
					   if (LOWORD(wParam) == ID_combo2 && HIWORD(wParam) == CBN_SELENDOK && !move)
					   {
						   cur_sel = SendMessage(combo2, CB_GETCURSEL, 0, 0);
						   SendMessage(combo2, CB_GETLBTEXT, cur_sel, (LPARAM)str2);
						   block = cur_sel;
					   }
	}

	case BN_CLICKED:

		if (LOWORD(wParam) == ID_button1 && move){
			//���� ==========================================================================================================================================================
			if (move){	//������ �� ������
				if (str1[0] == L'\0' && str2[0] == L'\0')
					MessageBox(hWnd, L"�� ������� ��� � ���� ����!", L"������!", MB_OK);
				else if (str1[0] == '\0' && str2[0] != '\0')
					MessageBox(hWnd, L"�� ������� ��� ����!", L"������!", MB_OK);
				else if (str1[0] != '\0' && str2[0] == '\0')
					MessageBox(hWnd, L"�� ������� ���� ����!", L"������!", MB_OK);
				else //���� ��� ������� ���������
				{
					//�������� ������ "��� � ���� ������" ��� ������ � ���� ������
					_itow_s(count + 1, temp, 10);
					wcscpy_s(res, temp);
					wcscat_s(res, L". ");
					wcscat_s(res, str1);
					wcscat_s(res, L" ");
					wcscat_s(res, str2);

					SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);																			//�������� ����� � ������
					SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);															//������������� � ���������� ��������
					ShowWindow(combo1, SW_HIDE);																								//������ ������� ���������
					GetClientRect(hWnd, &cr);																									//������ ��������� ����������� ����
					SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 3.75, cr.bottom / 2.5, cr.right / 9 * 1.5, cr.bottom, SWP_SHOWWINDOW);		//�������� ������ ���������
					SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"����������� ����");															//�������� ����� � ���������� (���������)
					SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"�����������");																//�������� ���� �� ������

					//������������ ����� ������============================================================================================================================
					//������� ��� �����������
					srand((unsigned)time(NULL));
					int block = rand() % 3;

					if (kick[1] == block) //���� ����������
					{
						count = SendMessage(list2, LB_GETCOUNT, 0, 0);
						_itow_s(count + 1, temp, 10);
						wcscpy_s(res, temp);
						wcscat_s(res, L". ���� ����������");
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
						SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
						if (kick[0]) //���� ����� ��� ����� - �������� ��� �������� �� 5%. ����� ����� �������� ��� ����� ����� :))
						{
							health[0] -= 5;
							wcscpy_s(temp, L"�������� ");
							_itow_s(health[0], tempInt, 10);
							wcscat_s(temp, tempInt);
							wcscat_s(temp, L"%");
							SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
						}
					}
					else //���� ���� ��������
					{
						count = SendMessage(list2, LB_GETCOUNT, 0, 0);
						_itow_s(count + 1, temp, 10);
						wcscpy_s(res, temp);
						wcscat_s(res, L". ���� �������� (������� ");
						switch (block)
						{
						case 0:
							wcscat_s(res, L"������)");
							break;
						case 1:
							wcscat_s(res, L"�����)");
							break;
						case 2:
							wcscat_s(res, L"�����)");
							break;

						}
						SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
						SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
						health[1] -= kick[0] ? 15 : 10;
						wcscpy_s(temp, L"�������� ");
						_itow_s(health[1], tempInt, 10);
						wcscat_s(temp, tempInt);
						wcscat_s(temp, L"%");
						SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
					}
					str1[0] = str2[0] = res[0] = temp[0] = tempInt[0] = '\0';
					move = false;	//������� ����
					wParam = 0;		//������� wParam, ����� ������� ����������  WM_COMMAND
				}
			}
		}
		//����==================================================================================================================
		if (LOWORD(wParam) == ID_button1 && !move){
			////�������� ���� �����
			//kick[0] = rand() % 2;	//��� ����
			//kick[1] = rand() % 3;	//���� ����
			//
			//count = SendMessage(list2, LB_GETCOUNT, 0, 0);
			//_itow_s(count + 1, temp, 10);
			//wcscpy_s(res, temp);
			//wcscat_s(res, L". ");

			////�������� ������ ��� ���� ����� (��� � ���� ������)
			//if (kick[0])
			//	wcscat_s (res, L"����� ");
			//else
			//	wcscat_s (res, L"����� ");

			//switch (kick[1])
			//{
			//case 0:
			//	wcscat_s (res, L"� ������");
			//	break;
			//case 1:
			//	wcscat_s (res, L"� �����");
			//	break;
			//case 2:
			//	wcscat_s (res, L"� �����");
			//	break;
			//}
			//SendMessage(list2, LB_ADDSTRING, 0, (LPARAM)res);
			//SendMessage(list2, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);

			////////////////////////////////////////////////////////////////////				

			if (str2[0] == '\0')
				MessageBox(hWnd, L"�� ������� ��� �����������", L"������!", MB_OK);

			else
			{
				//�������� ���� �����
				kick[0] = rand() % 2;	//��� ����
				kick[1] = rand() % 3;	//���� ����

				count = SendMessage(list2, LB_GETCOUNT, 0, 0);
				_itow_s(count + 1, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". ");

				//�������� ������ ��� ���� ����� (��� � ���� ������)
				if (kick[0])
					wcscat_s(res, L"����� ");
				else
					wcscat_s(res, L"����� ");

				switch (kick[1])
				{
				case 0:
					wcscat_s(res, L"� ������");
					break;
				case 1:
					wcscat_s(res, L"� �����");
					break;
				case 2:
					wcscat_s(res, L"� �����");
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
				wcscat_s(res, L". ���� ����������");
				SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);
				SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
				if (kick[0])
				{
					health[1] -= 5;
					wcscpy_s(temp, L"�������� ");
					_itow_s(health[1], tempInt, 10);
					wcscat_s(temp, tempInt);
					wcscat_s(temp, L"%");
					SendMessage(edit4, WM_SETTEXT, 0, (LPARAM)temp);
				}
			}
			else if (kick[1] != block && str2[0] != '\0')
			{
				count = SendMessage(list1, LB_GETCOUNT, 0, 0);
				_itow_s(count + 1, temp, 10);
				wcscpy_s(res, temp);
				wcscat_s(res, L". ���� �������� (������� ");
				switch (cur_sel)
				{
				case 0:
					wcscat_s(res, L"������)");
					break;
				case 1:
					wcscat_s(res, L"�����)");
					break;
				case 2:
					wcscat_s(res, L"�����)");
					break;
				}
				SendMessage(list1, LB_ADDSTRING, 0, (LPARAM)res);
				SendMessage(list1, WM_VSCROLL, MAKEWPARAM(SB_BOTTOM, NULL), NULL);
				health[0] -= kick[0] ? 15 : 10;
				wcscpy_s(temp, L"�������� ");
				_itow_s(health[0], tempInt, 10);
				wcscat_s(temp, tempInt);
				wcscat_s(temp, L"%");
				SendMessage(edit3, WM_SETTEXT, 0, (LPARAM)temp);
			}

			if (str2[0] != '\0')
			{
				ShowWindow(combo1, SW_NORMAL);																								//�������� ������� ���������
				SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"��� ����?");
				GetClientRect(hWnd, &cr);																									//������ ��������� ����������� ����
				SetWindowPos(combo2, HWND_TOP, cr.right / 9 * 4, cr.bottom / 2.5, cr.right / 9, cr.bottom, SWP_SHOWWINDOW);					//�������� ������ ��������� �� �����
				SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"���� ����?");
				SendMessage(button1, WM_SETTEXT, 0, (LPARAM)L"�������!!!");																	//�������� ���� �� ������
				str1[0] = str2[0] = res[0] = temp[0] = tempInt[0] = '\0';
				move = true;
				wParam = 0;
			}
		}

		if (health[0] <= 0){
			MessageBox(hWnd, L"�� ���������!", L"����� ����", MB_OK);
			PostQuitMessage(0);
			break;
		}

		if (health[1] <= 0){
			MessageBox(hWnd, L"����������! �� ��������!", L"����� ����", MB_OK);
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

		SendMessage(combo1, WM_SETTEXT, 0, (LPARAM)L"��� ����?");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"����� ");
		SendMessage(combo1, CB_ADDSTRING, 0, (LPARAM)L"����� ");

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

		SendMessage(combo2, WM_SETTEXT, 0, (LPARAM)L"���� ����?");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"� ������");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"� �����");
		SendMessage(combo2, CB_ADDSTRING, 0, (LPARAM)L"� �����");

		button1 = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			L"button",
			L"�������!!!",
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
			L"���� ������ ����",
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
			L"����� ����� ����������",
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
			L"�������� 100%",
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
			L"�������� 100%",
			WS_CHILD | WS_VISIBLE | ES_CENTER | ES_READONLY,
			cr.right / 9.3 * 6,
			cr.bottom / 1.3 + 5,
			cr.right / 9 * 2,
			20,
			hWnd,
			(HMENU)ID_edit4,
			hinst,
			NULL);

		MessageBox(hWnd, L"������� ���� ����� - 10%  ������� ����������;\n\n������� ���� ����� - 15%  �������� ����������;\n\n��������� (�������������) ���� ����� - 5%  �������� �����������.", L"������� ����", MB_OK);


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
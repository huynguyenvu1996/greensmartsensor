// P2.cpp : Defines the entry point for the application.
//
/*https://msdn.microsoft.com/en-us/library/windows/desktop/hh298345(v=vs.85).aspx */
/*http://www.codeproject.com/Articles/2890/Using-ListView-control-under-Win-API */
#include "stdafx.h"
#include "P2.h"
#include <Commctrl.h>
#include<stdio.h>
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<string>
#include <commdlg.h>
#include <shellapi.h>
#include<Mmsystem.h>
#include<vector>
#include<algorithm>
#include<ctime>
#include<cstdlib>
#include <locale>
#include <codecvt>
using namespace std;

#define MAX_LOADSTRING 100
struct TIMEPLAY
{
	int hour;
	int minute;
	int second;
};
int randomFuction(int i) 
{ 
	return std::rand() % i; 
}
//Hàm random các giá trị trong vector 
void doRandomVector(vector<int>&iVector)
{
	std::srand(unsigned(std::time(0)));
	std::random_shuffle(iVector.begin(), iVector.end(), randomFuction);
}
//Hàm kiểm tra xem một phần tử x có nằm trong vector hay không
bool hasInVector(vector<int> Vec, int x)
{
	if (Vec.empty()==true){
		return false;
	}
	if (std::find(Vec.begin(), Vec.end(), x) != Vec.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}
HICON hiconItem;     // Icon for list-view items.
// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
LV_COLUMN listViewCol;
LVITEM listViewItem;
int number_image=6;
int play_music = 0;
int loop_music = 0;
wstring path_music;
DWORD countCol;
TCHAR path_music_dialog_open[256];
TIMEPLAY count_time;
MCIDEVICEID	mciWaveDevID = NULL;
bool is_loop = false;
bool is_open_file = false;
bool is_play_music = false;
//Kiểm tra xem người dùng đã nhấn play hay chưa
bool is_play = false;
int countScore = 0;
HIMAGELIST hLarge;   // Image list for icon view.
HIMAGELIST hSmall;   // Image list for other views

vector<int>List_ID_Image;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DialogGameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
BOOL				InitListViewImageLists(HWND hWndListView);
BOOL				InitListViewColumns(HWND hWndListView);
void				doCreate_ListView(HWND hDlg, HWND hWndListView);
BOOL				InsertListViewItems(HWND hWndListView, int cItems);
bool				readFileConfigure();
bool				writeFileConfigure();
LPWSTR				selectMusicFile(HWND hDlg);
bool				isWaveFile(TCHAR* szFileName);
void				doMCICommandMessageDevice(HWND hDlg, TCHAR* szFileName);
void				doMCICommandMessagePlay(HWND hDlg);
void				doMCICommandMessageStop(HWND hFrameWnd);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_P2));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
	
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hDlg;
	hInst = hInstance; // Store instance handle in our global variable
	hDlg = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG_GAME), NULL, DialogGameProc);
	if (!hDlg)
	{
		return FALSE;
	}

	ShowWindow(hDlg, nCmdShow);
	UpdateWindow(hDlg);
	return TRUE;
}
// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP hOldBmp;
	HBITMAP hBmp;
	BITMAP bm;
	HDC memDC;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK||LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hDlg, &ps);
		hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_ABOUT));
		RECT rec;
		GetClientRect(hDlg, &rec);
		if (hBmp == NULL)
		{
			MessageBox(hDlg, L"Fail to loading bitmap!", L"Result", MB_OK);
		}
		GetObject(hBmp, sizeof(BITMAP), &bm);
		memDC = CreateCompatibleDC(hdc);
		hOldBmp = (HBITMAP)SelectObject(memDC, hBmp);
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, memDC, 0, 0, SRCCOPY);
		SelectObject(memDC, hOldBmp);
		DeleteDC(memDC);
		EndPaint(hDlg, &ps);
		break;
	}

	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK DialogGameProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HWND hListCrtl;
	static TCHAR buff[128];
	static TCHAR c_time[128];
	static TCHAR c_score[128];
	static TCHAR c_image[128];
	static int choose_one = -1;
	static int choose_two = -1;
	static vector<int>saveChoose;
	static bool check_reade_file;
	static bool isWinner = false;
	bool checkBox = false;
	static bool checkOpenFile = false;
	static bool is_fist_openfile_music = false;

	//Load hình
	HDC hdc;
	PAINTSTRUCT ps;
	HBITMAP hOldBmp;
	HBITMAP hBmp;
	BITMAP bm;
	HDC memDC;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
	{
		hListCrtl = GetDlgItem(hDlg, IDC_LIST_PLAY);
		check_reade_file = readFileConfigure();
		 wsprintf(buff, L"%d", number_image);
		SendDlgItemMessage(hDlg, IDC_EDIT_NUMBER_IMAGE, WM_SETTEXT, NULL, (LPARAM)buff);
		if (path_music != L"\0")
		{
			SendDlgItemMessage(hDlg, IDC_STATIC_PATH, WM_SETTEXT, NULL, (LPARAM)path_music.c_str());
		}

		if (play_music == 1)
		{
			CheckDlgButton(hDlg, IDC_CHECK_PLAY_MUSIC, TRUE);
		}
		if (loop_music == 0)
		{
			CheckRadioButton(hDlg, IDC_RADIO_NO_LOOP, IDC_RADIO_LOOP, IDC_RADIO_NO_LOOP);
		}
		else
		{
			CheckRadioButton(hDlg, IDC_RADIO_NO_LOOP, IDC_RADIO_LOOP, IDC_RADIO_LOOP);
		}
		checkBox = IsDlgButtonChecked(hDlg, IDC_CHECK_PLAY_MUSIC);
		if (checkBox == false)
		{
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO_NO_LOOP), false);
			EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LOOP), false);
			EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_OPEN_FILE), false);
		}
		return (INT_PTR)TRUE;
	}
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_PLAY_GAME:
			//-----------------------------------------------------
			//Khởi tạo lại toàn bộ dữ liệu khi nhấn play
			//Xem người dùng có mở file wav bằng dialog open không
			if (play_music == 1)
			{
				if (checkOpenFile == false)
				{
					wsprintf(path_music_dialog_open, path_music.c_str());
				}
				if (mciWaveDevID != NULL)
				{
					doMCICommandMessageStop(hDlg);
					mciSendCommand(mciWaveDevID, MCI_CLOSE, 0, 0L);
					mciWaveDevID = NULL;
				}

				if (mciWaveDevID == NULL)
				{
					doMCICommandMessageDevice(hDlg, path_music_dialog_open);
				}
				doMCICommandMessagePlay(hDlg);
			}


			choose_one = -1;
			choose_two = -1;
			is_play = true;
			isWinner = false;
			SetTimer(hDlg, IDC_TIMER, 1000, NULL);
			count_time.hour = 0;
			count_time.minute = 0;
			count_time.second = 0;
			saveChoose.clear();
			countScore = 0;
			List_ID_Image.clear();
			SendDlgItemMessage(hDlg, IDC_SCORE, WM_SETTEXT, 0, (LPARAM)L"0");
			SetFocus(hListCrtl);
			ListView_DeleteItem(hListCrtl, ListView_GetSelectionMark(hListCrtl));
			SendMessage(hListCrtl, LVM_DELETEALLITEMS, 0, 0);
			//-----------------------------------------------


			SendDlgItemMessage(hDlg, IDC_EDIT_NUMBER_IMAGE, WM_GETTEXT, (WPARAM)sizeof(c_image), (LPARAM)c_image);
			number_image = _ttoi(c_image);
			if (1 <= number_image && number_image <= 10)
			{
				doCreate_ListView(hDlg, hListCrtl);
				if (play_music == 1)
				{
					CheckDlgButton(hDlg, IDC_CHECK_PLAY_MUSIC, TRUE);
					//Xem người dùng có mở file wav bằng dialog open không
					if (checkOpenFile == false)
					{
						wsprintf(path_music_dialog_open, path_music.c_str());
					}
					SendDlgItemMessage(hDlg, IDC_STATIC_PATH, WM_SETTEXT, NULL, (LPARAM)path_music_dialog_open);
					if (wcscmp(path_music_dialog_open, L"\0"))
					{
						if (mciWaveDevID == NULL)
						{
							doMCICommandMessageDevice(hDlg, path_music_dialog_open);
						}
						doMCICommandMessagePlay(hDlg);
						is_play_music = true;
					}
				}
				if (loop_music == 0)
				{
					is_loop == false;
					CheckRadioButton(hDlg, IDC_RADIO_NO_LOOP, IDC_RADIO_LOOP, IDC_RADIO_NO_LOOP);
				}
				else
				{
					is_loop = true;
					CheckRadioButton(hDlg, IDC_RADIO_NO_LOOP, IDC_RADIO_LOOP, IDC_RADIO_LOOP);
				}
				checkBox = IsDlgButtonChecked(hDlg, IDC_CHECK_PLAY_MUSIC);
				if (checkBox == false)
				{
					EnableWindow(GetDlgItem(hDlg, IDC_RADIO_NO_LOOP), false);
					EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LOOP), false);
				}
			}
			else
			{
				MessageBox(hDlg, L"Số hình phải nằm trong khoảng [1,10]", L"Thông báo", MB_OK | MB_ICONERROR);
			}
			is_fist_openfile_music = true;
			return (INT_PTR)TRUE;
			break;
		case IDC_CHECK_PLAY_MUSIC:
			checkBox = IsDlgButtonChecked(hDlg, IDC_CHECK_PLAY_MUSIC);
			if (checkBox == true)
			{
				play_music = 1;
				if (wcscmp(path_music_dialog_open, L"\0") && is_play==true)
				{
					is_play_music = true;
					if (mciWaveDevID == NULL)
					{
						doMCICommandMessageDevice(hDlg, path_music_dialog_open);
					}
					doMCICommandMessagePlay(hDlg);
				}
				EnableWindow(GetDlgItem(hDlg, IDC_RADIO_NO_LOOP), true);
				EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LOOP), true);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_OPEN_FILE), true);
			}
			else
			{
				play_music = 0;
				is_play_music = false;
				if (mciWaveDevID != NULL)
				{
					doMCICommandMessageStop(hDlg);
					mciSendCommand(mciWaveDevID, MCI_CLOSE, 0, 0L);
					mciWaveDevID = NULL;
				}
				EnableWindow(GetDlgItem(hDlg, IDC_RADIO_NO_LOOP), false);
				EnableWindow(GetDlgItem(hDlg, IDC_RADIO_LOOP), false);
				EnableWindow(GetDlgItem(hDlg, IDC_BUTTON_OPEN_FILE), false);
			}
			return (INT_PTR)TRUE;
			break;
		case IDC_BUTTON_OPEN_FILE:
			 wcscpy(path_music_dialog_open, selectMusicFile(hDlg));
			 if (wcscmp(path_music_dialog_open, L"\0") )
			{
				 SendDlgItemMessage(hDlg, IDC_STATIC_PATH, WM_SETTEXT, NULL, (LPARAM)path_music_dialog_open);
				 if (is_fist_openfile_music == true)
				 {
					 if (mciWaveDevID != NULL)
					 {
						 doMCICommandMessageStop(hDlg);
						 mciSendCommand(mciWaveDevID, MCI_CLOSE, 0, 0L);
						 mciWaveDevID = NULL;
					 }
					
					 if (mciWaveDevID == NULL)
					 {
						 doMCICommandMessageDevice(hDlg, path_music_dialog_open);
					 }
					 doMCICommandMessagePlay(hDlg);
				 }
				checkOpenFile = true;
			}
			return (INT_PTR)TRUE;
			break;
		case IDC_RADIO_NO_LOOP:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO_NO_LOOP) == true)
			{
				is_loop = false;
				loop_music = 0;
			}
			return (INT_PTR)TRUE;
			break;
		case IDC_RADIO_LOOP:
			if (IsDlgButtonChecked(hDlg, IDC_RADIO_LOOP) == true)
			{
				is_loop = true;
				loop_music = 1;
			}
			return (INT_PTR)TRUE;
			break;
		case IDC_INFORMATION:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
			return(INT_PTR)TRUE;
		case IDCANCEL:
			SendMessage(hDlg, WM_CLOSE, 0, 0);
			return (INT_PTR)TRUE;
			break;
		default:
			return (INT_PTR)FALSE;
		}
		break;
	case WM_TIMER:
		if (isWinner == false)
		{
			count_time.second += 1;
			if (count_time.second > 59)
			{
				count_time.minute += 1;
				count_time.second = 0;
				if (count_time.minute > 59)
				{
					count_time.hour += 1;
					count_time.minute = 0;
				}
			}
			if (count_time.second < 10)
			{
				if (count_time.minute < 10)
				{
					if (count_time.hour < 10)
					{
						wsprintf(c_time, L"0%d:0%d:0%d", count_time.hour, count_time.minute, count_time.second);
					}
					else
					{
						wsprintf(c_time, L"%d:0%d:0%d", count_time.hour, count_time.minute, count_time.second);
					}
				}
				else
				{
					if (count_time.hour < 10)
					{
						wsprintf(c_time, L"0%d:0%d:0%d", count_time.hour, count_time.minute, count_time.second);
					}
					else
					{
						wsprintf(c_time, L"%d:%d:0%d", count_time.hour, count_time.minute, count_time.second);
					}
				}

			}
			else
			{
				if (count_time.minute < 10)
				{
					if (count_time.hour < 10)
					{
						wsprintf(c_time, L"0%d:0%d:%d", count_time.hour, count_time.minute, count_time.second);
					}
					else
					{
						wsprintf(c_time, L"%d:0%d:%d", count_time.hour, count_time.minute, count_time.second);
					}
				}
				else
				{
					if (count_time.hour < 10)
					{
						wsprintf(c_time, L"0%d:0%d:%d", count_time.hour, count_time.minute, count_time.second);
					}
					else
					{
						wsprintf(c_time, L"%d:%d:%d", count_time.hour, count_time.minute, count_time.second);
					}
				}
			}

			SendDlgItemMessage(hDlg, IDC_COUNT_TIME, WM_SETTEXT, 0, (LPARAM)c_time);
		}
		return (INT_PTR)TRUE;
		break;
	case WM_PAINT: // use this case as the way to hide dialog as first time run it
		hdc = BeginPaint(hDlg, &ps);
		EndPaint(hDlg, &ps);
		return (INT_PTR)FALSE;
		break;
	case MM_MCINOTIFY:
		switch (wParam)
		{
		case MCI_NOTIFY_SUCCESSFUL:
			if (is_loop == true && is_play_music == true)
			{
				if (mciWaveDevID != NULL)
				{
					mciSendCommand(lParam, MCI_CLOSE, 0, 0L);
					mciWaveDevID = NULL;
				}
				if (mciWaveDevID == NULL)
				{
					doMCICommandMessageDevice(hDlg, path_music_dialog_open);
				}
				if (mciWaveDevID != NULL)
				{
					doMCICommandMessagePlay(hDlg);
				}
			}
			else
			{
					if (mciWaveDevID != NULL)
					{
						mciSendCommand(lParam, MCI_CLOSE, 0, 0L);
						mciWaveDevID = NULL;
					}
			}
			break;
		default:
			break;
		}
	
		return (INT_PTR)TRUE;
		break;
	case WM_NOTIFY:
	{
	
	case IDC_LIST_PLAY:
		switch (((LPNMHDR)lParam)->code)
		{
		case NM_CLICK:
			int iSlected = SendMessage(hListCrtl, LVM_GETNEXTITEM, -1, LVNI_SELECTED);
			if (iSlected != -1)
			{
				if (choose_one != -1 && hasInVector(saveChoose,iSlected)==false && iSlected!=choose_one)
				{
					choose_two = iSlected;
				}
				if (choose_one == -1 && hasInVector(saveChoose, iSlected) == false)
				{
					choose_one = iSlected;
				}
				//Lượt đầu tiên
				if (choose_two == -1 && choose_one != -1)
				{
					ImageList_ReplaceIcon(hLarge, choose_one, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_one] + IDI_ICON1)));
					ImageList_ReplaceIcon(hSmall, choose_one, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_one] + IDI_ICON1)));
					RedrawWindow(GetDlgItem(hDlg, IDC_LIST_PLAY), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
				}
				if (choose_two != -1 && choose_one != -1)
				{
					if (List_ID_Image[choose_one] == List_ID_Image[choose_two])
					{
						ImageList_ReplaceIcon(hLarge, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_two] + IDI_ICON1)));
						ImageList_ReplaceIcon(hSmall, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_two] + IDI_ICON1)));
						RedrawWindow(GetDlgItem(hDlg, IDC_LIST_PLAY), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
						saveChoose.push_back(choose_one);
						saveChoose.push_back(choose_two);
						countScore += 1;
						wsprintf(c_score, L"%d", countScore);
						SendDlgItemMessage(hDlg, IDC_SCORE, WM_SETTEXT, 0, (LPARAM)c_score);
						choose_one = -1;
						choose_two = -1;
						if (countScore == number_image)
						{
							isWinner = true;
							MessageBox(hDlg, L"Bạn đã chiến thắng", L"Thông báo", MB_OK | MB_ICONINFORMATION);
								
						}
					}
					else
					{
						ImageList_ReplaceIcon(hLarge, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_two] + IDI_ICON1)));
						ImageList_ReplaceIcon(hSmall, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(List_ID_Image[choose_two]  + IDI_ICON1)));
						RedrawWindow(GetDlgItem(hDlg, IDC_LIST_PLAY), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);

						Sleep(500);
						//Hiện lại hình pokeball
						ImageList_ReplaceIcon(hLarge, choose_one, LoadIcon(hInst, MAKEINTRESOURCE(IDI_POKEBALL)));
						ImageList_ReplaceIcon(hSmall, choose_one, LoadIcon(hInst, MAKEINTRESOURCE(IDI_POKEBALL)));
						ImageList_ReplaceIcon(hLarge, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(IDI_POKEBALL)));
						ImageList_ReplaceIcon(hSmall, choose_two, LoadIcon(hInst, MAKEINTRESOURCE(IDI_POKEBALL)));
						RedrawWindow(GetDlgItem(hDlg, IDC_LIST_PLAY), NULL, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
						choose_one = -1;
						choose_two = -1;
					}
				}
			}
		}

		return (INT_PTR)TRUE;
	}
		break;
	case WM_CLOSE:
		KillTimer(hDlg, IDC_TIMER);
		//Đóng device MCI đang sử dụng
		if (mciWaveDevID != NULL)
		{
			mciSendCommand(lParam, MCI_CLOSE, 0, 0L);
			mciWaveDevID = NULL;
		}
		//Lưu thông tin cấu hình của game
		writeFileConfigure();
		DestroyWindow(hDlg);
	return (INT_PTR)TRUE; 
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return (INT_PTR)TRUE;

	}
	return (INT_PTR)FALSE;
}

BOOL InitListViewImageLists(HWND hWndListView)
{

	// Create the full-sized icon image lists. 
	hLarge = ImageList_Create(GetSystemMetrics(SM_CXICON),GetSystemMetrics(SM_CYICON),ILC_MASK, 1, 1);

	hSmall = ImageList_Create(GetSystemMetrics(SM_CXSMICON),GetSystemMetrics(SM_CYSMICON),ILC_MASK, 1, 1);

	// Add an icon to each image list.  
	for (int i = 0; i < number_image*2; i++)
	{
		hiconItem = LoadIcon(hInst, MAKEINTRESOURCE(IDI_POKEBALL));
		ImageList_AddIcon(hLarge, hiconItem);
		ImageList_AddIcon(hSmall, hiconItem);
		
		DestroyIcon(hiconItem);
	}
	for (int i = 0; i < number_image; i++)
	{
		List_ID_Image.push_back(i);
		List_ID_Image.push_back(i);
	}
	//Thực hiện random vector sau khi lấy id của các image
	doRandomVector(List_ID_Image);
	// Assign the image lists to the list-view control. 
		ListView_SetImageList(hWndListView, hLarge, LVSIL_NORMAL);
		ListView_SetImageList(hWndListView, hSmall, LVSIL_SMALL);
	return TRUE;
}


// InitListViewColumns: Adds columns to a list-view control.
// hWndListView:        Handle to the list-view control. 
// Returns TRUE if successful, and FALSE otherwise. 
BOOL InitListViewColumns(HWND hWndListView)
{
	//WCHAR szText[256];     // Temporary buffer.
	LVCOLUMN lvc;
	int iCol;
	countCol = 1;
	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_WIDTH  ;
	// Add the columns.
	for (iCol = 0; iCol < countCol; iCol++)
	{
		lvc.iSubItem = iCol;
		//lvc.pszText = szText;
		lvc.cx = 50;               // Width of column in pixels.

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.
		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;
	}
	return TRUE;
}


// InsertListViewItems: Inserts items into a list view. 
// hWndListView:        Handle to the list-view control.
// cItems:              Number of items to insert.
// Returns TRUE if successful, and FALSE otherwise.
BOOL InsertListViewItems(HWND hWndListView, int cItems)
{
	LVITEM lvI; //Cấu trúc mô tả 1 Item của ListView
	// Initialize LVITEM members that are common to all items.
	//lvI.pszText = LPSTR_TEXTCALLBACK; // Sends an LVN_GETDISPINFO message.
	lvI.mask = LVIF_IMAGE | LVIF_STATE;
	lvI.stateMask = 0;

	lvI.state = 0;

	// Initialize LVITEM members that are different for each item.
	for (int index = 0; index < cItems; index++)
	{
		lvI.iSubItem = 0;
		lvI.iItem = index;
		lvI.iImage = index;

		// Insert items into the list.
		if (ListView_InsertItem(hWndListView, &lvI) == -1)
			return FALSE;
	}
	return TRUE;
}

//  ---------------------------------------------------------------------
//    doCreate_ListView Function
//		Ham tao lap 1 List View
//  ---------------------------------------------------------------------
void doCreate_ListView(HWND hDlg,HWND hWndListView)
{
	// Nap thu vien Common Control DLL
	InitCommonControls();
	if (!InitListViewColumns(hWndListView) || !InitListViewImageLists(hWndListView) || !InsertListViewItems(hWndListView,number_image*2))
	{
		MessageBox(hDlg, L"Khong khoi tao duoc Image-list hay khong tao duoc cac item. \nHuy bo ListView.",
			L"Add error", MB_OK);
		DestroyWindow(hWndListView);
		hWndListView = NULL;
		return;
	}
}

bool readFileConfigure()
{
	wifstream reader;
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);
	reader.open("TCLH.INI");
	if (!reader.is_open())
	{
		return false;
	}
	wstring str,temp,data_str;
	reader.imbue(utf8_locale);
	while (!reader.eof())
	{
		getline(reader, str);
		if (str[0] != ';' &&  str == L"[IMAGE]")
		{
			getline(reader, temp);
			for (int i = 7; i < temp.length(); i++)
			{
				data_str.push_back(temp[i]);
			}
			number_image = _ttoi(data_str.c_str());
		}

		if (str[0] != ';' &&  str == L"[MUSIC]")
		{
			getline(reader, temp);
			for (int i = 10; i < temp.length(); i++)
			{
				data_str.push_back(temp[i]);
			}
			play_music = _ttoi(data_str.c_str());
			data_str.clear();
			getline(reader, temp);

			for (int i = 5; i < temp.length(); i++)
			{
				data_str.push_back(temp[i]);
			}
			loop_music = _ttoi(data_str.c_str());
		}

		if (str[0] != ';' &&  str == L"[MUSICPATH]")
		{
			getline(reader, temp);
			for (int i = 5; i < temp.length(); i++)
			{
				path_music.push_back(temp[i]);
			}
			//path_music = data_str;
		}
		data_str.clear();
	};
	reader.close();
	return true;
}

//Hàm xuất file cấu hình
bool writeFileConfigure()
{

	wofstream writer;
	const std::locale empty_locale = std::locale::empty();
	typedef std::codecvt_utf8<wchar_t> converter_type;
	const converter_type* converter = new converter_type;
	const std::locale utf8_locale = std::locale(empty_locale, converter);
	writer.open(L"TCLH.INI");
	if (!writer.is_open())
	{
		MessageBox(NULL, L"Có lỗi trong quá trình ghi file cấu hình", L"Thông báo!", MB_OK);
		return false;
	}
	writer.imbue(utf8_locale);
	writer << ";TCLH Application" << endl;
	writer << ";This is the configuration file stored information of the game, not edit, do not delete or anything related to this file. All the action can be associated with any program error." << endl;
	writer << ";1453017_NguyenVuHuy_14CLC_LapTrinhWindows" << endl;
	writer << ";Copyright 2016 by Nguyen Vu Huy. All right reversed!" << endl;
	writer << endl;
	writer << "[IMAGE]" << endl;
	writer <<"NUMBER="<< number_image << endl;
	writer << "[MUSIC]" << endl;
	writer << "PLAYMUSIC=" << play_music << endl;
	writer << "LOOP=" << loop_music << endl;
	writer << "[MUSICPATH]" << endl;
	writer << "PATH=" << path_music_dialog_open;
	writer.flush();
	writer.close();
	MessageBox(NULL, L"Đã ghi file cấu hình thành công", L"Thông báo!", MB_OK);
	return true;
}

LPWSTR selectMusicFile(HWND hDlg)
{
	OPENFILENAME ofn;
	TCHAR szFile[256];
	TCHAR szFilter[] = TEXT("Wav file\0 *.wav\0");
	szFile[0] = '\0';
	// Khởi tạo struct
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hDlg; // handle của window cha
	ofn.lpstrFilter = szFilter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFile = szFile; // chuỗi tên file trả về
	ofn.nMaxFile = sizeof(szFile);
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	ofn.hInstance = hInst;
	if (GetOpenFileName(&ofn))
	{
		return ofn.lpstrFile;
	}
	else
	{
		MessageBox(hDlg, L"Bạn chưa chọn file nào!", L"Thông báo", MB_OK);
		return L"\0";
	}
}

//Hàm kiểm tra xem file wav có đúng cấu trúc hay không
bool isWaveFile(TCHAR* szFileName)
{
	HMMIO hmmio;

	// Mo file 
	if (!(hmmio = mmioOpen(szFileName, 0, MMIO_READ | MMIO_ALLOCBUF)))
		return FALSE;

	MMCKINFO mmckinfoParent;   // Group Header (special chunk)

	// Dinh vi nhom wave (group header) 
	mmckinfoParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');

	// Neu khong co nhom wave thi day la khong phai thuoc dinh dang wave
	if (mmioDescend(hmmio, (LPMMCKINFO)&mmckinfoParent, 0, MMIO_FINDRIFF))
		return FALSE;

	// Nguoc lai dung -> dung
	return TRUE;
}

//Hàm chạy file wav theo kiểu truyền Message
void doMCICommandMessageDevice(HWND hDlg,TCHAR* szFileName)
{
	// Mo wave/midi
	MCI_OPEN_PARMS	mciOpenParams;
	MCIERROR		dwReturn;

	// mciOpenParams.lpstrDeviceType phai dung voi flag MCI_OPEN_TYPE
	if (isWaveFile(szFileName))
	{
		mciOpenParams.lpstrDeviceType = L"waveaudio";
	}
	else 
	{
		MessageBox(hDlg, L"File nhạc không đúng định dạng hoặc đường dẫn file không hợp lệ!", L"Lỗi", MB_OK|MB_ICONERROR);
		return;
	}
	mciOpenParams.lpstrElementName = szFileName;	// phai dung voi flag MCI_OPEN_ELEMENT

		if (dwReturn = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT,
			(DWORD)(LPVOID)&mciOpenParams))
		{
			wchar_t szError[128];

			mciGetErrorString(dwReturn, szError, 128);

			MessageBox(hDlg, szError, L"Error", MB_OK);
			return;
		}
		// Lấy device ID
		mciWaveDevID = mciOpenParams.wDeviceID;
}


void doMCICommandMessagePlay(HWND hDlg)
{
	MCI_OPEN_PARMS	mciOpenParams;
	MCIERROR		dwReturn;
	// Play file
	MCI_PLAY_PARMS mciPlayParams;
	mciPlayParams.dwCallback = (unsigned long)hDlg;			// cua so nhan message MM_MCINOTIFY

	if (dwReturn = mciSendCommand(mciWaveDevID, MCI_PLAY, MCI_NOTIFY, (DWORD)(LPVOID)&mciPlayParams))
	{
		wchar_t szError[128];

		mciSendCommand(mciWaveDevID, MCI_CLOSE, 0, 0L);

		mciGetErrorString(dwReturn, szError, 128);

		//MessageBox(hDlg, szError, L"Error", MB_OK);
		return;
	}
}


//Hàm dừng file nhạc theo kiểu truyền message
void doMCICommandMessageStop(HWND hFrameWnd)
{
	if (mciWaveDevID)
	{
		mciSendCommand(mciWaveDevID, MCI_STOP, 0, 0);
	}
}

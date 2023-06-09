/*
   Demomode
*/

#include "stdafx.h"
#include "resource.h"
#include <Windows.h>
#include <windowsx.h>
#include <iostream>
#include <Commctrl.h>
#include <vector>
#include <set>
#include <map>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>
#include <fstream>
#include <string>
#include <Shlobj.h>
#include <time.h>
#include <queue>


/*
   Опредление глобальных переменных

   BurundukPro - это универсальный эмулятор/автоматизатор клавиатуры и мышки. 
   
   Программа имеет два режима работы - это пользовательский режим и режим редактор сценариев. 
   
   Первый режим нужен для записи простых действий - перемещение мыши, клики и запись клавиатуры, не требует погружения в детали.

*/

#include "global.h"

#include "list.h"

// -----------------------------------------------------
// Функция InsTreeItem
// -----------------------------------------------------

HTREEITEM InsTreeItem(HTREEITEM hParent, LPWSTR szText,
	HTREEITEM hAfter, int iImage, int iSelectedImage)
{
	TV_INSERTSTRUCT tvins;

	HTREEITEM hItem;

	memset(&tvins, 0, sizeof(tvins));

	tvins.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;

	tvins.item.pszText = szText;

	tvins.item.cchTextMax = lstrlen(szText);

	tvins.item.iImage = iImage;

	tvins.item.iSelectedImage = iSelectedImage;

	tvins.hInsertAfter = hAfter;

	tvins.hParent = hParent;

	hItem = TreeView_InsertItem(hwndTree, &tvins);

	return hItem;
}

void Indicate_Release(HWND hWnd) {

	nCountIndicateRevers = 0;

	nCountIndicate = 0;

	while (!qBottomIndicate.empty()) {

		qBottomIndicate.pop();
	}

	for (int j = 1; j < 244 * scaleX; j += 4) {

		qBottomIndicate.push(left_panel_toolsBG_X + (5) * j);
	}	

	qBottomIndicateTemp = qBottomIndicate;
	
	SendMessage(hWnd, WM_COMMAND, DELAY_TOP_MSG_UPDATE_INDICATION, NULL);
}

void Indicate_Update(HWND hWnd) {

	RECT r;

	GetClientRect(hWnd, &r);

	r.left = left_panel_toolsBG_X + 5;

	r.top = r.bottom - bottom_panel_indicateY;

	r.bottom = r.bottom - 5;

	r.right = r.right - 5;

	nCountIndicateRevers++;

	nCountIndicate += 5;

	InvalidateRect(hWnd, &r, false);

	UpdateWindow(hWnd);
}

bool paintRectLeftPanelTools(HDC hdc, RECT dim, COLORREF penCol, COLORREF brushCol, unsigned int opacity)
{

	HDC tempHdc = CreateCompatibleDC(hdc);

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	HBITMAP hbitmap;       // bitmap handle 

	BITMAPINFO bmi;        // bitmap header 
	// zero the memory for the bitmap info 
	ZeroMemory(&bmi, sizeof(BITMAPINFO));

	// setup bitmap info  
	// set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 

	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	bmi.bmiHeader.biWidth = dim.right - dim.left;

	bmi.bmiHeader.biHeight = dim.bottom - dim.top;

	bmi.bmiHeader.biPlanes = 1;

	bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 

	bmi.bmiHeader.biCompression = BI_RGB;

	bmi.bmiHeader.biSizeImage = (dim.right - dim.left) * (dim.bottom - dim.top) * 4;

	// create our DIB section and select the bitmap into the dc 
	hbitmap = CreateDIBSection(tempHdc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);

	SelectObject(tempHdc, hbitmap);

	SetDCPenColor(tempHdc, RGB(80, 80, 80));

	SetDCBrushColor(tempHdc, RGB(80, 80, 80));

	HBRUSH hBrsh = CreateSolidBrush(RGB(80, 80, 80));

	RECT tm;

	tm.left = dim.left;

	tm.right = dim.right;

	tm.top = 0;//dim.top;

	tm.bottom = dim.bottom;

	FillRect(tempHdc, &tm, hBrsh);

	bool res = false;

	if (AlphaBlend(hdc, dim.left, dim.top, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, tempHdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, blend)) {

		res = true;
	}
	else
	{
		res = false;
	}

	DeleteDC(tempHdc);

	DeleteObject(hBrsh);

	DeleteObject(hbitmap);

	return res;
}

bool paintRect(HDC hdc, RECT dim, COLORREF penCol, COLORREF brushCol, unsigned int opacity)
{
		
	HDC tempHdc = CreateCompatibleDC(hdc);

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	HBITMAP hbitmap;       // bitmap handle 

	BITMAPINFO bmi;        // bitmap header 
	// zero the memory for the bitmap info 
	ZeroMemory(&bmi, sizeof(BITMAPINFO));

	// setup bitmap info  
	// set the bitmap width and height to 60% of the width and height of each of the three horizontal areas. Later on, the blending will occur in the center of each of the three areas. 
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = dim.right - dim.left;
	bmi.bmiHeader.biHeight = dim.bottom - dim.top;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;         // four 8-bit components 
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = (dim.right - dim.left) * (dim.bottom - dim.top) * 4;

	// create our DIB section and select the bitmap into the dc 
	hbitmap = CreateDIBSection(tempHdc, &bmi, DIB_RGB_COLORS, NULL, NULL, 0x0);

	SelectObject(tempHdc, hbitmap);

	SetDCPenColor(tempHdc, RGB(0, 0, 255));

	SetDCBrushColor(tempHdc, RGB(0, 0, 255));

	HBRUSH hBrsh = CreateSolidBrush(RGB(0, 0, 255));

	RECT tm;

	tm.left = 0;

	tm.right = 40;

	tm.top = 0;

	tm.bottom = bmi.bmiHeader.biHeight;

	FillRect(tempHdc, &tm, hBrsh);

	bool res = false;

	if (AlphaBlend(hdc, dim.left, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, tempHdc, 0, 0, bmi.bmiHeader.biWidth, bmi.bmiHeader.biHeight, blend)) {

		res = true;
	}
	else
	{
		res = false;
	}

	DeleteDC(tempHdc);

	DeleteObject(hBrsh);

	DeleteObject(hbitmap);

	return res;
}

//--------------------------------

static HBRUSH CreateGradientBrush(COLORREF top, COLORREF bottom, LPNMCUSTOMDRAW item)
{
	HBRUSH Brush = NULL;

	HDC hdcmem = CreateCompatibleDC(item->hdc);

	HBITMAP hbitmap = CreateCompatibleBitmap(item->hdc, item->rc.right - item->rc.left, item->rc.bottom - item->rc.top);

	SelectObject(hdcmem, hbitmap);

	int r1 = GetRValue(top), r2 = GetRValue(bottom), g1 = GetGValue(top), g2 = GetGValue(bottom), b1 = GetBValue(top), b2 = GetBValue(bottom);

	for (int i = 0; i < item->rc.bottom - item->rc.top; i++)
	{
		RECT temp;

		int r, g, b;

		r = int(r1 + double(i * (r2 - r1) / item->rc.bottom - item->rc.top));

		g = int(g1 + double(i * (g2 - g1) / item->rc.bottom - item->rc.top));

		b = int(b1 + double(i * (b2 - b1) / item->rc.bottom - item->rc.top));

		Brush = CreateSolidBrush(RGB(r, g, b));

		temp.left = 0;

		temp.top = i;

		temp.right = item->rc.right - item->rc.left;

		temp.bottom = i + 1;

		FillRect(hdcmem, &temp, Brush);

		DeleteObject(Brush);
	}

	HBRUSH pattern = CreatePatternBrush(hbitmap);

	DeleteDC(hdcmem);

	DeleteObject(Brush);

	DeleteObject(hbitmap);

	return pattern;
}

std::wstring get_patch_for_save_scripts(HWND hWnd) {

	
		return L"Default desktop";	
}

void activate_add_func(HWND hWnd, bool a) {

	EnableWindow(GetDlgItem(hWnd, IDC_CHECK2), a);

	EnableWindow(GetDlgItem(hWnd, IDC_CHECK5), a);

	EnableWindow(GetDlgItem(hWnd, IDC_CHECK11), a);

	//EnableWindow(GetDlgItem(hWnd, IDCANCEL6), !a);

	EnableWindow(GetDlgItem(hWnd, IDCANCEL7), !a);
}

BOOL DrawLine(HDC hdc, int x1, int y1, int x2, int y2)
{
	POINT pt;

	MoveToEx(hdc, x1, y1, &pt);

	return LineTo(hdc, x2, y2);
}

// Генерируем рандомное число между значениями min и max.
// Предполагается, что функцию srand() уже вызывали
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

std::wstring ConvertStringToWstring(const std::string& str)
{
	if (str.empty())
	{
		return std::wstring();
	}
	
	int num_chars = MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), NULL, 0);
	
	std::wstring wstrTo;
	
	if (num_chars)
	{
		wstrTo.resize(num_chars);
		if (MultiByteToWideChar(CP_ACP, MB_ERR_INVALID_CHARS, str.c_str(), str.length(), &wstrTo[0], num_chars))
		{
			return wstrTo;
		}
	}
	
	return std::wstring();
}

BOOL AddItems_Key(HWND hwndCB) {

	COMBOBOXEXITEMW cbei = { 0 };

	int iCnt;

	typedef struct {

		const char* pszText;

	} ITEMINFO, * PITEMINFO;

	ITEMINFO IInf[] = {

	" vKey::KEY_0",
	" vKey::KEY_1",
	" vKey::KEY_2",
	" vKey::KEY_3",
	" vKey::KEY_4",
	" vKey::KEY_5",
	" vKey::KEY_6",
	" vKey::KEY_7",
	" vKey::KEY_8",
	" vKey::KEY_9",
	" vKey::KEY_A",
	" vKey::KEY_B",
	" vKey::KEY_C",
	" vKey::KEY_D",
	" vKey::KEY_E",
	" vKey::KEY_F",
	" vKey::KEY_G",
	" vKey::KEY_H",
	" vKey::KEY_I",
	" vKey::KEY_J",
	" vKey::KEY_K",
	" vKey::KEY_L",
	" vKey::KEY_M",
	" vKey::KEY_N",
	" vKey::KEY_O",
	" vKey::KEY_P",
	" vKey::KEY_Q",
	" vKey::KEY_R",
	" vKey::KEY_S",
	" vKey::KEY_T",
	" vKey::KEY_U",
	" vKey::KEY_V",
	" vKey::KEY_W",
	" vKey::KEY_X",
	" vKey::KEY_Y",
	" vKey::KEY_Z",
	" vKey::KEY_F1",
	" vKey::KEY_F2",
	" vKey::KEY_F3",
	" vKey::KEY_F4",
	" vKey::KEY_F5",
	" vKey::KEY_F6",
	" vKey::KEY_F7",
	" vKey::KEY_F8",
	" vKey::KEY_F9",
	" vKey::KEY_F10",
	" vKey::KEY_F11",
	" vKey::KEY_F12",
	" vKey::KEY_ESC"

	};

	SendMessage(hwndCB, CB_RESETCONTENT, 0, 0);

	cbei.mask = CBEIF_TEXT;

	for (iCnt = 0; iCnt < 49; iCnt++) {

		cbei.iItem = iCnt;

		std::wstring tmp = ConvertStringToWstring(IInf[iCnt].pszText);

		cbei.pszText = (LPWSTR)tmp.c_str();

		cbei.cchTextMax = strlen(IInf[iCnt].pszText);

		if (SendMessage(hwndCB, CBEM_INSERTITEM, 0, (LPARAM)&cbei) == -1)
			return FALSE;
	}


	return TRUE;

}


BOOL AddItems_Key_Bind(HWND hwndCB) {

	COMBOBOXEXITEMW cbei = { 0 };

	int iCnt;

	typedef struct {

		const char* pszText;

	} ITEMINFO, * PITEMINFO;

	ITEMINFO IInf[] = {

	" vKey::KEY_0",
	" vKey::KEY_1",
	" vKey::KEY_2",
	" vKey::KEY_3",
	" vKey::KEY_4",
	" vKey::KEY_5",
	" vKey::KEY_6",
	" vKey::KEY_7",
	" vKey::KEY_8",
	" vKey::KEY_9",
	" vKey::KEY_A",
	" vKey::KEY_B",
	" vKey::KEY_C",
	" vKey::KEY_D",
	" vKey::KEY_E",
	" vKey::KEY_F",
	" vKey::KEY_G",
	" vKey::KEY_H",
	" vKey::KEY_I",
	" vKey::KEY_J",
	" vKey::KEY_K",
	" vKey::KEY_L",
	" vKey::KEY_M",
	" vKey::KEY_N",
	" vKey::KEY_O",
	" vKey::KEY_P",
	" vKey::KEY_Q",
	" vKey::KEY_R",
	" vKey::KEY_S",
	" vKey::KEY_T",
	" vKey::KEY_U",
	" vKey::KEY_V",
	" vKey::KEY_W",
	" vKey::KEY_X",
	" vKey::KEY_Y",
	" vKey::KEY_Z",
	" vKey::KEY_F1",
	" vKey::KEY_F2",
	" vKey::KEY_F3",
	" vKey::KEY_F4",
	" vKey::KEY_F5",
	" vKey::KEY_F6",
	" vKey::KEY_F7",
	" vKey::KEY_F8",
	" vKey::KEY_F9",
	" vKey::KEY_F10",
	" vKey::KEY_F11",
	" vKey::KEY_F12",
	" vKey::KEY_ESC"

	};

	SendMessage(hwndCB, CB_RESETCONTENT, 0, 0);

	cbei.mask = CBEIF_TEXT;

	for (iCnt = 0; iCnt < 49; iCnt++) {

		cbei.iItem = iCnt;

		std::wstring tmp = ConvertStringToWstring(IInf[iCnt].pszText);

		cbei.pszText = (LPWSTR)tmp.c_str();

		cbei.cchTextMax = strlen(IInf[iCnt].pszText);

		if (SendMessage(hwndCB, CBEM_INSERTITEM, 0, (LPARAM)&cbei) == -1)
			return FALSE;
	}


	return TRUE;

}

BOOL  AddItems_Random(HWND hwndCB)
{
	COMBOBOXEXITEMW cbei = { 0 };

	int iCnt;

	typedef struct {

		const char* pszText;

	} ITEMINFO, * PITEMINFO;

	ITEMINFO IInf[] = {

		 " Малый",
		 " Средний",
		 " Высокий"
	};

	SendMessage(hwndCB, CB_RESETCONTENT, 0, 0);

	cbei.mask = CBEIF_TEXT;

	for (iCnt = 0; iCnt < 3; iCnt++) {

		cbei.iItem = iCnt;

		std::wstring tmp = ConvertStringToWstring(IInf[iCnt].pszText);

		cbei.pszText = (LPWSTR)tmp.c_str();

		cbei.cchTextMax = strlen(IInf[iCnt].pszText);

		if (SendMessage(hwndCB, CBEM_INSERTITEM, 0, (LPARAM)&cbei) == -1)
			return FALSE;
	}


	return TRUE;
}

BOOL  AddItems_VariosTime(HWND hwndCB)
{
	COMBOBOXEXITEMW cbei = { 0 };

	int iCnt;

	typedef struct {

		const char* pszText;

	} ITEMINFO, * PITEMINFO;

	ITEMINFO IInf[] = {

		 " Секунда",
		 " Микросекунда"		 
	};

	SendMessage(hwndCB, CB_RESETCONTENT, 0, 0);

	cbei.mask = CBEIF_TEXT;

	for (iCnt = 0; iCnt < 2; iCnt++) {

		cbei.iItem = iCnt;

		std::wstring tmp = ConvertStringToWstring(IInf[iCnt].pszText);

		cbei.pszText = (LPWSTR)tmp.c_str();

		cbei.cchTextMax = strlen(IInf[iCnt].pszText);

		if (SendMessage(hwndCB, CBEM_INSERTITEM, 0, (LPARAM)&cbei) == -1)
			return FALSE;
	}


	return TRUE;
}

BOOL  AddItems(HWND hwndCB)
{
	COMBOBOXEXITEMW cbei = {0};

	int iCnt;

	typedef struct {

		const char* pszText;

	} ITEMINFO, * PITEMINFO;

	ITEMINFO IInf[] = {

		 " Эмуляция - [Low]",
		 " Эмуляция - [Medium]",
		 " Эмуляция - [High]"		 
	};

	SendMessage(hwndCB, CB_RESETCONTENT, 0, 0);

	cbei.mask = CBEIF_TEXT;

	for (iCnt = 0; iCnt < 3; iCnt++) {

		cbei.iItem = iCnt;

		std::wstring tmp = ConvertStringToWstring(IInf[iCnt].pszText);

		cbei.pszText = (LPWSTR)tmp.c_str();

		cbei.cchTextMax = strlen(IInf[iCnt].pszText);

		if (SendMessage(hwndCB, CBEM_INSERTITEM, 0, (LPARAM)&cbei) == -1)
			return FALSE;
	}


	return TRUE;
}

bool operator<(const main& a, const main& b)
{
	return (a.x < b.x) && (a.y < b.y);
}

bool save(const vector<main>& s) {

	

	return true;
}

int press(UINT code)
{

	return 0;
}

void core_time(int x, int y, bool lB, bool rB, bool press_key, UINT code_key, int sp) {

	
}

int ShutdownWindows()
{
	

	return 0;
}

int playClick(HWND hWDlg, bool left, bool right, bool infinity, bool shutdown, long sleep, int x, int y, long countItter, bool randomFlg) {

	
	return 0;
}

void play(HWND hWDlg, int speedControl, bool infinity, UINT code_iter) {

	
}

void write(bool aLft, bool aC, bool bC, bool cC, bool key_brd, int speedControl, HWND hWDlg) {


}

int get_file_size(std::wstring filename) // path to file
{
	FILE* p_file = NULL;

	p_file = _wfopen(filename.c_str(), L"rb");

	if (p_file == nullptr) 
	{
		return 0;
	}

	fseek(p_file, 0, SEEK_END);

	int size = ftell(p_file);

	fclose(p_file);

	return size;
}


string readofparametrsinreg() {

	return "NULL";
}

static bool SetUserSetting(const wstring &str) {




	return TRUE;
}

wstring readinifail() {

	return L"FAIL";
}

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
	
	time_t     now = time(0);
	
	struct tm  tstruct;
	
	char       buf[80];
	
	tstruct = *localtime(&now);	
	
	strftime(buf, sizeof(buf), "%X", &tstruct);

	return buf;
}

static HWND getWindowActive(LPARAM lParam) {

	

	return NULL;
}

DWORD WINAPI ThreadProcPlan(CONST LPVOID lpParam) {//функция

	return 0;
}

DWORD WINAPI ThreadProcWelcome(CONST LPVOID lpParam) {//функция


	while (true) {

		Sleep(sleep_time);

		if (visible_period) {

			visible_period = false;

			if (true)
			{
				string ret = readofparametrsinreg();

				if ("FAIL" == ret) {

					MessageBox(NULL, L"A critical error has occurred", L"Attention!", NULL);

					exit(0);
				}
				else
				{

					{

						ext = true;

						SetWindowText(hwndFrame, L"Burunduk Pro v.2.4 - Не зарегистрировано m.0.1.2");

						activate_add_func(hWndDlgTmp, false);

						ShowWindow(hWndInfo, SW_SHOW);

					}
				}
			}

			ShowWindow(hWnd_net, SW_HIDE);

			int x = GetSystemMetrics(SM_CXSCREEN);

			int y = GetSystemMetrics(SM_CYSCREEN);

			SetWindowPos(hWndInfo, HWND_TOPMOST, (x / 2) - ((bmpW * scaleX) / 2), (y / 2) - ((bmpH * scaleY) / 2), (bmpW * scaleX), (bmpH * scaleY), SWP_SHOWWINDOW);

			InvalidateRect(hWndInfo, NULL, false);

			UpdateWindow(hWndInfo);
		}
	}
}

DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
	
	return 0;
}

DWORD WINAPI ThreadProcGetMessage(CONST LPVOID lpParam) {
	
	while (true)
	{
		Sleep(1000);

		SendMessage(hwndFrame, WM_COMMAND, DELAY_TOP_MSG_UPDATE_INDICATION, NULL);
	}	

	return 0;
}

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{			
	
	scaleX = GetDeviceCaps(GetDC(NULL), LOGPIXELSX) / 96.0;

	scaleY = GetDeviceCaps(GetDC(NULL), LOGPIXELSY) / 96.0;

	hbrushNew = CreateSolidBrush(RGB(25, 25, 25));

	hbrushNewBlue = CreateSolidBrush(RGB(20, 90, 250));

	hbrushNewBlueEx = CreateSolidBrush(RGB(61, 109, 100));

	if (true)
	{
		on = true;		
	}	

	if (true)
	{			
		onEdit = true;
	}	

	bool RemoveTraces = false;

	if (true)
	{				
		RemoveTraces = true;		
	}
	
	hPatchGlobalForScripts = L"";

	hFinger = LoadCursor(nullptr, IDC_HAND);
	
	hbrushForDialog = CreateSolidBrush(FON_BG);

	hbrushForRect = CreateSolidBrush(FON_BG_RECT);

	hBrushRed = CreateSolidBrush(FON_BG_INFO);

	hBrushFramrDlg = CreateSolidBrush(RGB(40,40,40));

	hBitmap[0] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[1] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP2), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[2] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP3), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[3] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[4] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[5] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP6), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[6] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP7), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[7] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP8), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[8] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP9), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[9] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[10] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP11), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[11] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP12), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[12] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP13), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[13] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP14), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[14] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP15), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[15] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP16), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[16] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP17), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[17] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP18), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[18] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP19), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[19] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP20), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[20] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP21), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[21] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP22), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[22] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP23), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[23] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP24), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[24] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP28), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[25] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP27), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[26] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP26), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[27] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP25), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[28] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP29), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[29] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP30), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[30] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP35), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[31] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP36), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[32] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP37), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	hBitmap[33] = (HBITMAP)LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP38), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);

	BITMAP  bm;

	GetObject(hBitmap[3], sizeof(BITMAP), (LPSTR)&bm);

	this_tools = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[3], bm.bmHeight * bm.bmWidth * sizeof(DWORD), this_tools);

	bmpW = bm.bmWidth;

	bmpH = bm.bmHeight;

	int is = 0;	

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_tools[j], this_tools[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_tools[j + is], this_tools[k + is]);
		}

		is += bm.bmWidth;
	}	
	
	//***********************************************************************************************

	GetObject(hBitmap[13], sizeof(BITMAP), (LPSTR)&bm);

	this_toolsTips = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[13], bm.bmHeight* bm.bmWidth * sizeof(DWORD), this_toolsTips);

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_toolsTips[j], this_toolsTips[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_toolsTips[j + is], this_toolsTips[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[14], sizeof(BITMAP), (LPSTR)&bm);

	this_toolsTips_0 = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[14], bm.bmHeight * bm.bmWidth * sizeof(DWORD), this_toolsTips_0);

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_toolsTips_0[j], this_toolsTips_0[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_toolsTips_0[j + is], this_toolsTips_0[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[16], sizeof(BITMAP), (LPSTR)&bm);

	this_toolsTipsLiteVers = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[16], bm.bmHeight * bm.bmWidth * sizeof(DWORD), this_toolsTipsLiteVers);

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_toolsTipsLiteVers[j], this_toolsTipsLiteVers[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_toolsTipsLiteVers[j + is], this_toolsTipsLiteVers[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[17], sizeof(BITMAP), (LPSTR)&bm);

	this_toolsTipsWelcome = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[17], bm.bmHeight * bm.bmWidth * sizeof(DWORD), this_toolsTipsWelcome);

	bmpWelcome = bm.bmWidth;

	bmpHbmpWelcome = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_toolsTipsWelcome[j], this_toolsTipsWelcome[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_toolsTipsWelcome[j + is], this_toolsTipsWelcome[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[18], sizeof(BITMAP), (LPSTR)&bm);

	this_Header = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[18], bm.bmHeight* bm.bmWidth * sizeof(DWORD), this_Header);

	headerW = bm.bmWidth;

	headerH = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_Header[j], this_Header[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_Header[j + is], this_Header[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[19], sizeof(BITMAP), (LPSTR)&bm);

	this_HeaderButtons = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[19], bm.bmHeight * bm.bmWidth * sizeof(DWORD), this_HeaderButtons);

	headerButtonsW = bm.bmWidth;

	headerButtonsH = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_HeaderButtons[j], this_HeaderButtons[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_HeaderButtons[j + is], this_HeaderButtons[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[20], sizeof(BITMAP), (LPSTR)&bm);

	this_HeaderButtonsTemps = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[20], bm.bmHeight* bm.bmWidth * sizeof(DWORD), this_HeaderButtonsTemps);

	headerButtonsLeftCaptureW = bm.bmWidth;

	headerButtonsLeftCaptureH = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(this_HeaderButtonsTemps[j], this_HeaderButtonsTemps[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(this_HeaderButtonsTemps[j + is], this_HeaderButtonsTemps[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[21], sizeof(BITMAP), (LPSTR)&bm);

	networks_config = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[21], bm.bmHeight* bm.bmWidth * sizeof(DWORD), networks_config);

	netw_config_w = bm.bmWidth;

	netw_config_h = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(networks_config[j], networks_config[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(networks_config[j + is], networks_config[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[22], sizeof(BITMAP), (LPSTR)&bm);

	pointLPT = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[22], bm.bmHeight* bm.bmWidth * sizeof(DWORD), pointLPT);

	left_panel_tools_X = bm.bmWidth;

	left_panel_tools_Y = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointLPT[j], pointLPT[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointLPT[j + is], pointLPT[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[23], sizeof(BITMAP), (LPSTR)&bm);

	pointLPTBG = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[23], bm.bmHeight* bm.bmWidth * sizeof(DWORD), pointLPTBG);

	left_panel_toolsBG_X = bm.bmWidth;

	left_panel_toolsBG_Y = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointLPTBG[j], pointLPTBG[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointLPTBG[j + is], pointLPTBG[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[28], sizeof(BITMAP), (LPSTR)&bm);

	pointBottomIndicate = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[28], bm.bmHeight* bm.bmWidth * sizeof(DWORD), pointBottomIndicate);

	bottom_panel_indicateX = bm.bmWidth;

	bottom_panel_indicateY = bm.bmHeight;

	for (int j = 1; j < 244 * scaleX; j += 4) {

		qBottomIndicate.push(left_panel_toolsBG_X + 5 * j);
	}

	queueStart = true;

	qBottomIndicateTemp = qBottomIndicate;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointBottomIndicate[j], pointBottomIndicate[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointBottomIndicate[j + is], pointBottomIndicate[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[29], sizeof(BITMAP), (LPSTR)&bm);

	pointImageButton = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[29], bm.bmHeight * bm.bmWidth * sizeof(DWORD), pointImageButton);

	pointImageButton_X = bm.bmWidth;

	pointImageButton_Y = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointImageButton[j], pointImageButton[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointImageButton[j + is], pointImageButton[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[30], sizeof(BITMAP), (LPSTR)&bm);

	cloud_buffer = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[30], bm.bmHeight * bm.bmWidth * sizeof(DWORD), cloud_buffer);

	cloud_buffer_w = bm.bmWidth;

	cloud_buffer_h = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(cloud_buffer[j], cloud_buffer[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(cloud_buffer[j + is], cloud_buffer[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[31], sizeof(BITMAP), (LPSTR)&bm);

	carbon_buffer = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[31], bm.bmHeight * bm.bmWidth * sizeof(DWORD), carbon_buffer);

	carbon_buffer_w = bm.bmWidth;

	carbon_buffer_h = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(carbon_buffer[j], carbon_buffer[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(carbon_buffer[j + is], carbon_buffer[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************	

	GetObject(hBitmap[32], sizeof(BITMAP), (LPSTR)&bm);

	pointSettingBuffer = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[32], bm.bmHeight* bm.bmWidth * sizeof(DWORD), pointSettingBuffer);

	setting_buffer_w = bm.bmWidth;

	setting_buffer_h = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointSettingBuffer[j], pointSettingBuffer[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointSettingBuffer[j + is], pointSettingBuffer[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************

	GetObject(hBitmap[33], sizeof(BITMAP), (LPSTR)&bm);

	pointheadernewinterfeace = new DWORD[bm.bmHeight * bm.bmWidth * sizeof(DWORD)];

	GetBitmapBits(hBitmap[33], bm.bmHeight* bm.bmWidth * sizeof(DWORD), pointheadernewinterfeace);

	headernewinterfeace_w = bm.bmWidth;

	headernewinterfeace_h = bm.bmHeight;

	is = 0;

	/* - - - - - - - - - - - -  - - - -  - - - - - - - - - - - - - - - - - - - - */

	for (size_t j = 0, k = bm.bmHeight * bm.bmWidth - 1; j < k; j++, k--)
	{
		std::swap(pointheadernewinterfeace[j], pointheadernewinterfeace[k]);
	}

	for (size_t s = 0; s < bm.bmHeight; s++)
	{
		for (size_t j = 0, k = bm.bmWidth - 1; j < k; j++, k--)
		{
			std::swap(pointheadernewinterfeace[j + is], pointheadernewinterfeace[k + is]);
		}

		is += bm.bmWidth;
	}

	//***************************************************************************************
	
	memset(&wcex, 0, sizeof(WNDCLASSEX));

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = 0;

	wcex.lpfnWndProc = WndProc;

	wcex.cbClsExtra = 0;

	wcex.cbWndExtra = 0;

	wcex.hInstance = hInstance;

	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wcex.lpszClassName = L"new_inter_f";

	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	RegisterClassExW(&wcex);

	HWND hWnd = CreateWindowEx(WS_EX_TOPMOST, wcex.lpszClassName, L"Burunduk Pro", WS_POPUP,/* | WS_BORDER*/	0, 0, 0, 0, nullptr, nullptr, wcex.hInstance, nullptr);
	
	ShowWindow(hWnd, SW_HIDE);	

	if (true) {

		
		HWND hWelcomeDialog = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG12),
			NULL,
			(DLGPROC)GoToProcToolWelcome);

		int x = GetSystemMetrics(SM_CXSCREEN);

		int y = GetSystemMetrics(SM_CYSCREEN);

		SetWindowPos(hWelcomeDialog, HWND_TOPMOST, (x / 2) - (bmpWelcome / 2), (y / 2) - (bmpHbmpWelcome / 2), bmpWelcome, bmpHbmpWelcome, SWP_SHOWWINDOW);

		InvalidateRect(hWelcomeDialog, NULL, false);

		UpdateWindow(hWelcomeDialog);

		Sleep(3000);

		ShowWindow(hWelcomeDialog, SW_HIDE);

		Sleep(1000);

		hwndFrame = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG13),
			NULL,
			(DLGPROC)GoToProcFrame);
		
		HWND hWnd = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG1),
			hwndFrame,
			(DLGPROC)GoToProc);			

		hWndDlgTmp = hWnd;		

		if (on)
		{
			EnableWindow(GetDlgItem(hWnd, IDOK6), true);
		}

		if (onEdit)
		{
			EnableWindow(GetDlgItem(hWnd, IDOK5), true);

			EnableWindow(GetDlgItem(hWnd, IDOK8), true);
		}

		if (RemoveTraces) {

			EnableWindow(GetDlgItem(hWnd, IDOK9), true);
		}

		hWndReg = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG2),
			NULL,
			(DLGPROC)GoToProcReg); //hWndInfo

		hWndInfo = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG3),
			NULL,
			(DLGPROC)GoToProcInfo);//лицензия		

		hWndInfoLisen = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG14),
			NULL,
			(DLGPROC)GoToProcLisen);

		hWndInfoSetting = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG14),
			NULL,
			(DLGPROC)GoToProcSetting);

		hwndTip = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG9),
			NULL,
			(DLGPROC)GoToProcToolTips);

		hwndTip_0 = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG10),
			NULL,
			(DLGPROC)GoToProcToolTips);

		hwndTip_1 = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG11),
			NULL,
			(DLGPROC)GoToProcToolTips);	

		hwndVersion = CreateDialog(
			hPrevInstance,
			MAKEINTRESOURCE(IDD_DIALOG14),
			NULL,
			(DLGPROC)GoToProcVersion);

		//*******************************************************************************************

		// Создаем главное окно приложения
		hWnd_net = CreateWindowEx(WS_EX_TOPMOST, wcex.lpszClassName, L"Сетевая конфигурация", WS_OVERLAPPEDWINDOW & ~(WS_MAXIMIZEBOX | WS_MINIMIZEBOX),/* | WS_BORDER*/	100, 100, 400, 700, nullptr, nullptr, wcex.hInstance, nullptr);

		if (!hWnd_net) return(FALSE);

		//SetWindowLong(hWndInfo, GWL_STYLE, GetWindowLong(hWndInfo, GWL_STYLE) & CS_NOCLOSE);

		//SetClassLongPtr(hWndInfo, GCL_STYLE, GetClassLongPtr(hWndInfo, GCL_STYLE) | CS_NOCLOSE);

		ShowWindow(hwndVersion, SW_HIDE);
		
		ShowWindow(hWndDlgTmp, SW_HIDE);
		
		ShowWindow(hwndFrame, SW_HIDE);
		
		ShowWindow(hwndTip, SW_HIDE);

		ShowWindow(hwndTip_0, SW_HIDE);

		ShowWindow(hwndPlanirovshik, SW_HIDE);

		ShowWindow(hWndInfo, SW_HIDE);

		ShowWindow(hWndInfoManual, SW_HIDE);

		ShowWindow(hWndInfoLisen, SW_HIDE);

		ShowWindow(hWndInfoSetting, SW_HIDE);

		ShowWindow(hWndDebug, SW_HIDE);

		SendDlgItemMessage(hWnd, IDC_STATIC335, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[2]);

		SendDlgItemMessage(hWnd, IDC_STATIC336, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[0]);

		SendDlgItemMessage(hWnd, IDC_STATIC337, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[1]);

		SendDlgItemMessage(hWnd, IDC_STATIC338, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[8]);

		SendDlgItemMessage(hWnd, IDC_STATIC339, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[4]);

		SendDlgItemMessage(hWnd, IDC_STATIC340, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[5]);

		SendDlgItemMessage(hWnd, IDC_STATIC341, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[6]);

		SendDlgItemMessage(hWnd, IDC_STATIC342, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[7]);

		SendDlgItemMessage(hWnd, IDC_STATIC343, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[9]);//

		SendDlgItemMessage(hWnd, IDC_STATIC344, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[11]);//

		SendDlgItemMessage(hWnd, IDC_STATIC345, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[10]);//

		SendDlgItemMessage(hWnd, IDC_STATIC400, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[12]);//

		SendDlgItemMessage(hwndTip_0, IDC_STATIC401, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[13]);//

		SendDlgItemMessage(hwndTip, IDC_STATIC402, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[14]);//

		SendDlgItemMessage(hWnd, IDC_STATIC403, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[15]);//	

		SendDlgItemMessage(hWnd, IDC_STATIC404, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[24]);//

		SendDlgItemMessage(hWnd, IDC_STATIC405, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[25]);//

		SendDlgItemMessage(hWnd, IDC_STATIC406, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[26]);//

		SendDlgItemMessage(hWnd, IDC_STATIC407, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap[27]);//

		hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));		

		Sleep(2000);

		RECT mainWin;

		GetWindowRect(hWnd, &mainWin);		

		SetWindowPos(hwndFrame, HWND_BOTTOM, mainWin.left - MARGIN_LEFT, mainWin.top - MARGIN_TOP, (mainWin.right - mainWin.left) + MARGIN_RIGHT, (mainWin.bottom - mainWin.top) + MARGIN_BOTTOM,SWP_SHOWWINDOW);
		
		// Инициализируем библиотеку стандартных органов управления
		InitCommonControls();

		// Регистрируем класс окна
		memset(&wc, 0, sizeof(wc));

		wc.cbSize = sizeof(WNDCLASSEX);		

		wc.style = 0;

		wc.lpfnWndProc = (WNDPROC)WndProcTreeWnd;

		wc.cbClsExtra = 0;

		wc.cbWndExtra = 0;

		wc.hInstance = wcex.hInstance;		

		wc.hCursor = LoadCursor(NULL, IDC_ARROW);

		wc.hbrBackground = (HBRUSH)hbrushForDialog;		

		wc.lpszClassName = L"TreeWBur";

		if (!RegisterClassEx(&wc))
			if (!RegisterClass((LPWNDCLASS)&wc.style))
				return FALSE;

		RECT mainWinTreeBuilder;

		GetClientRect(hWnd, &mainWinTreeBuilder);

		// Создаем орган управления Tree View
		hwndTree = CreateWindowEx(0L, WC_TREEVIEW, L"",
			WS_VISIBLE | WS_CHILD |
			TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS,
			(mainWinTreeBuilder.right) + 40, mainWinTreeBuilder.top + MARGIN_TOP + 70, ((mainWin.right - mainWin.left) + MARGIN_RIGHT) - ((mainWinTreeBuilder.right) + 48), ((mainWin.bottom - mainWin.top) + MARGIN_BOTTOM - 40) - 70,
			hwndFrame, (HMENU)IDC_TREEVIEW, wcex.hInstance, NULL);

		if (hwndTree == NULL)
			return FALSE;

		cloud_panel_w = (mainWinTreeBuilder.right) + 40;

		cloud_panel_h = mainWinTreeBuilder.top + MARGIN_TOP;

		TreeView_SetBkColor(hwndTree, RGB(30, 30, 30));		

		TreeView_SetTextColor(hwndTree, RGB(235, 235, 235));

		TreeView_SetItemHeight(hwndTree, TreeView_GetItemHeight(hwndTree) + 6);

		{
			// Создаем список изображений
			himl = ImageList_Create(16, 16, ILC_COLOR32, 3, 1);

			// Добавляем изображения в список
			//HICON hBmp = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_ICON2));

			hBmp = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BITMAP31));

			idx[0] = ImageList_Add(himl, hBmp, NULL);

			hBmp = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BITMAP33));

			idx[1] = ImageList_Add(himl, hBmp, NULL);

			hBmp = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BITMAP34));

			idx[2] = ImageList_Add(himl, hBmp, NULL);

			hBmp = LoadBitmap(wc.hInstance, MAKEINTRESOURCE(IDB_BITMAP39));

			idx[3] = ImageList_Add(himl, hBmp, NULL);

			/*************************************************************************************************************************/
                
			// Добавляем список изображений
			TreeView_SetImageList(hwndTree, himl, TVSIL_NORMAL);

			for (UINT32 i = 0; i < 9; i++)
			{
				HTREEITEM hRootItemSPI = InsTreeItem((HTREEITEM)TVI_ROOT, rgStepBookInfoUniversal[i].szBookName,
					(HTREEITEM)TVI_FIRST, idx[0], idx[0]);
			}

			/*---------------------------------------------------------------------------------------------------------*/

		//// Вставляем корневой элемент
		//	HTREEITEM hRootItemSPI = InsTreeItem((HTREEITEM)TVI_ROOT, SPI,
		//		(HTREEITEM)TVI_FIRST, idx[0], idx[0]);

		//	for (int j = 0; j < SPI_INT; j++) {

		//		// Вставляем вложенные элементы второго уровня
		//		InsTreeItem(hRootItemSPI, rgStepSPI[j].szBookName,
		//			(HTREEITEM)TVI_SORT, idx[2], idx[2]);
		//	}

		//	TreeView_Expand(hwndTree, hRootItemSPI, TVM_EXPAND);

		//	/*---------------------------------------------------------------------------------------------------------*/

		//	// Вставляем корневой элемент
		//	hRootItem = InsTreeItem((HTREEITEM)TVI_ROOT, szBooks,
		//		(HTREEITEM)TVI_FIRST, idx[0], idx[0]);

		//	// Вставляем вложенные элементы первого уровня
		//	hBSPItem = InsTreeItem(hRootItem, szBSPSeries,
		//		(HTREEITEM)TVI_LAST, idx[1], idx[1]);

		//	hStepItem = InsTreeItem(hRootItem, szStepSeries,
		//		(HTREEITEM)TVI_LAST, idx[1], idx[1]);

		//	// Вставляем вложенные элементы второго уровня
		//	InsTreeItem(hBSPItem, rgSPLBookInfo[0].szBookName,
		//		(HTREEITEM)TVI_SORT, idx[3], idx[3]);

		//	InsTreeItem(hBSPItem, rgSPLBookInfo[1].szBookName,
		//		(HTREEITEM)TVI_SORT, idx[3], idx[3]);

		//	InsTreeItem(hStepItem, rgStepBookInfo[0].szBookName,
		//		(HTREEITEM)TVI_SORT, idx[3], idx[3]);

		//	InsTreeItem(hStepItem, rgStepBookInfo[1].szBookName,
		//		(HTREEITEM)TVI_SORT, idx[3], idx[3]);

			//TreeView_Expand(hwndTree, hRootItem, TVM_EXPAND);

			//TreeView_Expand(hwndTree, hBSPItem, TVM_EXPAND);

			//TreeView_Expand(hwndTree, hStepItem, TVM_EXPAND);
		}		

		ShowWindow(hwndTree, SW_HIDE);

		ShowWindow(hWnd, SW_HIDE);

		VISIBLEpODLO = true;//нужна, что бы не было мерцания,т.к при создания диалога,было его появление и потом скрытие

		SendMessage(hwndFrame, WM_COMMAND, DELAY_TOP_MSG, NULL);
							
		/******************************************************/

		InvalidateRect(hwndFrame, NULL, false);

		UpdateWindow(hwndFrame);

		/******************************************************/

		InvalidateRect(hWnd, NULL, false);

		UpdateWindow(hWnd);

		/******************************************************/

		CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);// если нужно, что бы окно для оплаты вылезло сразу

	}	
	
	for (int i = 0; i < THREADS_NUMBER; i++) {

		hThreads[i] = CreateThread(NULL, 0, &ThreadProc, NULL, 0, NULL);

		if (NULL == hThreads[i])
		{
			MessageBox(NULL, L"A critical error has occurred", L"Attention!", NULL);

			exit(0);
		}

		//SuspendThread(hThreads[i]);
	}
	
	hThreadsWork = CreateThread(NULL, 0, &ThreadProcGetMessage, NULL, 0, NULL);

	if (NULL == hThreadsWork)
	{
		MessageBox(NULL, L"A critical error has occurred", L"Attention!", NULL);

		exit(0);
	}

	MSG msg = { 0 };    //структура сообщения

	int iGetOk = 0;   //переменная состояния	

	while ((iGetOk = GetMessage(&msg, NULL, 0, 0)) != 0)
	{
		//Sleep(1);

		//WaitMessage();

		if (iGetOk == -1) return 3;

		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}

	delete[] this_tools;

	delete[] this_toolsTips;

	delete[] this_toolsTips_0;

	DeleteObject(hbrushForDialog);

	DeleteObject(hbrushForRect);

	FreeLibrary(hinstLib);

	return 0;
}

DLGPROC GoToProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	wchar_t buff[100];

	static int start_prog = 0;

	static INT count_iterration = 0;
	
	static HBRUSH defaultbrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH AdvancerBoxBrushOverM = NULL;

	static HBRUSH AdvancerBoxBrush = NULL;

	static HBRUSH AdvancerBoxBrushUppdate = NULL;

	static HBRUSH AdvancerBoxBrushUppdateM = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	static RECT hRectButton = { 0 };

	static bool pressCursor = false;

	static POINT g_ptMousePos;

	switch (message)
	{

	case WM_TIMER:

		switch (wParam)
		{
			case IDT_TIMER1:
			
			{
				RECT r;

				GetClientRect(hwndFrame, &r);

				r.left = left_panel_toolsBG_X + 5;

				r.top = r.bottom - bottom_panel_indicateY;

				r.bottom = r.bottom - 5;

				r.right = r.right - 5;

				nCountIndicateRevers++;

				nCountIndicate += 5;

				InvalidateRect(hwndFrame, &r, false);
				
			}			
			
				return 0;
		}

		break;	
	
	case WM_INITDIALOG:	
			
		//SetTimer(hwndDlg,             // дескриптор главного окна
			//IDT_TIMER1,            // идентификатор таймера
			//1000,                 // 10-секундный интервал
			//NULL);			

		hPen = CreatePen(
			PS_SOLID,   // стиль пера
			static_cast<int>(0.1f),       // ширина пера
			RGB(21,82,100)); // цвет пера

		hPenGold = CreatePen(
			PS_SOLID,   // стиль пера
			1,       // ширина пера
			RGB(255, 219, 15)); // цвет пера

		ItemBindStartInsert = true;//инициализация для бинда

		SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)wcex.hIcon);
		
		AddItems(GetDlgItem(hwndDlg, IDC_COMBOBOXEX1));

		SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX1, CB_SETCURSEL, cur_Item, 0);

		/**************************************************************************/

		AddItems_Key(GetDlgItem(hwndDlg, IDC_COMBOBOXEX2));

		SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX2, CB_SETCURSEL, cur_Item_Key, 0);

		/**************************************************************************/

		AddItems_Random(GetDlgItem(hwndDlg, IDC_COMBOBOXEX3));

		SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX3, CB_SETCURSEL, cur_Item_Random, 0);

		/**************************************************************************/

		AddItems_Key_Bind(GetDlgItem(hwndDlg, IDC_COMBOBOXEX4));

		SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX4, CB_SETCURSEL, curItemBind, 0);

		/**************************************************************************/

		AddItems_VariosTime(GetDlgItem(hwndDlg, IDC_COMBOBOXEX5));

		SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX5, CB_SETCURSEL, curItemVariosTime, 0);

		/*---------------------*/

		if(cur_Item_Key == 48)
		{
			stop_absolute = VK_ESCAPE;

			if (curItemBind < 36) {

				bindAbsolute = vtable[curItemBind];
			}
			else 
			{
				bindAbsolute = vtable[curItemBind + 10];
			}
		}
		else if(cur_Item_Key < 36)
		{
			stop_absolute = vtable[cur_Item_Key]; 

			bindAbsolute = vtable[curItemBind];
		}
		else 
		{

			stop_absolute = vtable[cur_Item_Key + 10];

			bindAbsolute = vtable[curItemBind + 10];
		}

		/**************************************************************************/

		sliderBar = GetDlgItem(hwndDlg, IDC_SLIDER1);

		SendMessage(sliderBar, TBM_SETRANGEMIN, false, 0);   //минимум

		SendMessage(sliderBar, TBM_SETRANGEMAX, false, 1000); //максимум

		SendMessage(sliderBar, TBM_SETTICFREQ, false, 1);   //шаг	

		if (start_prog == 0)
		{
			SendMessage(sliderBar, TBM_SETPOS, 1, 970); // Получаем текущее положение слайдера	
			
			start_prog++;
		}

		curNumb = SendMessage(sliderBar, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера			
		
		wsprintfW(buff, L"%d", curNumb); // Преобразуем число к строке
		
		SetDlgItemText(hwndDlg, IDC_STATIC25, buff);

		a = IsDlgButtonChecked(hwndDlg, IDC_CHECK1);

		if (a)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK1, BST_CHECKED);// поставить галочку 
		}

		b = IsDlgButtonChecked(hwndDlg, IDC_CHECK2);

		if (b)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK2, BST_CHECKED);// поставить галочку //цикл
		}

		c = IsDlgButtonChecked(hwndDlg, IDC_CHECK3);

		if (!c)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK3, BST_CHECKED);// поставить галочку
		}

		d = IsDlgButtonChecked(hwndDlg, IDC_CHECK4);

		if (d)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK4, BST_CHECKED);// поставить галочку
		}

		h = IsDlgButtonChecked(hwndDlg, IDC_CHECK5);

		if (h)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK5, BST_CHECKED);// поставить галочку
		}

		human = IsDlgButtonChecked(hwndDlg, IDC_CHECK6);

		if (human)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK6, BST_CHECKED);// поставить галочку
		}

		click = IsDlgButtonChecked(hwndDlg, IDC_CHECK7);

		if (click)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK7, BST_CHECKED);// поставить галочку
		}
		
		modeSleep = IsDlgButtonChecked(hwndDlg, IDC_CHECK8);

		if (modeSleep)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK8, BST_CHECKED);// поставить галочку
		}

		leftButtonEx = IsDlgButtonChecked(hwndDlg, IDC_CHECK9);

		if (leftButtonEx)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK9, BST_CHECKED);// поставить галочку
		}

		rightButtonEx = IsDlgButtonChecked(hwndDlg, IDC_CHECK10);

		if (rightButtonEx)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK10, BST_CHECKED);// поставить галочку
		}

		InfiniteWork = IsDlgButtonChecked(hwndDlg, IDC_CHECK11);

		if (InfiniteWork)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK11, BST_CHECKED);// поставить галочку
		}

		shutDown = IsDlgButtonChecked(hwndDlg, IDC_CHECK12);

		if (shutDown)
		{
			CheckDlgButton(hwndDlg, IDC_CHECK12, BST_CHECKED);// поставить галочку
		}		

		topMost = IsDlgButtonChecked(hwndDlg, IDC_CHECK13);

		if (topMost && VISIBLEpODLO) {

			GetWindowRect(hwndDlg, &glRect);
			
			SetWindowPos(hwndFrame, HWND_TOPMOST, glRect.left - MARGIN_LEFT, glRect.top - MARGIN_TOP, (glRect.right - glRect.left) + MARGIN_RIGHT, (glRect.bottom - glRect.top) + MARGIN_BOTTOM, SWP_SHOWWINDOW);

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);
		}
		else if(VISIBLEpODLO)
		{
			GetWindowRect(hwndDlg, &glRect);

			SetWindowPos(hwndFrame, HWND_NOTOPMOST, glRect.left - MARGIN_LEFT, glRect.top - MARGIN_TOP, (glRect.right - glRect.left) + MARGIN_RIGHT, (glRect.bottom - glRect.top) + MARGIN_BOTTOM, SWP_SHOWWINDOW);

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);
		}		

		{

			HWND g = GetDlgItem(hwndDlg, IDCANCEL7);

			GetWindowRect(g, &hRectButton);

			GetClientRect(hwndDlg, &hRectMainW);
		}

		break;

	case WM_CTLCOLORDLG:

	{	
		return (DLGPROC)hbrushForDialog;
	}
		

	case WM_MOUSEMOVE:

	{		
		SendMessage(hwndFrame, WM_MOUSEMOVE, NULL, NULL);		
		
		if (pressCursor)
		{
			RECT rc;

			POINT ptCursor;

			POINT ptDelta;

			GetWindowRect(hwndDlg, &rc);

			GetCursorPos(&ptCursor);

			ptDelta.x = g_ptMousePos.x - ptCursor.x;

			ptDelta.y = g_ptMousePos.y - ptCursor.y;

			MoveWindow(hwndDlg, rc.left - ptDelta.x, rc.top - ptDelta.y,
				rc.right - rc.left, rc.bottom - rc.top, TRUE);

			MoveWindow(hwndFrame, (rc.left - ptDelta.x) - MARGIN_LEFT, (rc.top - ptDelta.y) - MARGIN_TOP,
				(rc.right - rc.left) + MARGIN_RIGHT, (rc.bottom - rc.top) + MARGIN_BOTTOM, TRUE);

			g_ptMousePos.x = ptCursor.x;

			g_ptMousePos.y = ptCursor.y;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);
		}
		
		
		int xPos = GET_X_LPARAM(lParam);

		int yPos = GET_Y_LPARAM(lParam);		

		auto p = [](HWND hwndDlg, int x, int y)

		{			

			HWND h = GetDlgItem(hwndDlg, IDC_STATIC337);

			RECT r;

			GetWindowRect(h, &r);			

			POINT pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);
						

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				SetCursor(hFinger);	

				SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 10, pt.y + 10, 200, 40, SWP_SHOWWINDOW);

				InvalidateRect(hwndTip, NULL, false);

				UpdateWindow(hwndTip);
			}
			else 
			{
				SetWindowPos(hwndTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW);
			}

			//*************************************************************************************

			h = GetDlgItem(hwndDlg, IDC_STATIC400);

			memset(&r, 0, sizeof(r));

			GetWindowRect(h, &r);

			pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);				

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				SetCursor(hFinger);

				SetWindowPos(hwndTip_0, HWND_TOPMOST, pt.x + 10, pt.y + 10, 200, 40, SWP_SHOWWINDOW);

				InvalidateRect(hwndTip_0, NULL, false);

				UpdateWindow(hwndTip_0);
			}
			else
			{
				SetWindowPos(hwndTip_0, HWND_TOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW);
			}

			//*************************************************************************************

			h = GetDlgItem(hwndDlg, IDC_STATIC403);

			memset(&r, 0, sizeof(r));

			GetWindowRect(h, &r);

			pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				SetCursor(hFinger);

				SetWindowPos(hwndTip_1, HWND_TOPMOST, pt.x + 10, pt.y + 10, 200, 40, SWP_SHOWWINDOW);

				InvalidateRect(hwndTip_1, NULL, false);

				UpdateWindow(hwndTip_1);
			}
			else
			{
				SetWindowPos(hwndTip_1, HWND_TOPMOST, 0, 0, 0, 0, SWP_HIDEWINDOW);
			}

		};

		//p(hwndDlg, xPos, yPos);
		
	}
		break;

	case WM_LBUTTONUP:

	{
		if (pressCursor)
		{
			ReleaseCapture();

			pressCursor = false;
		}
	}

		break;

	case WM_LBUTTONDOWN:

	{		

		if (!pressCursor)
		{
			SetCapture(hwndDlg);

			pressCursor = true;

			GetCursorPos(&g_ptMousePos);
		}
		
		int xPos = GET_X_LPARAM(lParam);

		int yPos = GET_Y_LPARAM(lParam);

		auto p = [](HWND hwndDlg, int x, int y)

		{

			HWND h = GetDlgItem(hwndDlg, IDC_STATIC337);

			RECT r;

			GetWindowRect(h, &r);

			POINT pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				if (ext)
				{
					ShowWindow(hWndReg, SW_SHOW);

					return 0;
				}
			}


			//*************************************************************************************

			h = GetDlgItem(hwndDlg, IDC_STATIC400);

			memset(&r, 0, sizeof(r));

			GetWindowRect(h, &r);

			pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				if (ext)
				{
					ShowWindow(hWndReg, SW_SHOW);

					return 0;
				}
			}

			//*************************************************************************************

			h = GetDlgItem(hwndDlg, IDC_STATIC403);

			memset(&r, 0, sizeof(r));

			GetWindowRect(h, &r);

			pt;

			pt.x = x;

			pt.y = y;

			ClientToScreen(hwndDlg, &pt);

			if (r.left < pt.x && pt.x < r.right && r.top < pt.y && pt.y < r.bottom) {

				if (ext)
				{
					ShowWindow(hWndReg, SW_SHOW);

					return 0;
				}
			}

		};

		//p(hwndDlg, xPos, yPos);	
		
	}

	break;	

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;		

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);		

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;

	case WM_HSCROLL:
	{
		if (sliderBar == (HWND)lParam) //если речь идет о нашел слайдере
		{
			curNumb = SendMessage(sliderBar, TBM_GETPOS, 0, 0); // Получаем текущее положение слайдера			
			
			wsprintfW(buff, L"%d", curNumb); // Преобразуем число к строке
			
			SetDlgItemText(hwndDlg, IDC_STATIC25, buff);														/* Делаем с ним, что хотим */
		}

		break;
	}

	case WM_NOTIFY:

		switch (((LPNMHDR)lParam)->code)
		{        

		case NM_CLICK:          // Fall through to the next case.
		{
			PNMLINK pNMLink = (PNMLINK)lParam; 
		
			pNMLink->item.state  &= ~(CDIS_FOCUS ^ CDIS_SELECTED);
		}
		case NM_RETURN:

			{
				PNMLINK pNMLink = (PNMLINK)lParam;

				LITEM   item = pNMLink->item;

				if (pNMLink->hdr.idFrom == IDC_SYSLINK1)
				{
					//MessageBox(0, L"No network connection [INVALID_SOCKET]", L"Attention!", MB_OK + MB_ICONWARNING);					
				}
			
			}		
		}
		
		{//start

			LPNMHDR some_item = (LPNMHDR)lParam;

			if ((some_item->idFrom == IDC_BUTTON9) && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

				{

					if (item->uItemState & CDIS_HOT) //Our mouse is over the button
					{
						SetCursor(hFinger);

						if (AdvancerBoxBrushUppdateM == NULL)
						{
							AdvancerBoxBrushUppdateM = CreateGradientBrush(RGB(60, 60, 60), RGB(30, 30, 30), item);
						}

						HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(90, 90, 90));

						HGDIOBJ old_pen = SelectObject(item->hdc, pen);

						HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrushUppdateM);

						RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

						SelectObject(item->hdc, old_pen);

						SelectObject(item->hdc, old_brush);

						DeleteObject(pen);

						return CDRF_DODEFAULT;
					}


					if (AdvancerBoxBrushUppdate == NULL)
					{
						AdvancerBoxBrushUppdate = CreateGradientBrush(RGB(80, 80, 80), RGB(45, 45, 45), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(90, 90, 90));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrushUppdate);					

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}
			}

			else if ((some_item->idFrom == IDC_BUTTON3 || some_item->idFrom == IDOK || some_item->idFrom == IDOK4) && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

				{

					if (item->uItemState & CDIS_HOT) //Our mouse is over the button
					{
						SetCursor(hFinger);

						if (AdvancerBoxBrushOverM == NULL)
						{
							AdvancerBoxBrushOverM = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 70, 70), item);
						}

						HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 50, 50));

						HGDIOBJ old_pen = SelectObject(item->hdc, pen);

						HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrushOverM);

						RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

						SelectObject(item->hdc, old_pen);

						SelectObject(item->hdc, old_brush);

						DeleteObject(pen);

						return CDRF_DODEFAULT;
					}


					if (AdvancerBoxBrush == NULL)
					{
						AdvancerBoxBrush = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 100, 100), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 55, 55));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrush);					

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}
			}
			else if ((some_item->idFrom == IDC_BUTTON1 || some_item->idFrom == IDC_BUTTON8) && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

				{
					if (GroupBoxBrush == NULL)
					{
						GroupBoxBrush = CreateGradientBrush(RGB(30, 30, 30), RGB(30, 30, 30), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, GroupBoxBrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 0, 0);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}
			}
			else if ((some_item->idFrom == IDOK10 || some_item->idFrom == IDOK9 || some_item->idFrom == IDOK8 || some_item->idFrom == IDOK2 || some_item->idFrom == IDOK3
				|| some_item->idFrom == IDOK5 || some_item->idFrom == IDOK6
				|| some_item->idFrom == IDCANCEL || some_item->idFrom == IDCANCEL2 || some_item->idFrom == IDCANCEL5
				|| some_item->idFrom == IDCANCEL7 || some_item->idFrom == IDCANCEL6 || some_item->idFrom == IDCANCEL4) && some_item->code == NM_CUSTOMDRAW)
			{
				LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

				if (item->uItemState & CDIS_SELECTED)
				{
					//Select our color when the button is selected

					if (selectbrush == NULL)
					{
						selectbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
					}

					//Create pen for button border
					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

					//Select our brush into hDC
					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

					//If you want rounded button, then use this, otherwise use FillRect().
					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					//Clean up
					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					//Now, I don't want to do anything else myself (draw text) so I use this value for return:
					return CDRF_DODEFAULT;
					//Let say I wanted to draw text and stuff, then I would have to do it before return with
					//DrawText() or other function and return CDRF_SKIPDEFAULT
				}
				else
				{
					if (item->uItemState & CDIS_HOT) //Our mouse is over the button
					{
						//Select our color when the mouse hovers our button

						if (hotbrush == NULL)
						{
							hotbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
						}

						HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

						HGDIOBJ old_pen = SelectObject(item->hdc, pen);

						HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

						RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

						SelectObject(item->hdc, old_pen);

						SelectObject(item->hdc, old_brush);

						DeleteObject(pen);

						return CDRF_DODEFAULT;
					}

					//Select our color when our button is doing nothing

					if (defaultbrush == NULL)
					{
						defaultbrush = CreateGradientBrush(RGB(40, 165, 196), RGB(102, 199, 223), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}
			}

			return CDRF_DODEFAULT;

		}//end			

		break;

	case WM_PAINT:

	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hwndDlg, &ps);

		RECT rt, rt1, rt2, rt3;

		GetClientRect(hwndDlg, &rt);

		rt.left += 10 * scaleX;

		rt.right -= 14 * scaleX;

		rt.top = rt.bottom - 280 * scaleY;

		int tmp = rt.bottom - (280 / 2) * scaleY;

		rt.bottom = rt.bottom - 50 * scaleY;

		tmp -= (50 / 2) * scaleY;

		rt2 = rt;

		FillRect(hdc, &rt, hbrushNew);

		FrameRect(hdc, &rt, hbrushNewBlueEx);

		waveInfo = rt;

		rt1 = rt;

		rt1.left += 5;

		rt1.right -= 5;

		rt1.top = rt.bottom - 25 * scaleY;

		rt1.bottom = rt.bottom - 5 * scaleY;

		rt3 = rt1;

		FillRect(hdc, &rt1, hbrushNewBlue);

		SetTextColor(hdc, RGB(255, 255, 255));

		SetBkMode(hdc, TRANSPARENT);

		SelectObject(hdc, hFont);

		rt3.left += 10;

		DrawText(hdc, L"Построение пиков", wcslen(L"Построение пиков"), &rt3, DT_SINGLELINE | DT_VCENTER);

		SIZE xV;

		GetTextExtentPoint32(hdc, L"Построение пиков", wcslen(L"Построение пиков"), &xV);

		rt3.left += xV.cx;

		rt3.left += 50;

		wchar_t buf[20];

		memset(buf, 0, sizeof(wchar_t) * 20);

		wsprintf(buf, L"%d", numFramesAvailable);

		wstring str = L"Размер пакета : ";

		str += buf;

		DrawText(hdc, str.c_str(), str.size(), &rt3, DT_SINGLELINE | DT_VCENTER);

		{//желтая линия, горизонт

			HGDIOBJ old = SelectObject(hdc, hPenGold);

			DrawLine(hdc, rt2.left + 3, tmp, rt2.right - 3, tmp);

			SelectObject(hdc, old);
		}

		if (true)

		{

			{//blue линия, горизонт

				HGDIOBJ old = SelectObject(hdc, hPenBlue);

				for (int j = rt2.left + 3; j < rt2.right - 3; j += 3)
				{
					int step = rand() % 70;

					DrawLine(hdc, j, tmp, j, tmp + step);

					DrawLine(hdc, j, tmp, j, tmp - step);
				}

				SelectObject(hdc, old);
			}


			{//желтая линия, горизонт

				HGDIOBJ old = SelectObject(hdc, hPenGold);

				for (int j = rt2.left + 3; j < rt2.right - 3; j += 1)
				{
					int step = rand() % 40;

					DrawLine(hdc, j, tmp, j, tmp + step);

					DrawLine(hdc, j, tmp, j, tmp - step);
				}

				SelectObject(hdc, old);
			}
		}

		EndPaint(hwndDlg, &ps);
	}

	break;

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK9:

		{

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}			
			
		}

			break;

		case IDC_CHECK13:

			topMost = IsDlgButtonChecked(hwndDlg, IDC_CHECK13);

			if (topMost) {

				GetWindowRect(hwndDlg, &glRect);

				SetWindowPos(hwndFrame, HWND_TOPMOST, glRect.left - MARGIN_LEFT, glRect.top - MARGIN_TOP, (glRect.right - glRect.left) + MARGIN_RIGHT, (glRect.bottom - glRect.top) + MARGIN_BOTTOM, SWP_SHOWWINDOW);
			}
			else
			{
				GetWindowRect(hwndDlg, &glRect);

				SetWindowPos(hwndFrame, HWND_NOTOPMOST, glRect.left - MARGIN_LEFT, glRect.top - MARGIN_TOP, (glRect.right - glRect.left) + MARGIN_RIGHT, (glRect.bottom - glRect.top) + MARGIN_BOTTOM, SWP_SHOWWINDOW);
			}

			break;

		case IDOK:

		{
			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			
		}
			break;			

		case IDOK4:

			if (store.size() == 0) {

				MessageBox(NULL, L"Необходимо записать координаты курсора мыши!", L"Внимание!", MB_TOPMOST);
			}		

			break;

		case IDCANCEL4:

		{			
				if (ext)
				{
					ShowWindow(hWndReg, SW_SHOW);

					return 0;
				}		   
		}

			break;

		case IDCANCEL5:

		{
			ShowWindow(hWndInfoManual, SW_SHOW);
		}

		break;

		case IDCANCEL6:

		{
			startworklist = true;

			hSmallImgBigList = ImageList_Create(GetSystemMetrics(SM_CXSMICON), GetSystemMetrics(SM_CYSMICON), ILC_MASK | ILC_COLOR32, 1, 1);

			CheckDlgButton(hWndDebug, IDC_CHECK1, BST_CHECKED);// поставить галочку	

			statedbgClick = false;

			ShowWindow(hWndDebug, SW_SHOW);
		}

		break;

		case IDCANCEL7:

		{
			ShowWindow(hWndReg, SW_SHOW);
		}

		break;

		case IDC_BUTTON3:

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;

		case IDC_BUTTON9:
			
			ShowWindow(hWnd_net, SW_SHOW);
			
			break;

		case IDOK3:

		{
			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}			
		}

		break;

		case IDC_COMBOBOXEX1:

			if (HIWORD(wParam) == CBN_SELCHANGE)

			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				TCHAR  ListItem[256];

				memset(ListItem, 0, 256 * sizeof(TCHAR));

				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);

				SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX1, CB_SETCURSEL, ItemIndex, 0);

				cur_Item = ItemIndex;
			}

			break;

		case IDC_COMBOBOXEX2:

			if (HIWORD(wParam) == CBN_SELCHANGE)

			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				TCHAR  ListItem[256];

				memset(ListItem, 0, 256 * sizeof(TCHAR));

				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);

				SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX2, CB_SETCURSEL, ItemIndex, 0);

				cur_Item_Key = ItemIndex;

				SendMessage(hwndDlg, WM_INITDIALOG, NULL, NULL);
			}

			break;

		case IDC_COMBOBOXEX3:

			if (HIWORD(wParam) == CBN_SELCHANGE)

			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				TCHAR  ListItem[256];

				memset(ListItem, 0, 256 * sizeof(TCHAR));

				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);

				SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX3, CB_SETCURSEL, ItemIndex, 0);

				cur_Item_Random = ItemIndex;

				SendMessage(hwndDlg, WM_INITDIALOG, NULL, NULL);
			}

			break;

		case IDC_COMBOBOXEX4:

			if (HIWORD(wParam) == CBN_SELCHANGE)

			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				TCHAR  ListItem[256];

				memset(ListItem, 0, 256 * sizeof(TCHAR));

				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);

				SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX4, CB_SETCURSEL, ItemIndex, 0);

				curItemBind = ItemIndex;

				SendMessage(hwndDlg, WM_INITDIALOG, NULL, NULL);
			}

			break;

		case IDC_COMBOBOXEX5:

			if (HIWORD(wParam) == CBN_SELCHANGE)

			{
				int ItemIndex = SendMessage((HWND)lParam, (UINT)CB_GETCURSEL,
					(WPARAM)0, (LPARAM)0);

				TCHAR  ListItem[256];

				memset(ListItem, 0, 256 * sizeof(TCHAR));

				(TCHAR)SendMessage((HWND)lParam, (UINT)CB_GETLBTEXT,
					(WPARAM)ItemIndex, (LPARAM)ListItem);

				SendDlgItemMessage(hwndDlg, IDC_COMBOBOXEX5, CB_SETCURSEL, ItemIndex, 0);//важно не забывать менять IDC_COMBOBOXEX5 и case

				curItemVariosTime = ItemIndex;

				SendMessage(hwndDlg, WM_INITDIALOG, NULL, NULL);
			}

			break;

		case IDOK2:

			if (store.size() == 0) {

				MessageBox(NULL, L"Необходимо записать координаты курсора мыши!", L"Внимание!", MB_TOPMOST);
			}		

			break;

		case IDCANCEL:
						
			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;		

		case IDOK6:

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			ProcAddSh();

			break;

		case IDOK5:

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}			

			break;

		case IDOK8:

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}			

			break;	

		case IDOK10:

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}		
	}

	return FALSE;
}


DLGPROC GoToProcReg(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH defaultbrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	switch (message)
	{

	case WM_NOTIFY:


	{//start

		LPNMHDR some_item = (LPNMHDR)lParam;

		if ((some_item->idFrom == IDOK || some_item->idFrom == IDCANCEL || some_item->idFrom == IDCANCEL3) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (item->uItemState & CDIS_SELECTED)
			{
				//Select our color when the button is selected

				if (selectbrush == NULL)
				{
					selectbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
				}

				//Create pen for button border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(166, 166, 166));

				//Select our brush into hDC
				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

				//If you want rounded button, then use this, otherwise use FillRect().
				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				//Clean up
				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//Now, I don't want to do anything else myself (draw text) so I use this value for return:
				return CDRF_DODEFAULT;
				//Let say I wanted to draw text and stuff, then I would have to do it before return with
				//DrawText() or other function and return CDRF_SKIPDEFAULT
			}
			else
			{
				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					//Select our color when the mouse hovers our button

					if (hotbrush == NULL)
					{
						hotbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(166, 166, 166));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}

				//Select our color when our button is doing nothing

				if (defaultbrush == NULL)
				{
					defaultbrush = CreateGradientBrush(RGB(40, 165, 196), RGB(102, 199, 223), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				return CDRF_DODEFAULT;
			}
		}

		return CDRF_DODEFAULT;

	}//end	

	break;
	
	case WM_INITDIALOG:

		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)wcex.hIcon);

		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_DESTROY:
		
		{			
			ShowWindow(hwndDlg, SW_HIDE);
		}

		break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;

	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDOK:

			{
			   char login[100];

			   memset(login, 0, sizeof(char) * 100);

			   GetDlgItemTextA(hwndDlg, IDC_EDIT1, login, 100);

			   int sz = strlen(login);

			   if (sz == 0) {

				   MessageBox(NULL, L"You did not specify the program registration key", L"Attention!", NULL);				   
			   }

			  if (true) {

				   MessageBox(NULL, L"Invalid program registration key or the program is blocked", L"Attention!", NULL);				  
			   }			  
			}			

			break;

		case IDCANCEL3:

		{
			
		}

			break;
		
		case IDCANCEL:

		{
			ShowWindow(hwndDlg, SW_HIDE);
		}
			break;		
		}
	}

	return FALSE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	

	switch (message)
	{	

		case WM_CLOSE:

		{
			ShowWindow(hWnd, SW_HIDE);
		}

		break;

		case WM_LBUTTONDOWN:

		{
			MessageBox(NULL, L"Для продолжения работы программы, необходимо приобрести ключ регистрации!", L"Внимание!", MB_TOPMOST);
		}

			break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;

			HDC hdc = BeginPaint(hWnd, &ps);			

			BITMAPFILEHEADER   bmfHeader = { NULL };

			BITMAPINFOHEADER   bi = { NULL };

			bi.biSize = sizeof(BITMAPINFOHEADER);

			bi.biWidth = netw_config_w;

			bi.biHeight = netw_config_h;

			bi.biPlanes = 1;

			bi.biBitCount = 32;

			bi.biCompression = BI_RGB;

			bi.biSizeImage = 0;

			bi.biXPelsPerMeter = 0;

			bi.biYPelsPerMeter = 0;

			bi.biClrUsed = 0;

			bi.biClrImportant = 0;

			SetStretchBltMode(hdc, HALFTONE);

			if (GDI_ERROR == StretchDIBits(hdc, 0, 0, bi.biWidth, bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, networks_config, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hWnd, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}

			EndPaint(hWnd, &ps);
		}

		break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DLGPROC GoToProcInfo(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH defaultbrush = NULL;

	static HBRUSH AdvancerBoxBrushOverM = NULL;

	static HBRUSH AdvancerBoxBrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	switch (message)
	{

	case WM_NOTIFY:


	{//start

		LPNMHDR some_item = (LPNMHDR)lParam;

		if ((some_item->idFrom == IDOK || some_item->idFrom == IDOK3 || some_item->idFrom == IDOK5) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (item->uItemState & CDIS_SELECTED)
			{
				//Select our color when the button is selected				

				if (selectbrush == NULL)
				{
					selectbrush = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 70, 70), item);
				}

				//Create pen for button border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 50, 50));

				//Select our brush into hDC
				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

				//If you want rounded button, then use this, otherwise use FillRect().
				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				//Clean up
				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//Now, I don't want to do anything else myself (draw text) so I use this value for return:
				return CDRF_DODEFAULT;
				//Let say I wanted to draw text and stuff, then I would have to do it before return with
				//DrawText() or other function and return CDRF_SKIPDEFAULT
			}
			else
			{
				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					//Select our color when the mouse hovers our button					

					if (hotbrush == NULL)
					{
						hotbrush = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 40, 40), item);
					}									

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 50, 50));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}

				//Select our color when our button is doing nothing

				if (defaultbrush == NULL)
				{
					defaultbrush = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 100, 100), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 55, 55));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				///DrawText(item->hdc, L"1111", 4, &item->rc, DT_CENTER);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//return (DLGPROC)CDRF_NEWFONT;

				return CDRF_DODEFAULT;
			}
		}

		else if ((some_item->idFrom == IDC_BUTTON1) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			{

				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					SetCursor(hFinger);

					if (AdvancerBoxBrushOverM == NULL)
					{
						AdvancerBoxBrushOverM = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 70, 70), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 50, 50));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrushOverM);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}


				if (AdvancerBoxBrush == NULL)
				{
					AdvancerBoxBrush = CreateGradientBrush(RGB(255, 50, 50), RGB(255, 100, 100), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(255, 55, 55));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, AdvancerBoxBrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				return CDRF_DODEFAULT;
			}
		}

		return CDRF_DODEFAULT;

	}//end	

	break;

	case WM_INITDIALOG:

	{
		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)wcex.hIcon);
	}

		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hBrushRed;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR_INFO);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hBrushRed;
	}

	break;

	case WM_PAINT:

		{
			PAINTSTRUCT ps;

			RECT rcClient;

			HDC hdc = BeginPaint(hwndDlg, &ps);

			GetClientRect(hwndDlg, &rcClient);

			BITMAPFILEHEADER   bmfHeader = { NULL };

			BITMAPINFOHEADER   bi = { NULL };

			bi.biSize = sizeof(BITMAPINFOHEADER);

			bi.biWidth = bmpW;

			bi.biHeight = bmpH;

			bi.biPlanes = 1;

			bi.biBitCount = 32;

			bi.biCompression = BI_RGB;

			bi.biSizeImage = 0;

			bi.biXPelsPerMeter = 0;

			bi.biYPelsPerMeter = 0;

			bi.biClrUsed = 0;

			bi.biClrImportant = 0;

			SetStretchBltMode(hdc, HALFTONE);

			if (GDI_ERROR == StretchDIBits(hdc, 0, 0, bmpW, bmpH, 0, 0, bmpW, bmpH, this_tools, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}

			EndPaint(hwndDlg, &ps);
		}

	break;

	case WM_CLOSE:
		
		{
			ShowWindow(hwndDlg, SW_HIDE);

			ShowWindow(hWndReg, SW_SHOW);

			visible_period = true;

			sleep_time += 5000;

			ShellExecute(NULL, L"open", L"https://burundukpro.ru/pay-system", NULL, NULL, SW_SHOWNORMAL);
		}

		break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hBrushRed;


	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK:

		{
			ShowWindow(hwndDlg, SW_HIDE);

			ShowWindow(hWndReg, SW_SHOW);

			ShellExecute(NULL, L"open", L"https://burundukpro.ru/extended-pay/", NULL, NULL, SW_SHOWNORMAL);
		}

		break;

		case IDOK3:

		{
			//ShowWindow(hwndDlg, SW_HIDE);

			ShowWindow(hWndReg, SW_SHOW);

			//ShellExecute(NULL, L"open", L"https://burundukpro.ru/pay/", NULL, NULL, SW_SHOWNORMAL);
		}

		break;

		case IDOK5:

		{
			ShowWindow(hwndDlg, SW_HIDE);

			ShowWindow(hWndReg, SW_SHOW);

			ShellExecute(NULL, L"open", L"https://burundukpro.ru/standart-pay/", NULL, NULL, SW_SHOWNORMAL);
		}

		break;

		case IDC_BUTTON1:

		{
			
		}

		break;

		}
	}

	return FALSE;
}

DLGPROC GoToProcLisen(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH defaultbrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	switch (message)
	{

	case WM_NOTIFY:


	{//start

		LPNMHDR some_item = (LPNMHDR)lParam;

		if ((some_item->idFrom == IDOK) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (item->uItemState & CDIS_SELECTED)
			{
				//Select our color when the button is selected

				if (selectbrush == NULL)
				{
					selectbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
				}

				//Create pen for button border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				//Select our brush into hDC
				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

				//If you want rounded button, then use this, otherwise use FillRect().
				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				//Clean up
				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//Now, I don't want to do anything else myself (draw text) so I use this value for return:
				return CDRF_DODEFAULT;
				//Let say I wanted to draw text and stuff, then I would have to do it before return with
				//DrawText() or other function and return CDRF_SKIPDEFAULT
			}
			else
			{
				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					//Select our color when the mouse hovers our button

					if (hotbrush == NULL)
					{
						hotbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}

				//Select our color when our button is doing nothing

				if (defaultbrush == NULL)
				{
					defaultbrush = CreateGradientBrush(RGB(40, 165, 196), RGB(102, 199, 223), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				return CDRF_DODEFAULT;
			}
		}

		return CDRF_DODEFAULT;

	}//end	

	break;

	case WM_INITDIALOG:

	{
		HRSRC hRsrc;

		char far* lpXlatTable = nullptr;

		hRsrc = FindResource(wcex.hInstance, MAKEINTRESOURCE(IDR_HTML1), RT_HTML);

		// Получаем идентификатор ресурса
		HGLOBAL hXlatTable = LoadResource(wcex.hInstance, hRsrc);

		// Фиксируем ресурс в памяти, получая его адрес
		lpXlatTable = (char far*)LockResource(hXlatTable);

		// Если адрес равен NULL, при загрузке или
		// фиксации ресурса произошла ошибка
		if (lpXlatTable == NULL)
		{
			MessageBox(NULL, L"Error", L"Resource loading error",
				MB_OK);

			return 0;
		}

		string str = lpXlatTable;

		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)wcex.hIcon);

		SetWindowTextA(GetDlgItem(hwndDlg, IDC_EDIT1), str.c_str());
	}
		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_CLOSE:

	{
		ShowWindow(hwndDlg, SW_HIDE);
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;


	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		case IDOK:

		{
			ShowWindow(hwndDlg, SW_HIDE);
		}

		break;

		}
	}

	return FALSE;
}

DLGPROC GoToProcSetting(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH defaultbrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	switch (message)
	{

	case WM_NOTIFY:


	{//start

		LPNMHDR some_item = (LPNMHDR)lParam;

		if ((some_item->idFrom == IDOK || some_item->idFrom == IDOK5 || some_item->idFrom == IDCANCEL) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (item->uItemState & CDIS_SELECTED)
			{
				//Select our color when the button is selected

				if (selectbrush == NULL)
				{
					selectbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
				}

				//Create pen for button border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				//Select our brush into hDC
				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

				//If you want rounded button, then use this, otherwise use FillRect().
				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				//Clean up
				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//Now, I don't want to do anything else myself (draw text) so I use this value for return:
				return CDRF_DODEFAULT;
				//Let say I wanted to draw text and stuff, then I would have to do it before return with
				//DrawText() or other function and return CDRF_SKIPDEFAULT
			}
			else
			{
				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					//Select our color when the mouse hovers our button

					if (hotbrush == NULL)
					{
						hotbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}

				//Select our color when our button is doing nothing

				if (defaultbrush == NULL)
				{
					defaultbrush = CreateGradientBrush(RGB(40, 165, 196), RGB(102, 199, 223), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				return CDRF_DODEFAULT;
			}
		}

		return CDRF_DODEFAULT;

	}//end	

	break;

	case WM_INITDIALOG:

		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)wcex.hIcon);

		SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), hPatchGlobalForScripts.c_str());

		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_CLOSE:

	{
		ShowWindow(hwndDlg, SW_HIDE);
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hwndDlg, &ps);

		RECT r;

		GetClientRect(GetDlgItem(hwndDlg, IDC_EDIT1), &r);

		scaleX = GetDeviceCaps(hdc, LOGPIXELSX) / 96.0;

		scaleY = GetDeviceCaps(hdc, LOGPIXELSY) / 96.0;

		r.top = SCALEY(18);

		r.left = SCALEX(155);

		r.right = r.right + r.left + SCALEX(20);

		r.bottom = r.bottom + r.top + SCALEY(5);
		
		FrameRect(hdc, &r, hbrushForRect);

		EndPaint(hwndDlg, &ps);
	}

	break;

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{

		
		case IDC_CHECK1:

		{
			if (!IsDlgButtonChecked(hwndDlg, IDC_CHECK1)) {

				ItemBindStart = false;
			}
			else
			{
				ItemBindStart = true;
			}
		}

		break;		
		
		case IDOK:

		{
			ShowWindow(hwndDlg, SW_HIDE);
		}

		break;

		case IDOK5:

		{
			hPatchGlobalForScripts = get_patch_for_save_scripts(hwndDlg);

			SetWindowText(GetDlgItem(hwndDlg, IDC_EDIT1), hPatchGlobalForScripts.c_str());

			SetUserSetting(hPatchGlobalForScripts);
		}

		break;

		case IDCANCEL:

		{
			ShowWindow(hwndDlg, SW_HIDE);
		}

		break;

		}
	}

	return FALSE;
}

DLGPROC GoToProcToolTips(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HFONT hFont;

	switch (message)
	{
	
	
	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;	

	case WM_INITDIALOG:

	{
		hFont = CreateFont(16, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS,
			CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Arial"));
	}

	break;

	case WM_PAINT:

	{
		PAINTSTRUCT ps;

		RECT rcClient;

		HDC hdc = BeginPaint(hwndDlg, &ps);

		GetClientRect(hwndDlg, &rcClient);

		HGDIOBJ old = SelectObject(hdc, hFont);

		SetTextColor(hdc, RGB(255, 255, 255));

		SetBkMode(hdc, TRANSPARENT);

		DrawText(hdc, leftMenuToolsTip[leftMenuToolsTipIndex].c_str(), leftMenuToolsTip[leftMenuToolsTipIndex].size(), &rcClient, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

		SelectObject(hdc, old);

		EndPaint(hwndDlg, &ps);
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;

	}

	return FALSE;
}

DLGPROC GoToProcToolWelcome(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{


	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_PAINT:

	{
		PAINTSTRUCT ps;

		RECT rcClient;

		HDC hdc = BeginPaint(hwndDlg, &ps);

		GetClientRect(hwndDlg, &rcClient);

		BITMAPFILEHEADER   bmfHeader = { NULL };

		BITMAPINFOHEADER   bi = { NULL };

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = bmpWelcome;

		bi.biHeight = bmpHbmpWelcome;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		SetStretchBltMode(hdc, HALFTONE);

		
		if (GDI_ERROR == StretchDIBits(hdc, 0, 0, bi.biWidth, bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, this_toolsTipsWelcome, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}		

		EndPaint(hwndDlg, &ps);
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;

	}

	return FALSE;
}

DLGPROC GoToProcFrame(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static BOOL bMouseInside;

	TRACKMOUSEEVENT tme;

	static bool pressCursor = false;

	static POINT g_ptMousePos;

	switch (message)
	{
	

	case WM_INITDIALOG:	

		//SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);

		SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (LPARAM)wcex.hIcon);
		
		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hBrushFramrDlg;

#ifdef _NOTIFY_

	case WM_NOTIFY:
	{
		switch (LOWORD(wParam))
		{

		case IDC_TREEVIEW:
			
			if (((LPNMHDR)lParam)->code == NM_CLICK)
			{
				//LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)lParam;

				//TreeView_DeleteItem(((LPNMHDR)lParam)->hwndFrom, pCustomDraw->nmcd.dwItemSpec);

				// Вставляем вложенные элементы первого уровня
				//InsTreeItem(hRootItem, szBSPSeries,
					//(HTREEITEM)TVI_SORT, pCustomDraw->nmcd.dwItemSpec, pCustomDraw->nmcd.dwItemSpec);

				ShowWindow(hWndReg, SW_SHOW);
			}			
			// if code == NM_CLICK - Single click on an item
			if (((LPNMHDR)lParam)->code == NM_DBLCLK)
			{
				
				//MessageBox(NULL, L"No Items in TreeView",
					//L"Error", MB_OK | MB_ICONINFORMATION);
				
				/*char Text[255] = "";
				memset(&tvi, 0, sizeof(tvi));
				Selected = (HTREEITEM)SendDlgItemMessage(hWnd,
					IDC_TREE1, TVM_GETNEXTITEM, TVGN_CARET, (LPARAM)Selected);

				if (Selected == NULL)
				{
					MessageBox(hWnd, "No Items in TreeView",
						"Error", MB_OK | MB_ICONINFORMATION);
					break;
				}
				TreeView_EnsureVisible(hWnd, Selected);
				SendDlgItemMessage(hWnd, IDC_TREE1,
					TVM_SELECTITEM, TVGN_CARET, (LPARAM)Selected);
				flagSelected = true;
				tvi.mask = TVIF_TEXT;
				tvi.pszText = Text;
				tvi.cchTextMax = 256;
				tvi.hItem = Selected;

				if (SendDlgItemMessage(hWnd, IDC_TREE1,
					TVM_GETITEM, TVGN_CARET, (LPARAM)&tvi))
				{
					if (tvi.cChildren == 0 && strcmp(tvi.pszText, "Double Click Me!") == 0)
					{
						MessageBox(hWnd, "Press OK to delete me!",
							"Example", MB_OK | MB_ICONINFORMATION);
						SendDlgItemMessage(hWnd, IDC_TREE1,
							TVM_DELETEITEM, TVGN_CARET, (LPARAM)tvi.hItem);
						flagSelected = false;
						break;
					}
				}*/
			}

			if (((LPNMHDR)lParam)->code == NM_CUSTOMDRAW)
			{
				LPNMTVCUSTOMDRAW pCustomDraw = (LPNMTVCUSTOMDRAW)lParam;

				SetWindowLongPtr(hwndDlg, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);

				switch (pCustomDraw->nmcd.dwDrawStage)
				{
				
				case CDDS_PREPAINT:						

					return (DLGPROC)CDRF_NOTIFYITEMDRAW;

				case CDDS_ITEMPREPAINT | CDDS_SUBITEM:

					pCustomDraw->clrTextBk = RGB(30, 30, 30);

					if (pCustomDraw ->nmcd.uItemState & (CDIS_SELECTED | CDIS_FOCUS))
					{
						pCustomDraw->clrTextBk = RGB(190, 190, 190); // sel color			    
					}

					pCustomDraw->nmcd.uItemState &= ~(CDIS_FOCUS ^ CDIS_SELECTED); // kill sys sel					

					return (DLGPROC)CDRF_NEWFONT;

				default:

					pCustomDraw->clrTextBk = RGB(30, 30, 30);

					if (pCustomDraw->nmcd.uItemState & (CDIS_SELECTED | CDIS_FOCUS))
					{
						pCustomDraw->clrTextBk = RGB(190, 190, 190); // sel color			    
					}

					pCustomDraw->nmcd.uItemState &= ~(CDIS_FOCUS ^ CDIS_SELECTED); // kill sys sel	

					return (DLGPROC)CDRF_NOTIFYSUBITEMDRAW;

				}				

				return (DLGPROC)TRUE;// customdraw				
			}

			break;			
		}
	}

	break;

#endif

	case WM_MOUSEMOVE:

	{	
		
		if (pressCursor)
		{
			RECT rc;

			POINT ptCursor;

			POINT ptDelta;

			GetWindowRect(hWndDlgTmp, &rc);

			GetCursorPos(&ptCursor);

			ptDelta.x = g_ptMousePos.x - ptCursor.x;

			ptDelta.y = g_ptMousePos.y - ptCursor.y;

			MoveWindow(hWndDlgTmp, rc.left - ptDelta.x, rc.top - ptDelta.y,
				rc.right - rc.left, rc.bottom - rc.top, TRUE);

			MoveWindow(hwndDlg, (rc.left - ptDelta.x) - MARGIN_LEFT, (rc.top - ptDelta.y) - MARGIN_TOP,
				(rc.right - rc.left) + MARGIN_RIGHT, (rc.bottom - rc.top) + MARGIN_BOTTOM, TRUE);

			g_ptMousePos.x = ptCursor.x;

			g_ptMousePos.y = ptCursor.y;

			InvalidateRect(hwndDlg, NULL, false);

			UpdateWindow(hwndDlg);
		}
		
		RECT rcClient;

		GetClientRect(hwndDlg, &rcClient);

		int xPos = GET_X_LPARAM(lParam);

		int yPos = GET_Y_LPARAM(lParam);

		tme.cbSize = sizeof(tme);

		tme.hwndTrack = hwndFrame;

		tme.dwFlags = TME_LEAVE;

		_TrackMouseEvent(&tme);

		if (!bMouseInside)
		{
			bMouseInside = TRUE;			

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);
		}

		if (rcClient.right - headerButtonsW < xPos && xPos < rcClient.right - headerButtonsW / 2 && 5 < yPos && yPos < headerButtonsH)
		{
			SetCursor(hFinger);

			leftOver = true;

			righOver = false;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}

		if (rcClient.right - headerButtonsW / 2 < xPos && xPos < rcClient.right && 0 < yPos && yPos < headerButtonsH)
		{
			SetCursor(hFinger);

			righOver = true;

			leftOver = false;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}

		/*****************************************************************************************************************************************LEFT_MENU*/

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::ONE_ITEM)+8) < yPos && yPos < ((vITEMLM::ONE_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::ONE_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 0;	

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);
			
			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else 
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TWO_ITEM)+8) < yPos && yPos < ((vITEMLM::TWO_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::TWO_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 1;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::THREE_ITEM)+8) < yPos && yPos < ((vITEMLM::THREE_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::THREE_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 2;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FOUR_ITEM)+8) < yPos && yPos < ((vITEMLM::FOUR_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::FOUR_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 3;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FIVE_ITEM)+8) < yPos && yPos < ((vITEMLM::FIVE_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::FIVE_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 4;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::SIX_ITEM)+8) < yPos && yPos < ((vITEMLM::SIX_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::SIX_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 5;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::SEVEN_ITEM)+8) < yPos && yPos < ((vITEMLM::SEVEN_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::SEVEN_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 6;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::EIGHT_ITEM)+8) < yPos && yPos < ((vITEMLM::EIGHT_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::EIGHT_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 7;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::NINE_ITEM)+8) < yPos && yPos < ((vITEMLM::NINE_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::NINE_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 8;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TEN_ITEM)+8) < yPos && yPos < ((vITEMLM::TEN_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::TEN_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 9;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::ELEVEN_ITEM)+8) < yPos && yPos < ((vITEMLM::ELEVEN_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::ELEVEN_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 10;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TWELVE_ITEM)+8) < yPos && yPos < ((vITEMLM::TWELVE_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::TWELVE_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 11;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::THIRTEEN_ITEM)+8) < yPos && yPos < ((vITEMLM::THIRTEEN_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::THIRTEEN_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 12;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FOURTEEN_ITEM)+8) < yPos && yPos < ((vITEMLM::FOURTEEN_ITEM)+8) + 22)
		{
			yTopPanel_tools = (vITEMLM::FOURTEEN_ITEM)+8;

			yBottomPanel_tools = yTopPanel_tools + 22;

			SetCursor(hFinger);

			buf_left_p = true;

			/****************************************************************************************************/

			leftMenuToolsTipIndex = 13;

			POINT pt;

			pt.x = xPos;

			pt.y = yPos;

			ClientToScreen(hwndFrame, &pt);

			SetWindowPos(hwndTip, HWND_TOPMOST, pt.x + 30, pt.y, leftMenuToolsTip[leftMenuToolsTipIndex].length() * 8, 40, SWP_SHOWWINDOW);

			InvalidateRect(hwndTip, NULL, true);

			UpdateWindow(hwndTip);

			/****************************************************************************************************/

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (rcClient.right / 2 - szStr2.cx / 2 < xPos && xPos < rcClient.right / 2 + szStr2.cx / 2 && rcClient.bottom - szStr2.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr2.cy * 0.7 && !enter_tools_panels)
		{
			SetCursor(hFinger);

			textGNUProgram = true;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if (60 < xPos && xPos < (szStr0.cx + 60) && rcClient.bottom - szStr0.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr0.cy * 0.7 && !enter_tools_panels)
		{
			SetCursor(hFinger);

			textReadyProgram = true;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		if ((rcClient.right - setting_buffer_w) < xPos && xPos < ((rcClient.right - setting_buffer_w) + szStr1.cx) && rcClient.bottom - szStr0.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr0.cy * 0.7 && !enter_tools_panels)
		{
			SetCursor(hFinger);

			textSettingProgram = true;

			InvalidateRect(hwndFrame, NULL, false);

			UpdateWindow(hwndFrame);

			break;
		}
		else
		{
			ShowWindow(hwndTip, SW_HIDE);
		}

		/*****************************************************************************************************************************************/

		//--------for processor and sound burron*****************************

		if (enter_tools_panels)
		{
			if (left_panel_toolsBG_X + 50 < xPos && xPos < left_panel_toolsBG_X + 50 + pointImageButton_X && positionBottom < yPos && yPos < positionBottom + pointImageButton_Y)
			{
				SetCursor(hFinger);

				break;
			}

			/********************************************************************CLOUD*********************************************************************/

			if (cloud_panel_w < xPos && xPos < cloud_panel_w + cloud_buffer_w && cloud_panel_h < yPos && yPos < cloud_panel_h + cloud_buffer_h)
			{
				SetCursor(hFinger);

				break;
			}
		}

		/*******************************************************************CLOUDEND**********************************************************************/

		leftOver = false;

		righOver = false;

		buf_left_p = false;

		textReadyProgram = false;

		textSettingProgram = false;

		textGNUProgram = false;

		InvalidateRect(hwndFrame, NULL, false);

		UpdateWindow(hwndFrame);

	}
	     break;

	case WM_SYSCOMMAND:

		if (wParam == SC_RESTORE)
		{
			ShowWindow(hwndDlg, SW_SHOWNORMAL);

			/*
			   Важен порядок
			
			*/

			SendMessage(hwndFrame, WM_LBUTTONUP, NULL, NULL);// 1
			
			SendMessage(hwndFrame, WM_MOUSEMOVE, NULL, NULL);// 2

			Indicate_Release(hwndFrame);//

			ResumeThread(hThreadsWork);
			
			return DLGPROC(TRUE);
		}

		break;

	case WM_LBUTTONDOWN:

	{
		RECT rcClient;

		GetClientRect(hwndDlg, &rcClient);

		int xPos = GET_X_LPARAM(lParam);

		int yPos = GET_Y_LPARAM(lParam);

		if (!pressCursor)
		{
			SetCapture(hwndDlg);

			pressCursor = true;

			GetCursorPos(&g_ptMousePos);
		}

		if (rcClient.right - headerButtonsW < xPos && xPos < rcClient.right - headerButtonsW / 2 && 5 < yPos && yPos < headerButtonsH)
		{
			ShowWindow(hwndDlg, SW_MINIMIZE);

			SuspendThread(hThreadsWork);
			
			break;
		}

		if (rcClient.right - headerButtonsW / 2 < xPos && xPos < rcClient.right && 0 < yPos && yPos < headerButtonsH)
		{
			//ShellExecute(NULL, L"open", L"https://burundukpro.ru/pay-system", NULL, NULL, SW_SHOWNORMAL);

			ExitProcess(0);
			
			break;
		}

		/*****************************************************************************************************************************************LEFT_MENU*/

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::ONE_ITEM)+8) < yPos && yPos < ((vITEMLM::ONE_ITEM)+8) + 22)
		{			
			
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TWO_ITEM)+8) < yPos && yPos < ((vITEMLM::TWO_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::THREE_ITEM)+8) < yPos && yPos < ((vITEMLM::THREE_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FOUR_ITEM)+8) < yPos && yPos < ((vITEMLM::FOUR_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FIVE_ITEM)+8) < yPos && yPos < ((vITEMLM::FIVE_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::SIX_ITEM)+8) < yPos && yPos < ((vITEMLM::SIX_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::SEVEN_ITEM)+8) < yPos && yPos < ((vITEMLM::SEVEN_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::EIGHT_ITEM)+8) < yPos && yPos < ((vITEMLM::EIGHT_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::NINE_ITEM)+8) < yPos && yPos < ((vITEMLM::NINE_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TEN_ITEM)+8) < yPos && yPos < ((vITEMLM::TEN_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::ELEVEN_ITEM)+8) < yPos && yPos < ((vITEMLM::ELEVEN_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::TWELVE_ITEM)+8) < yPos && yPos < ((vITEMLM::TWELVE_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::THIRTEEN_ITEM)+8) < yPos && yPos < ((vITEMLM::THIRTEEN_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (rcClient.right / 2 - szStr2.cx / 2 < xPos && xPos < rcClient.right / 2 + szStr2.cx / 2 && rcClient.bottom - szStr2.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr2.cy * 0.7 && !enter_tools_panels)//клик для лицухи
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (5 < xPos && xPos < left_panel_tools_X && ((vITEMLM::FOURTEEN_ITEM)+8) < yPos && yPos < ((vITEMLM::FOURTEEN_ITEM)+8) + 22)
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			if (background_alternative == 0 && !enter_tools_panels)
			{
				enter_tools_panels = true;

				InvalidateRect(hwndFrame, NULL, true);

				UpdateWindow(hwndFrame);

				ShowWindow(hwndTree, SW_SHOW);

				ShowWindow(hWndDlgTmp, SW_SHOW);

				background_alternative++;
			}
			else 
			{
				enter_tools_panels = false;
				
				ShowWindow(hwndTree, SW_HIDE);

				ShowWindow(hWndDlgTmp, SW_HIDE);

				background_alternative = 0;
			}

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		if (60 < xPos && xPos < (szStr0.cx + 60) && rcClient.bottom - szStr0.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr0.cy * 0.7 && !enter_tools_panels)//клик для версии
		{
			ShowWindow(hwndVersion, SW_SHOW);
		}

		if ((rcClient.right - setting_buffer_w) < xPos && xPos < ((rcClient.right - setting_buffer_w) + szStr1.cx) && rcClient.bottom - szStr0.cy * 1.4 < yPos && yPos < rcClient.bottom - szStr0.cy * 0.7 && !enter_tools_panels)//клик для сеттинга
		{
			if (!enter_tools_panels && background_alternative == -1) {

				//CreateThread(NULL, 0, &ThreadProcWelcome, NULL, 0, NULL);

				background_alternative = 0;
			}

			enter_tools_panels = true;

			InvalidateRect(hwndFrame, NULL, true);

			UpdateWindow(hwndFrame);

			ShowWindow(hwndTree, SW_SHOW);

			ShowWindow(hWndDlgTmp, SW_SHOW);

			if (ext)
			{
				ShowWindow(hWndReg, SW_SHOW);

				return 0;
			}

			break;
		}

		/*****************************************************************************************************************************************/

		//--------for processor and sound burron*****************************

		if (enter_tools_panels) {

			if (left_panel_toolsBG_X + 50 < xPos && xPos < left_panel_toolsBG_X + 50 + pointImageButton_X && positionBottom < yPos && yPos < positionBottom + pointImageButton_Y)
			{
				ShowWindow(hWndReg, SW_SHOW);

				break;
			}

			/********************************************************************CLOUD*********************************************************************/

			if (cloud_panel_w < xPos && xPos < cloud_panel_w + cloud_buffer_w && cloud_panel_h < yPos && yPos < cloud_panel_h + cloud_buffer_h)
			{
				ShowWindow(hWndReg, SW_SHOW);

				break;
			}
		}

		/*******************************************************************CLOUDEND**********************************************************************/

	}

	break;

	case WM_LBUTTONUP:

	{
		if (pressCursor)
		{
			ReleaseCapture();

			pressCursor = false;
		}
	}

	break;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_CLOSE:

	{
		ExitProcess(0);
	}

	break;	

	case WM_MOUSELEAVE:

	{    bMouseInside = FALSE;

	     leftOver = false;

	     righOver = false;

		 textReadyProgram = false;

		 textSettingProgram = false;

		 textGNUProgram = false;

		 ShowWindow(hwndTip, SW_HIDE);

		 InvalidateRect(hwndFrame, NULL, false);

		 UpdateWindow(hwndFrame);
	}

		break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;	

	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hwndDlg, &ps);				

		RECT rcClient;		

		GetClientRect(hwndDlg, &rcClient);

		BITMAPFILEHEADER   bmfHeader = { NULL };

		BITMAPINFOHEADER   bi = { NULL };

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = headerW;

		bi.biHeight = headerH;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		SetStretchBltMode(hdc, HALFTONE);

		if (GDI_ERROR == StretchDIBits(hdc, (rcClient.right / 2) - (bi.biWidth / 2), 0, (bi.biWidth), bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, this_Header, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}		

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = headerButtonsW;

		bi.biHeight = headerButtonsH;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;		

		if (GDI_ERROR == StretchDIBits(hdc, (rcClient.right) - (bi.biWidth), 0, (bi.biWidth), bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, this_HeaderButtons, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = headerButtonsLeftCaptureW;

		bi.biHeight = headerButtonsLeftCaptureH;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;


		if (GDI_ERROR == StretchDIBits(hdc, 5, 5, (bi.biWidth), bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, this_HeaderButtonsTemps, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}

		/***************************************************************************************************************************************/

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = left_panel_tools_X;

		bi.biHeight = left_panel_tools_Y;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		if (GDI_ERROR == StretchDIBits(hdc, 5, MARGIN_TOP, (bi.biWidth), bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, pointLPT, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = left_panel_toolsBG_X;

		bi.biHeight = left_panel_toolsBG_Y;//rcClient.bottom - rcClient.top;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		if (GDI_ERROR == StretchDIBits(hdc, 5, left_panel_tools_Y + MARGIN_TOP, (bi.biWidth), (rcClient.bottom - (left_panel_tools_Y + MARGIN_TOP)) - 3, 0, 0, bi.biWidth, bi.biHeight, pointLPTBG, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}

		/**************************************************************CLOUD*****************************************************************************/

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = cloud_buffer_w;

		bi.biHeight = cloud_buffer_h;//rcClient.bottom - rcClient.top;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		if (enter_tools_panels) 
		{
			if (GDI_ERROR == StretchDIBits(hdc, cloud_panel_w, cloud_panel_h, bi.biWidth, bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, cloud_buffer, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}
		}
		else 
		{

			RECT rcClient;

			GetClientRect(hwndDlg, &rcClient);

			BITMAPFILEHEADER   bmfHeader = { NULL };

			BITMAPINFOHEADER   bi = { NULL };

			bi.biSize = sizeof(BITMAPINFOHEADER);

			bi.biWidth = carbon_buffer_w;

			bi.biHeight = carbon_buffer_h;

			bi.biPlanes = 1;

			bi.biBitCount = 32;

			bi.biCompression = BI_RGB;

			bi.biSizeImage = 0;

			bi.biXPelsPerMeter = 0;

			bi.biYPelsPerMeter = 0;

			bi.biClrUsed = 0;

			bi.biClrImportant = 0;

			SetStretchBltMode(hdc, HALFTONE);

			if (GDI_ERROR == StretchDIBits(hdc, 37, 35, rcClient.right - 39, rcClient.bottom - 37, 0, 0, bi.biWidth, bi.biHeight, carbon_buffer, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}

			if(textReadyProgram)
			{
				SetTextColor(hdc, RGB(180, 180, 180));
			}
			else 
			{
				SetTextColor(hdc, RGB(255, 255, 255));
			}			

			SetBkMode(hdc, TRANSPARENT);

			SelectObject(hdc, hFont);

			TextOut(hdc, 60, (rcClient.bottom) - szStr0.cy * 1.4, STRING_LEFT, wcslen(STRING_LEFT));

			GetTextExtentPoint32(hdc, STRING_LEFT, wcslen(STRING_LEFT), &szStr0);

			/************************************************************************************************************************************************/

			bi.biSize = sizeof(BITMAPINFOHEADER);

			bi.biWidth = setting_buffer_w;

			bi.biHeight = setting_buffer_h;

			bi.biPlanes = 1;

			bi.biBitCount = 32;

			bi.biCompression = BI_RGB;

			bi.biSizeImage = 0;

			bi.biXPelsPerMeter = 0;

			bi.biYPelsPerMeter = 0;

			bi.biClrUsed = 0;

			bi.biClrImportant = 0;

			SetStretchBltMode(hdc, HALFTONE);

			if (GDI_ERROR == StretchDIBits(hdc, rcClient.right - bi.biWidth - 25, (rcClient.bottom) - szStr0.cy * 1.4, setting_buffer_w, setting_buffer_h, 0, 0, bi.biWidth, bi.biHeight, pointSettingBuffer, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}

			if (textSettingProgram)
			{
				SetTextColor(hdc, RGB(180, 180, 180));
			}
			else
			{
				SetTextColor(hdc, RGB(255, 255, 255));
			}

			TextOut(hdc, rcClient.right - setting_buffer_w, (rcClient.bottom) - szStr0.cy * 1.4, STRING_RIGHT, wcslen(STRING_RIGHT));

			GetTextExtentPoint32(hdc, STRING_RIGHT, wcslen(STRING_RIGHT), &szStr1);

			/********************************************************************Start Hader Piramide**********************************************************************************/

			bi.biSize = sizeof(BITMAPINFOHEADER);

			bi.biWidth = headernewinterfeace_w;

			bi.biHeight = headernewinterfeace_h;

			bi.biPlanes = 1;

			bi.biBitCount = 32;

			bi.biCompression = BI_RGB;

			bi.biSizeImage = 0;

			bi.biXPelsPerMeter = 0;

			bi.biYPelsPerMeter = 0;

			bi.biClrUsed = 0;

			bi.biClrImportant = 0;

			if (GDI_ERROR == StretchDIBits(hdc, (rcClient.right / 2) - (bi.biWidth / 2), rcClient.bottom - bi.biHeight, (bi.biWidth), bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, pointheadernewinterfeace, (BITMAPINFO*)&bi, 0, SRCCOPY))
			{
				MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
			}

			if (textGNUProgram)
			{
				SetTextColor(hdc, RGB(180, 180, 180));
			}
			else
			{
				SetTextColor(hdc, RGB(255, 255, 255));
			}

			GetTextExtentPoint32(hdc, STRING_CENTER, wcslen(STRING_CENTER), &szStr2);

			TextOut(hdc, rcClient.right / 2 - szStr2.cx / 2, (rcClient.bottom) - szStr2.cy * 1.4, STRING_CENTER, wcslen(STRING_CENTER));

			/********************************************************************End Bottom Piramide**********************************************************************************/
		}

		/**************************************************************CLOUDEND*****************************************************************************/

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = bottom_panel_indicateX;

		bi.biHeight = bottom_panel_indicateY;//rcClient.bottom - rcClient.top;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		// create memory DC and memory bitmap where we shall do our drawing

		HDC memDC = CreateCompatibleDC(hdc);

		RECT rcClientRect, updateRect;

		GetClientRect(hwndDlg, &rcClientRect);

		GetClientRect(hWndDlgTmp, &updateRect);

		//rcClientRect.right = updateRect.right;

		// now we can create bitmap where we shall do our drawing
		HBITMAP bmp = CreateCompatibleBitmap(hdc,
			rcClientRect.right - rcClientRect.left,
			rcClientRect.bottom - rcClientRect.top);

		// we need to save original bitmap, and select it back when we are done,
		// in order to avoid GDI leaks!
		HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

		if (GDI_ERROR == StretchDIBits(memDC, left_panel_toolsBG_X + 5, rcClient.bottom - bi.biHeight, rcClient.right - 40, bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, pointBottomIndicate, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}

		int xSours = left_panel_toolsBG_X + 5;

		int ySours = rcClient.bottom - bi.biHeight;

		if (queueStart) {

			HGDIOBJ old = SelectObject(memDC, hPen);

			bool insert = false;

			while (!qBottomIndicate.empty()) {

				int start = qBottomIndicate.front();

				if ((left_panel_toolsBG_X + 5) < ((start) - nCountIndicate))
				{
					DrawLine(memDC, (start)-nCountIndicate, (rcClient.bottom - 5), (start)-nCountIndicate, (rcClient.bottom - bi.biHeight));
					
				}
				else 
				{
					insert = true;
				}

				qBottomIndicate.pop();
			}

			qBottomIndicate = qBottomIndicateTemp;

			if (insert) {

				int end = qBottomIndicate.back() + 20;

				qBottomIndicate.pop();

				qBottomIndicate.push(end);

				qBottomIndicateTemp = qBottomIndicate;

				nCountIndicateRevers = 0;

				DrawLine(memDC, qBottomIndicate.back(), (rcClient.bottom - 5), qBottomIndicate.back(), (rcClient.bottom - bi.biHeight));
			}

			for (int j = 1; j < 40; j += 4)
			/*зеленая линия гаризонт*/
			DrawLine(memDC, left_panel_toolsBG_X + 10, (rcClient.bottom - 5) - (bi.biHeight - j * 5), rcClient.right - 10, ((rcClient.bottom - 5) - (bi.biHeight - j * 5)));

			SelectObject(memDC, old);
		}		

		{//желтая линия, горизонт

			HGDIOBJ old = SelectObject(memDC, hPenGold);

			DrawLine(memDC, left_panel_toolsBG_X + 10, (rcClient.bottom - 5) - bi.biHeight / 2, rcClient.right - 10, ((rcClient.bottom - 5) - bi.biHeight / 2));

			SelectObject(memDC, old);
		}

		positionBottom = (rcClient.bottom - 5) - bi.biHeight;

		bi.biSize = sizeof(BITMAPINFOHEADER);

		bi.biWidth = pointImageButton_X;

		bi.biHeight = pointImageButton_Y;//rcClient.bottom - rcClient.top;

		bi.biPlanes = 1;

		bi.biBitCount = 32;

		bi.biCompression = BI_RGB;

		bi.biSizeImage = 0;

		bi.biXPelsPerMeter = 0;

		bi.biYPelsPerMeter = 0;

		bi.biClrUsed = 0;

		bi.biClrImportant = 0;

		if (GDI_ERROR == StretchDIBits(memDC, left_panel_toolsBG_X + 50, positionBottom, bi.biWidth, bi.biHeight, 0, 0, bi.biWidth, bi.biHeight, pointImageButton, (BITMAPINFO*)&bi, 0, SRCCOPY))
		{
			MessageBox(hwndDlg, L"Ошибка [GDI_ERROR == StretchDIBits] ", L"Внимание!", MB_TOPMOST);
		}		
		
		if (enter_tools_panels) 
		{
			BitBlt(hdc, xSours, ySours, bottom_panel_indicateX * scaleX,
				bottom_panel_indicateY, memDC, xSours, ySours, SRCCOPY);
		}

		// all done, now we need to cleanup
		SelectObject(memDC, oldBmp); // select back original bitmap

		DeleteObject(bmp); // delete bitmap since it is no longer required

		DeleteDC(memDC);   // delete memory DC since it is no longer required

		/***************************************************************************************************************************************/

		if (leftOver || righOver) {

			if (leftOver) {

				RECT dim;

				dim.left = rcClient.right - headerButtonsW;

				dim.right = rcClient.right - headerButtonsW / 2;

				dim.top = 0;

				dim.bottom = headerButtonsH;

				paintRect(hdc, dim, RGB(90, 90, 90), RGB(120, 120, 120), 5);
			}

			if (righOver) {

				RECT dim;

				dim.left = rcClient.right - headerButtonsW / 2;

				dim.right = rcClient.right;

				dim.top = 0;

				dim.bottom = headerButtonsH;

				paintRect(hdc, dim, RGB(90, 90, 90), RGB(120, 120, 120), 5);
			}

		}

		if (buf_left_p) {

			RECT dim;

			dim.left = 3;

			dim.right = left_panel_tools_X;

			dim.top = yTopPanel_tools;

			dim.bottom = yBottomPanel_tools;

			paintRectLeftPanelTools(hdc, dim, RGB(90, 90, 90), RGB(120, 120, 120), 5);
		}

		EndPaint(hwndDlg, &ps);
	}

	break;

	case WM_COMMAND:

		switch (LOWORD(wParam))
		{		

		case DELAY_TOP_MSG_UPDATE_INDICATION:

		{
			Indicate_Update(hwndFrame);
		}
			break;

		case DELAY_TOP_MSG:

		{
			RECT rc;

			POINT ptCursor;

			POINT ptDelta;

			GetWindowRect(hWndDlgTmp, &rc);

			GetCursorPos(&ptCursor);

			ptDelta.x = (g_ptMousePos.x - ptCursor.x) * 0;

			ptDelta.y = (g_ptMousePos.y - ptCursor.y) * 0 - 40;

			MoveWindow(hWndDlgTmp, rc.left - ptDelta.x, rc.top - ptDelta.y,
				rc.right - rc.left, rc.bottom - rc.top, TRUE);

			MoveWindow(hwndDlg, (rc.left - ptDelta.x) - MARGIN_LEFT, (rc.top - ptDelta.y) - MARGIN_TOP,
				(rc.right - rc.left) + MARGIN_RIGHT, (rc.bottom - rc.top) + MARGIN_BOTTOM, TRUE);

			g_ptMousePos.x = ptCursor.x;

			g_ptMousePos.y = ptCursor.y;
		}

		break;		

		}

	}

	return FALSE;
}

LRESULT CALLBACK WndProcTreeWnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	switch (message)
	{	
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);

		

		EndPaint(hWnd, &ps);
	}

	break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

DLGPROC GoToProcVersion(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH defaultbrush = NULL;

	static HBRUSH GroupBoxBrush = NULL;

	static HBRUSH hotbrush = NULL;

	static HBRUSH selectbrush = NULL;

	switch (message)
	{

	case WM_NOTIFY:


	{//start

		LPNMHDR some_item = (LPNMHDR)lParam;

		if ((some_item->idFrom == IDOK || some_item->idFrom == IDCANCEL || some_item->idFrom == IDCANCEL3) && some_item->code == NM_CUSTOMDRAW)
		{
			LPNMCUSTOMDRAW item = (LPNMCUSTOMDRAW)some_item;

			if (item->uItemState & CDIS_SELECTED)
			{
				//Select our color when the button is selected

				if (selectbrush == NULL)
				{
					selectbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
				}

				//Create pen for button border
				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(166, 166, 166));

				//Select our brush into hDC
				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, selectbrush);

				//If you want rounded button, then use this, otherwise use FillRect().
				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				//Clean up
				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				//Now, I don't want to do anything else myself (draw text) so I use this value for return:
				return CDRF_DODEFAULT;
				//Let say I wanted to draw text and stuff, then I would have to do it before return with
				//DrawText() or other function and return CDRF_SKIPDEFAULT
			}
			else
			{
				if (item->uItemState & CDIS_HOT) //Our mouse is over the button
				{
					//Select our color when the mouse hovers our button

					if (hotbrush == NULL)
					{
						hotbrush = CreateGradientBrush(RGB(21, 89, 106), RGB(34, 138, 164), item);
					}

					HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(166, 166, 166));

					HGDIOBJ old_pen = SelectObject(item->hdc, pen);

					HGDIOBJ old_brush = SelectObject(item->hdc, hotbrush);

					RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

					SelectObject(item->hdc, old_pen);

					SelectObject(item->hdc, old_brush);

					DeleteObject(pen);

					return CDRF_DODEFAULT;
				}

				//Select our color when our button is doing nothing

				if (defaultbrush == NULL)
				{
					defaultbrush = CreateGradientBrush(RGB(40, 165, 196), RGB(102, 199, 223), item);
				}

				HPEN pen = CreatePen(PS_INSIDEFRAME, 0, RGB(66, 66, 66));

				HGDIOBJ old_pen = SelectObject(item->hdc, pen);

				HGDIOBJ old_brush = SelectObject(item->hdc, defaultbrush);

				RoundRect(item->hdc, item->rc.left, item->rc.top, item->rc.right, item->rc.bottom, 3, 3);

				SelectObject(item->hdc, old_pen);

				SelectObject(item->hdc, old_brush);

				DeleteObject(pen);

				return CDRF_DODEFAULT;
			}
		}

		return CDRF_DODEFAULT;

	}//end	

	break;

	case WM_INITDIALOG:

		SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (LPARAM)wcex.hIcon);

		break;

	case WM_CTLCOLORDLG:

		return (DLGPROC)hbrushForDialog;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;

		SetTextColor(hdcStatic, TEXT_COLOR);

		SetBkMode(hdcStatic, TRANSPARENT);

		return (DLGPROC)hbrushForDialog;
	}

	break;

	case WM_DESTROY:

	{
		ShowWindow(hwndVersion, SW_HIDE);
	}

	break;

	case WM_CTLCOLORBTN:
		return (DLGPROC)hbrushForDialog;


	case WM_COMMAND:
		switch (LOWORD(wParam))
		{

		case IDOK:

		{
			ShowWindow(hwndVersion, SW_HIDE);
		}

		break;

		case IDCANCEL3:

		{

		}

		break;

		case IDCANCEL:

		{
			ShowWindow(hwndVersion, SW_HIDE);
		}

		break;
		}
	}

	return FALSE;
}
#pragma once

bool startworklist = false;

bool statedbgClick = false;

HIMAGELIST hSmallImgBigList;

namespace list {

		long start = 0;
};

static bool InitListViewColumnsDlgBox(HWND hWndListView, int w)
{
	LVCOLUMN lvc;

	int iCol;

	// Initialize the LVCOLUMN structure.
	// The mask specifies that the format, width, text,
	// and subitem members of the structure are valid.
	lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;

	vector<wstring> str = { L"", L"Сущность",L"Координаты" };

	// Add the columns.
	for (iCol = 0; iCol < (int)str.size(); iCol++)
	{
		lvc.iSubItem = iCol;

		lvc.pszText = (LPWSTR)str[iCol].c_str();

		if (iCol == 0)
			lvc.cx = 20;               // Width of column in pixels.
		else lvc.cx = w;

		if (iCol < 2)
			lvc.fmt = LVCFMT_LEFT;  // Left-aligned column.
		else
			lvc.fmt = LVCFMT_RIGHT; // Right-aligned column.	

		// Insert the columns into the list view.
		if (ListView_DeleteColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;

		// Insert the columns into the list view.
		if (ListView_InsertColumn(hWndListView, iCol, &lvc) == -1)
			return FALSE;
	}
}

static bool InsertListViewItemsDlgBox(HWND hWndListView, int index, const wstring &str, const wstring& cor)
{	

	LVITEM lvI = { 0 };

	SHFILEINFO lp = { 0 };		

	//присваеваем иконки 
	wchar_t buf1[MAX_PATH];

	memset(buf1, 0, sizeof(wchar_t) * MAX_PATH);

	GetWindowsDirectory(buf1, MAX_PATH);

	DWORD num = GetFileAttributes(buf1);

	SHGetFileInfo(buf1, num, &lp, sizeof(lp), SHGFI_ICONLOCATION | SHGFI_ICON | SHGFI_SMALLICON);

	{
		lvI.iItem = index;

		lvI.iImage = index;

		lvI.mask = LVIF_TEXT | LVIF_STATE | LVIF_IMAGE;

		lvI.stateMask = 0;

		lvI.iSubItem = 0;

		lvI.state = 0;

		// Insert items into the list.
		if (ListView_InsertItem(hWndListView, &lvI) == -1)
			return FALSE;

		lvI.pszText = (LPWSTR)" "; // Sends an LVN_GETDISPINFO message.		

		ListView_SetItemText(hWndListView, lvI.iItem, 1, (LPWSTR)str.c_str());

		ListView_SetItemText(hWndListView, lvI.iItem, 2, (LPWSTR)cor.c_str());

		ImageList_AddIcon(hSmallImgBigList, lp.hIcon);

	}

	DestroyIcon(lp.hIcon);

	ListView_SetImageList(hWndListView, hSmallImgBigList, LVSIL_SMALL);

	return TRUE;

}
/* Written by Jason Heddings - http://www.heddway.com/
**
** This software is provided 'as-is', without any express or implied warranty.
** In no event will the authors be held liable for any damages arising from
** the use of this software. 
**
** License: http://creativecommons.org/licenses/by-nc-sa/2.0/
** Copyright (c) 2005, Heddway
*/

#include "ml_stats.h"

///////////////////////////////////////////////////////////////////////////////

extern HWND config_tab_init(HWND tab, bool showProcessing);

///////////////////////////////////////////////////////////////////////////////

void setHeaderVersion(HWND hwndDlg) {
    memset(tmp, 0, sizeof(tmp));
    GetDlgItemText(hwndDlg, IDC_MLHDR_VER, tmp, sizeof(tmp));
    char version[10] = { 0 };
    sprintf(version, "0x%x", MLHDR_VER);
    strreplace(tmp, "%VERSION%", version);
    SetDlgItemText(hwndDlg, IDC_MLHDR_VER, tmp);
}

///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK internal_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
        case WM_INITDIALOG:
            config_tab_init(hwndDlg, false);
            setHeaderVersion(hwndDlg);
			break;
    }

    return 0;
}
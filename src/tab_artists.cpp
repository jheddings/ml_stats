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
extern void setCounter(HWND hwndDlg, int res, int count);
extern void setAverage(HWND hwndDlg, int res, int count, int total);

///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK artists_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HWND dlg;
    switch (uMsg) {
        case WM_INITDIALOG:
            dlg = config_tab_init(hwndDlg, true);
            DestroyWindow(dlg);
			break;
    }

    return 0;
}
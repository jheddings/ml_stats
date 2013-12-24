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

HWND m_hwndDlg,m_hwndTab,m_hwndTabDisplay;

///////////////////////////////////////////////////////////////////////////////

extern INT_PTR CALLBACK basic_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern INT_PTR CALLBACK albums_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern INT_PTR CALLBACK artists_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern INT_PTR CALLBACK internal_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK process_dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

struct pref_tab {
  char* title;
  int res_id;
  DLGPROC dlg_proc;
} tabs[] = {
  {"Basic", IDD_STATS_BASIC, basic_tabproc},
  {"Artists", IDD_STATS_ARTISTS, artists_tabproc},
  {"Albums", IDD_STATS_ALBUMS, albums_tabproc},
#ifdef _DEBUG
  {"Internal", IDD_STATS_INTERNAL, internal_tabproc},
#endif
};

///////////////////////////////////////////////////////////////////////////////

#define ETDT_DISABLE 0x01
#define ETDT_ENABLE 0x02
#define ETDT_ENABLETAB 0x06
#define ETDT_USETABTEXTURE 0x04
typedef HRESULT (WINAPI * ENABLETHEMEDIALOGTEXTURE)(HWND, DWORD);

HWND config_tab_init(HWND tab, bool showProcessing = false) {
    HWND procdlg = 0;
    if (showProcessing) {
        procdlg = CreateDialog(plugin.hDllInstance,
                               MAKEINTRESOURCE(IDD_PROCESSING),
                               tab, process_dlgproc);
    }

    RECT r;
    GetWindowRect(m_hwndTab, &r);
    TabCtrl_AdjustRect(m_hwndTab, FALSE, &r);
    MapWindowPoints(NULL, m_hwndDlg, (LPPOINT) &r, 2);
    SetWindowPos(tab, HWND_TOP,
                 r.left, r.top,
                 r.right - r.left, r.bottom - r.top,
                 SWP_NOACTIVATE);

    HINSTANCE hDll;
    ENABLETHEMEDIALOGTEXTURE pfnETDT;
    if (NULL != (hDll = LoadLibrary(TEXT("uxtheme.dll")))) {
        pfnETDT = (ENABLETHEMEDIALOGTEXTURE)GetProcAddress(hDll, "EnableThemeDialogTexture");
        if (NULL != pfnETDT) {
            pfnETDT(tab, ETDT_ENABLETAB);
        }
        FreeLibrary(hDll);
    }

    return procdlg;
}

///////////////////////////////////////////////////////////////////////////////

VOID WINAPI OnSelChanged(HWND hwndDlg) {
    int sel = TabCtrl_GetCurSel(m_hwndTab);
    if (m_hwndTabDisplay != NULL) {
        DestroyWindow(m_hwndTabDisplay);
    }
    m_hwndTabDisplay = CreateDialog(plugin.hDllInstance,
                                    MAKEINTRESOURCE(tabs[sel].res_id),
                                    hwndDlg, tabs[sel].dlg_proc);
}

///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK stats_dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HWND dlg;
    LPNMHDR lpn = (LPNMHDR) lParam; 
    switch (uMsg) {
        case WM_INITDIALOG:
            dlg = CreateDialog(plugin.hDllInstance,
                               MAKEINTRESOURCE(IDD_PROCESSING),
                               hwndDlg, process_dlgproc);

            memset(tmp, 0, sizeof(tmp));
            GetPrivateProfileString("ml_stats", "query", "type = \"0\"",
                                    tmp, sizeof(tmp), iniFile);
            SetDlgItemText(hwndDlg, IDC_QUERY, tmp);
            m_pRecords = getRecords(tmp);

            TCITEM tie;
            m_hwndDlg = hwndDlg;
            m_hwndTab = GetDlgItem(hwndDlg, IDC_STATS_TABS);
            tie.mask = TCIF_TEXT;
            for (int i = 0; i < sizeof(tabs) / sizeof(pref_tab); i++) {
                tie.pszText = tabs[i].title;
                TabCtrl_InsertItem(m_hwndTab, i, &tie);
            }

            DestroyWindow(dlg);
            TabCtrl_SetCurSel(m_hwndTab, 0);
            OnSelChanged(hwndDlg);
            break;

        case WM_NOTIFY:
            if(lpn) if(lpn->code==TCN_SELCHANGE) OnSelChanged(hwndDlg);
            break;

		case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDC_ABOUT:
                    LoadString(plugin.hDllInstance, IDS_ABOUTBOX, tmp, sizeof(tmp));
                    MessageBox(hwndDlg, tmp, "Media Library Stastics",
                               MB_OK | MB_ICONINFORMATION);
                    break;

                case IDC_FILTER:
                    dlg = CreateDialog(plugin.hDllInstance,
                                       MAKEINTRESOURCE(IDD_PROCESSING),
                                       hwndDlg, process_dlgproc);
                    memset(tmp, 0, sizeof(tmp));
                    GetDlgItemText(hwndDlg, IDC_QUERY, tmp, sizeof(tmp));
                    m_pRecords = getRecords(tmp);
                    DestroyWindow(dlg);
                    OnSelChanged(hwndDlg);
                    break;

               case IDOK:
                    memset(tmp, 0, sizeof(tmp));
                    GetDlgItemText(hwndDlg, IDC_QUERY, tmp, sizeof(tmp));
                    WritePrivateProfileString("ml_stats", "query", tmp, iniFile);
                case IDCANCEL:
                    EndDialog(hwndDlg, 0);
					break;
            }
            break;
    }

    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void setCounter(HWND hwndDlg, int res, int count) {
    memset(tmp, 0, sizeof(tmp));
    GetDlgItemText(hwndDlg, res, tmp, sizeof(tmp));
    char counter[10] = { 0 };
    sprintf(counter, "%d", count);
    strreplace(tmp, "%COUNT%", counter);
    SetDlgItemText(hwndDlg, res, tmp);
}

///////////////////////////////////////////////////////////////////////////////

void setAverage(HWND hwndDlg, int res, int count, int total) {
    memset(tmp, 0, sizeof(tmp));
    GetDlgItemText(hwndDlg, res, tmp, sizeof(tmp));
    char average[10] = { 0 };
    sprintf(average, "%0.1f", (count) ? ((float) total / (float) count) : 0);
    strreplace(tmp, "%AVG%", average);
    SetDlgItemText(hwndDlg, res, tmp);
}
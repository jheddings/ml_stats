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
#include <hash_map>
#include <string.h>

///////////////////////////////////////////////////////////////////////////////

extern HWND config_tab_init(HWND tab, bool showProcessing);
extern void setCounter(HWND hwndDlg, int res, int count);
extern void setAverage(HWND hwndDlg, int res, int count, int total);

///////////////////////////////////////////////////////////////////////////////

stdext::hash_map<const char*, int, cchar_hashcmp> m_albums;
stdext::hash_map<const char*, int, cchar_hashcmp>::iterator m_iAlbums;

///////////////////////////////////////////////////////////////////////////////

void generateAlbumsHashMap(void) {
    m_albums.clear();

    int aCount;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        aCount = m_albums[m_pRecords->Items[idx].album] + 1;
        m_albums[m_pRecords->Items[idx].album] = aCount;
    }
}

///////////////////////////////////////////////////////////////////////////////

void setLongestAlbum(HWND hwndDlg) {
    const char* pAlbum;
    int max_album = 0;
    for (m_iAlbums = m_albums.begin(); m_iAlbums != m_albums.end(); m_iAlbums++) {
        if (m_iAlbums->second > max_album) {
            pAlbum = m_iAlbums->first;
            max_album = m_iAlbums->second;
        }
    }
    setCounter(hwndDlg, IDC_LONGEST_ALBUM, max_album);
}

///////////////////////////////////////////////////////////////////////////////

void setOneTrackAlbumCounter(HWND hwndDlg) {
    int count = 0;
    for (m_iAlbums = m_albums.begin(); m_iAlbums != m_albums.end(); m_iAlbums++) {
        if (m_iAlbums->second == 1) {
            count++;
        }
    }
    setCounter(hwndDlg, IDC_ONE_TRACK_ALBUMS, count);
}

///////////////////////////////////////////////////////////////////////////////

void setAverageAlbum(HWND hwndDlg) {
    setAverage(hwndDlg, IDC_AVG_ALBUM, m_albums.size(), m_pRecords->Size);
}

///////////////////////////////////////////////////////////////////////////////

void setAlbumSpan(HWND hwndDlg) {

    stdext::hash_map<const char*, int, cchar_hashcmp> hYears;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        hYears[m_pRecords->Items[idx].album] = m_pRecords->Items[idx].year;
    }

    int year;
    int start = 2^10;
    int stop = 0;
    int years[10] = { 0 };
    for (m_iAlbums = hYears.begin(); m_iAlbums != hYears.end(); m_iAlbums++) {
        year = m_iAlbums->second;
        if (year > 0) {
            if (year < start) start = year;
            if (year > stop) stop = year;

            if (year < 1950) years[0]++;
            else if (year < 1960) years[1]++;
            else if (year < 1970) years[2]++;
            else if (year < 1980) years[3]++;
            else if (year < 1990) years[4]++;
            else if (year < 2000) years[5]++;
            else years[6]++;
        }
    }
    memset(tmp, 0, sizeof(tmp));
    GetDlgItemText(hwndDlg, IDC_ALBUM_SPAN, tmp, sizeof(tmp));
    char span[1024] = { 0 };
    sprintf(span, "%d - %d", start, stop);
    strreplace(tmp, "%SPAN%", span);
    SetDlgItemText(hwndDlg, IDC_ALBUM_SPAN, tmp);

    setCounter(hwndDlg, IDC_PRE_1950, years[0]);
    setCounter(hwndDlg, IDC_1950_1959, years[1]);
    setCounter(hwndDlg, IDC_1960_1969, years[2]);
    setCounter(hwndDlg, IDC_1970_1979, years[3]);
    setCounter(hwndDlg, IDC_1980_1989, years[4]);
    setCounter(hwndDlg, IDC_1990_1999, years[5]);
    setCounter(hwndDlg, IDC_2000_PLUS, years[6]);
}

///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK albums_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HWND dlg;
    switch (uMsg) {
        case WM_INITDIALOG:
            dlg = config_tab_init(hwndDlg, true);
            generateAlbumsHashMap();
            setLongestAlbum(hwndDlg);
            setAverageAlbum(hwndDlg);
            setOneTrackAlbumCounter(hwndDlg);
            setAlbumSpan(hwndDlg);
            DestroyWindow(dlg);
			break;
    }

    return 0;
}
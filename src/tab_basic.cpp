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
#include <list>

///////////////////////////////////////////////////////////////////////////////

extern HWND config_tab_init(HWND tab, bool showProcessing);
extern void setCounter(HWND hwndDlg, int res, int count);
extern void setAverage(HWND hwndDlg, int res, int count, int total);

///////////////////////////////////////////////////////////////////////////////

void setArtistCounter(HWND hwndDlg) {
    std::list<char*> artists;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        artists.push_back(m_pRecords->Items[idx].artist);
    }
    artists.sort(strless);
    artists.unique(strequal);
    setCounter(hwndDlg, IDC_ARTIST_COUNT, artists.size());
    //delete artists;
}

///////////////////////////////////////////////////////////////////////////////

void setAlbumCounter(HWND hwndDlg) {
    std::list<char*> albums;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        albums.push_back(m_pRecords->Items[idx].album);
    }
    albums.sort(strless);
    albums.unique(strequal);
    setCounter(hwndDlg, IDC_ALBUM_COUNT, albums.size());
}

///////////////////////////////////////////////////////////////////////////////

void setGenreCounter(HWND hwndDlg) {
    std::list<char*> genres;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        genres.push_back(m_pRecords->Items[idx].genre);
    }
    genres.sort(strless);
    genres.unique(strequal);
    setCounter(hwndDlg, IDC_GENRE_COUNT, genres.size());
}

///////////////////////////////////////////////////////////////////////////////

void setTrackCounter(HWND hwndDlg) {
    setCounter(hwndDlg, IDC_TRACK_COUNT, m_pRecords->Size);
}

///////////////////////////////////////////////////////////////////////////////

void setRatedCounter(HWND hwndDlg) {
    char* rating;
    int count = 0;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        rating = getRecordExtendedItem(&(m_pRecords->Items[idx]), "RATING");
        if ((rating != NULL) && (strcmp(rating, "") != 0) && (atoi(rating) > 0)) {
            count++;
        }
    }
    setCounter(hwndDlg, IDC_RATED_COUNT, count);
}

///////////////////////////////////////////////////////////////////////////////

void setUnratedCounter(HWND hwndDlg) {
    char* rating;
    int count = 0;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        rating = getRecordExtendedItem(&(m_pRecords->Items[idx]), "RATING");
        if ((rating == NULL) || (strcmp(rating, "") == 0) || (atoi(rating) == 0)) {
            count++;
        }
    }
    setCounter(hwndDlg, IDC_UNRATED_COUNT, count);
}

///////////////////////////////////////////////////////////////////////////////

void setRatingCounts(HWND hwndDlg) {
    char* rating;
    int ratings[6] = { 0 };
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        rating = getRecordExtendedItem(&(m_pRecords->Items[idx]), "RATING");
        if ((rating != NULL) && (strcmp(rating, "") != 0) && (atoi(rating) > 0)) {
            ratings[atoi(rating)]++;
        }
    }
    setCounter(hwndDlg, IDC_1STAR_TRACKS, ratings[1]);
    setCounter(hwndDlg, IDC_2STAR_TRACKS, ratings[2]);
    setCounter(hwndDlg, IDC_3STAR_TRACKS, ratings[3]);
    setCounter(hwndDlg, IDC_4STAR_TRACKS, ratings[4]);
    setCounter(hwndDlg, IDC_5STAR_TRACKS, ratings[5]);
}

///////////////////////////////////////////////////////////////////////////////

void setAverageRating(HWND hwndDlg) {
    char* rating;
    int total = 0;
    int count = 0;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        rating = getRecordExtendedItem(&(m_pRecords->Items[idx]), "RATING");
        if ((rating != NULL) && (strcmp(rating, "") != 0) && (atoi(rating) > 0)) {
            count++;
            total += atoi(rating);
        }
    }
    setAverage(hwndDlg, IDC_RATING_AVG, count, total);
}

///////////////////////////////////////////////////////////////////////////////

void setPlayCounter(HWND hwndDlg) {
    int count = 0;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        count += atoi(getRecordExtendedItem(&(m_pRecords->Items[idx]), "PLAYCOUNT"));
    }
    setCounter(hwndDlg, IDC_PLAY_COUNT, count);
}

///////////////////////////////////////////////////////////////////////////////

void setAveragePlayCount(HWND hwndDlg) {
    char* playcount;
    int total = 0;
    int count = 0;
    for (int idx = 0; idx < m_pRecords->Size; idx++) {
        playcount = getRecordExtendedItem(&(m_pRecords->Items[idx]), "PLAYCOUNT");
        if (atoi(playcount) > 0) {
            count++;
            total += atoi(playcount);
        }
    }
    setAverage(hwndDlg, IDC_PLAYCOUNT_AVG, count, total);
}

///////////////////////////////////////////////////////////////////////////////

INT_PTR CALLBACK basic_tabproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HWND dlg;
    switch (uMsg) {
        case WM_INITDIALOG:
            dlg = config_tab_init(hwndDlg, true);
            setArtistCounter(hwndDlg);
            setAlbumCounter(hwndDlg);
            setGenreCounter(hwndDlg);
            setTrackCounter(hwndDlg);
            setRatedCounter(hwndDlg);
            setAverageRating(hwndDlg);
            setRatingCounts(hwndDlg);
            setUnratedCounter(hwndDlg);
            setPlayCounter(hwndDlg);
            setAveragePlayCount(hwndDlg);
            DestroyWindow(dlg);
			break;
    }

    return 0;
}
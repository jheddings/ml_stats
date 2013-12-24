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

int init(void);
void quit(void);
void stats(HWND parent);
int msgproc(int message_type, int param1, int param2, int param3);

///////////////////////////////////////////////////////////////////////////////

char* iniFile;
char tmp[4096];
itemRecordList* m_pRecords;

///////////////////////////////////////////////////////////////////////////////

extern INT_PTR CALLBACK stats_dlgproc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

///////////////////////////////////////////////////////////////////////////////

winampMediaLibraryPlugin plugin = {
    MLHDR_VER,
    PLUGIN_DESC,
    init, quit, msgproc
};

///////////////////////////////////////////////////////////////////////////////

int init(void) {
    iniFile = (char*) SendMessage(plugin.hwndWinampParent, WM_WA_IPC, 0, IPC_GETINIFILE);
    return 0;
}

///////////////////////////////////////////////////////////////////////////////

void quit(void) { } 

///////////////////////////////////////////////////////////////////////////////

void stats(HWND parent) {
    DialogBox(plugin.hDllInstance, MAKEINTRESOURCE(IDD_STATS), parent, stats_dlgproc);
}

///////////////////////////////////////////////////////////////////////////////

itemRecordList* getRecords(char* qString) {
    itemRecordList* records = new itemRecordList();
    records->Size = 0;
    records->Alloc = 0;
    records->Items = NULL;

    // set up the ipc query
    mlQueryStruct query;
    query.max_results = 0;
    query.query = qString;
    query.results.Size = 0;
    query.results.Alloc = 0;
    query.results.Items = NULL;

    // send the message & copy the results
    SendMessage(plugin.hwndLibraryParent, WM_ML_IPC, (WPARAM) &query, ML_IPC_DB_RUNQUERY);
    copyRecordList(records, &(query.results));
    SendMessage(plugin.hwndLibraryParent, WM_ML_IPC, (WPARAM) &query, ML_IPC_DB_FREEQUERYRESULTS);

    return records;
}

///////////////////////////////////////////////////////////////////////////////

int msgproc(int message_type, int param1, int param2, int param3) {
	switch (message_type) {
		case ML_MSG_CONFIG:
			stats((HWND) param1);
			return IDOK;
	}

	return 0;
}
///////////////////////////////////////////////////////////////////////////////

extern "C" {
__declspec( dllexport ) winampMediaLibraryPlugin* winampGetMediaLibraryPlugin(void) {
	return &plugin;
}
};
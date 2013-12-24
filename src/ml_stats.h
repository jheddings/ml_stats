/* Written by Jason Heddings - http://www.heddway.com/
**
** This software is provided 'as-is', without any express or implied warranty.
** In no event will the authors be held liable for any damages arising from
** the use of this software. 
**
** License: http://creativecommons.org/licenses/by-nc-sa/2.0/
** Copyright (c) 2005, Heddway
*/

#ifndef __ML_STATS_H__
#define __ML_STATS_H__

// plugin information
#define VERSION     "1.0"
#define PLUGIN_DESC "Media Library Statistics,v1.0"

#include "utils_str.h"
#include "resource.h"

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <stdlib.h>

// Winamp SDK
#include "ml.h"
#include "wa_ipc.h"

// make all the global ml_org variables accessible
extern char* iniFile;
extern char tmp[4096];
extern itemRecordList* m_pRecords;
extern winampMediaLibraryPlugin plugin;

itemRecordList* getRecords(char* qString);

#endif
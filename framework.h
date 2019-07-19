// header.h: Includedatei für Include-Standardsystemdateien
// oder projektspezifische Includedateien.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Selten verwendete Komponenten aus Windows-Headern ausschließen
// Windows-Headerdateien
#include <windows.h>
// C RunTime-Headerdateien
#include <stdlib.h>
#include <malloc.h>
#include <memory>
#include <tchar.h>

#include <string>
#include "ErrorHandling.h"
#include <wrl.h>
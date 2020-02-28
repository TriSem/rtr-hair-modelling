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

#include <vector>
#include <string>
#include <time.h>
#include <random>
#include "ErrorHandling.h"
#include <wrl.h>
#include <Mouse.h>
#include <Keyboard.h>

using std::shared_ptr;
using std::make_shared;
using std::unique_ptr;
using std::make_unique;
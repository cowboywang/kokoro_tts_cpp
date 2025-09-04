#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include <string>
#include <vector>
#include <memory>
#include <thread>
#include <mutex>
#include <list>
#include <mmsystem.h>

using namespace std;

#include "kokoro_tts_dll_interface.h"
#include "log_interface.h"

#include "wave_writer.h"
#include "wave_render.h"

#define SAFE_DELETE(p)     \
    do {                   \
        delete p;          \
        p = NULL;          \
    } while (0);

#define SAFE_DELETES(p)    \
    do {                   \
        delete[] p;        \
        p = NULL;          \
    } while (0);

inline string UnicodeToUtf8(const wstring &in_str)
{
    int nNeedChars = WideCharToMultiByte(CP_UTF8, 0, in_str.c_str(), in_str.length(), 0, 0, 0, 0);
    if (nNeedChars > 0) {
        string temp;
        temp.resize(nNeedChars);
        ::WideCharToMultiByte(CP_UTF8, 0, in_str.c_str(), -1, &temp[0], nNeedChars, 0, 0);
        return temp;
    }

    return string();
}

inline wstring Utf8ToUnicode(const string& in_utf8Str)
{
    int nNeedWchars = MultiByteToWideChar(CP_UTF8, 0, in_utf8Str.c_str(), in_utf8Str.length(), NULL, 0);
    if (nNeedWchars > 0) {
        wstring temp;
        temp.resize(nNeedWchars);
        ::MultiByteToWideChar(CP_UTF8, 0, in_utf8Str.c_str(), -1, &temp[0], nNeedWchars);
        return temp;
    }

    return wstring();
}

#define WM_ON_INIT  (WM_USER + 100)
#define WM_ON_TTS   (WM_USER + 101)
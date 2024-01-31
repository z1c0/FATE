#ifndef __FATETYPEDEFS__H__
#define __FATETYPEDEFS__H__

#ifdef _WIN32
  #include <windows.h>
  #include <winsock.h>
  #include <stdio.h>
  #include <TCHAR.h>  
  #define FATE_SECTION  // just for compatibility

#ifdef _WIN32_WCE
  #include <aygshell.h>
  #include <gx.h>
#endif

#else
 
// PalmOS specfic
#ifndef FATE_SECTION
  #define FATE_SECTION __attribute__ ((section ("sect_FW"))) 
#endif

//------------------------------------------------------------------------------
/// String functions
#define TEXT
typedef const char* LPCTSTR;
typedef char TCHAR;

#define _tcscmp StrCompare
#define _tcslen StrLen
#define _tcscpy StrCopy


//------------------------------------------------------------------------------
/// RGB

//#define RGB(r, g ,b) ((unsigned long)(((unsigned char)(r)|((unsigned short)(g)<<8))|(((unsigned long)(unsigned char)(b))<<16))) 
#define RGB(r, g, b) (unsigned short)((((unsigned char)(r)&0xF8)<<8)|(((unsigned char)(g)&0xFC)<<3)|(((unsigned char)(b)&0xF8)>>3))
typedef unsigned long COLORREF;


#define GetRValue(rgb) ((unsigned char)(((unsigned short)(rgb)&0xF800)>>8))
#define GetGValue(rgb) ((unsigned char)(((unsigned short)rgb)>>3)&0xFC)
#define GetBValue(rgb) ((((unsigned char)rgb) << 3) & 0xF8)


//------------------------------------------------------------------------------

#define MAX_PATH 256

typedef void* HDC;


struct RECT
{
  int left;
  int top;
  int right;
  int bottom;
};

typedef void* HPEN;
typedef void* HBRUSH;

#endif  


//------------------------------------------------------------------------------

//
// Common
//
class CFBitmap;
struct ITEMLISTENTRY
{
  TCHAR *pszItem;
  TCHAR *pszAddInfo;
  unsigned long ulIndex;
  unsigned long ulUser;
  CFBitmap *pBmp;
  struct ITEMLISTENTRY *pNext;
};

// Drawing modes determine how the screen contents are drawn (flipped / landscape).
enum EFateDrawMode
{
  DM_PORTRAIT,
  DM_PORTRAIT_FLIPPED,
  DM_LANDSCAPE,     
  DM_LANDSCAPE_FLIPPED,
};

// Macro for safe memory release.
#define SAFE_DELETE(p)  { if (p) { delete(p); (p)= NULL; } }


#endif  // __FATETYPEDEFS__H__
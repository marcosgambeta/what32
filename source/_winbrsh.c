
// WHAT32
// Brush functions

/*

   GetBrushOrgEx(hDC) -> aPt, or NIL on failure
   SetBrushOrgEx(hDC,x,y,) -> aOldPt, or NIL on failure

*/

#define HB_OS_WIN_USED
#define _WIN32_WINNT 0x0400

#include <windows.h>
//#include <item.api>
#include <hbapi.h>

#include "what32.h"

//-----------------------------------------------------------------------------

HB_FUNC(CREATESOLIDBRUSH)
{
  hb_retnl((LONG)CreateSolidBrush(w32_par_COLORREF(1))); // brush color
}

//-----------------------------------------------------------------------------

HB_FUNC(CREATEPATTERNBRUSH)
{
  hb_retnl((LONG)CreatePatternBrush(w32_par_HBITMAP(1))); // bitmap handle
}

//-----------------------------------------------------------------------------

// WINGDIAPI HBRUSH WINAPI CreateDIBPatternBrush( IN HGLOBAL, IN UINT);

HB_FUNC(CREATEDIBPATTERNBRUSH)
{
  hb_retnl((LONG)CreateDIBPatternBrush(w32_par_HGLOBAL(1), w32_par_UINT(2)));
}

//-----------------------------------------------------------------------------

// WINGDIAPI HBRUSH WINAPI CreateBrushIndirect( IN CONST LOGBRUSH *);

// uses structure

HB_FUNC(CREATEBRUSHINDIRECT)
{
  LOGBRUSH *lb = (LOGBRUSH *)hb_param(1, HB_IT_STRING)->item.asString.value;

  hb_retnl((LONG)CreateBrushIndirect(lb));
}

//-----------------------------------------------------------------------------

// WINGDIAPI HBRUSH WINAPI CreateHatchBrush( IN int, IN COLORREF);

HB_FUNC(CREATEHATCHBRUSH)
{

  hb_retnl((LONG)CreateHatchBrush(hb_parni(1), w32_par_COLORREF(2)));
}

//-----------------------------------------------------------------------------

// no prototype ?

/*

// WINGDIAPI COLORREF WINAPI GetDCBrushColor( IN HDC);

HB_FUNC( GETDCBRUSHCOLOR )
{
   hb_retnl( (ULONG) GetDCBrushColor( (HDC) hb_parnl( 1 ) ) ) ;
}

*/

//-----------------------------------------------------------------------------

// no prototype ?

/*
// WINGDIAPI COLORREF WINAPI SetDCBrushColor(IN HDC, IN COLORREF);

HB_FUNC( SETDCBRUSHCOLOR )
{

   hb_retnl( (ULONG) SetDCBrushColor( (HDC) hb_parnl( 1 ), (COLORREF) hb_parnl( 2 ) ) ) ;
}
*/
//-----------------------------------------------------------------------------

// WINGDIAPI HBRUSH WINAPI CreateDIBPatternBrushPt( IN CONST VOID *, IN UINT);

// uses structure

HB_FUNC(CREATEDIBPATTERNBRUSHPT)
{
  BITMAPINFO *bmi = (BITMAPINFO *)hb_param(1, HB_IT_STRING)->item.asString.value;

  hb_retnl((LONG)CreateDIBPatternBrushPt(bmi, w32_par_UINT(2)));
}

//-----------------------------------------------------------------------------
// WINGDIAPI BOOL WINAPI FixBrushOrgEx( IN HDC, IN int, IN int, IN LPPOINT);

// obsolete - not implemented

HB_FUNC(FIXBRUSHORGEX)
{
  POINT *Point = (POINT *)hb_param(4, HB_IT_STRING)->item.asString.value;

  w32_ret_BOOL(FixBrushOrgEx(w32_par_HDC(1), hb_parni(2), hb_parni(3), Point));
}

//-----------------------------------------------------------------------------

// WINGDIAPI BOOL WINAPI GetBrushOrgEx( IN HDC, OUT LPPOINT);

HB_FUNC(GETBRUSHORGEX)
{
  POINT Point;
  PHB_ITEM aPt;
  PHB_ITEM temp;

  if (GetBrushOrgEx(w32_par_HDC(1), &Point)) {
    aPt = hb_itemArrayNew(2);

    temp = hb_itemPutNL(NULL, Point.x);
    hb_arraySet(aPt, 1, temp);
    hb_itemRelease(temp);

    temp = hb_itemPutNL(NULL, Point.y);
    hb_arraySet(aPt, 2, temp);
    hb_itemRelease(temp);

    hb_itemReturn(aPt);
    hb_itemRelease(aPt);
  }
}

//-----------------------------------------------------------------------------

// WINGDIAPI BOOL WINAPI SetBrushOrgEx( IN HDC, IN int, IN int, OUT LPPOINT);

HB_FUNC(SETBRUSHORGEX)
{

  POINT Point;
  PHB_ITEM aPt;
  PHB_ITEM temp;

  if (SetBrushOrgEx(w32_par_HDC(1), hb_parni(2), hb_parni(3), &Point)) {
    aPt = hb_itemArrayNew(2);

    temp = hb_itemPutNL(NULL, Point.x);
    hb_arraySet(aPt, 1, temp);
    hb_itemRelease(temp);

    temp = hb_itemPutNL(NULL, Point.y);
    hb_arraySet(aPt, 2, temp);
    hb_itemRelease(temp);

    hb_itemReturn(aPt);
    hb_itemRelease(aPt);
  }
}

#define _WIN32_WINNT 0x0400
#define _WIN32_IE 0x0400

#ifndef __WATCOMC__

#include <windows.h>
#include <commctrl.h>
#include <shlobj.h>

//#include <item.api>
#include <hbapi.h>
#include <hbvm.h>
#include <hbstack.h>
#include <hbapiitm.h>

#include "what32.h"

/* add parens to avoid warning */
#if defined(__BORLANDC__) && (__BORLANDC__ <= 0x620)
#undef MAKEWORD
#undef MAKELONG
#define MAKEWORD(a, b) ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | (((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8)))
#define MAKELONG(a, b)                                                                                                 \
  ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | (((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16)))
#endif

#if defined(__DMC__)
#if 0
#define TC_HITTESTINFO TCHITTESTINFO
typedef struct tagTCHITTESTINFO
{
    POINT pt;
    UINT flags;
} TCHITTESTINFO, FAR * LPTCHITTESTINFO;
#endif
#define CCM_FIRST 0x2000
#define TCM_SETMINTABWIDTH (TCM_FIRST + 49)
#define TabCtrl_SetMinTabWidth(hwnd, x) (int)SNDMSG((hwnd), TCM_SETMINTABWIDTH, 0, x)
#define TCM_FIRST 0x1300 // Tab control messages
#define TCM_DESELECTALL (TCM_FIRST + 50)
#define TabCtrl_DeselectAll(hwnd, fExcludeFocus) (void)SNDMSG((hwnd), TCM_DESELECTALL, fExcludeFocus, 0)
#define TCM_HIGHLIGHTITEM (TCM_FIRST + 51)
#define TabCtrl_HighlightItem(hwnd, i, fHighlight)                                                                     \
  (BOOL) SNDMSG((hwnd), TCM_HIGHLIGHTITEM, (WPARAM)(i), (LPARAM)MAKELONG(fHighlight, 0))
#define TCM_SETEXTENDEDSTYLE (TCM_FIRST + 52) // optional wParam == mask
#define TabCtrl_SetExtendedStyle(hwnd, dw) (DWORD) SNDMSG((hwnd), TCM_SETEXTENDEDSTYLE, 0, dw)
#define TCM_GETEXTENDEDSTYLE (TCM_FIRST + 53)
#define TabCtrl_GetExtendedStyle(hwnd) (DWORD) SNDMSG((hwnd), TCM_GETEXTENDEDSTYLE, 0, 0)
#define CCM_SETUNICODEFORMAT (CCM_FIRST + 5)
#define CCM_GETUNICODEFORMAT (CCM_FIRST + 6)
#define TCM_SETUNICODEFORMAT CCM_SETUNICODEFORMAT
#define TabCtrl_SetUnicodeFormat(hwnd, fUnicode) (BOOL) SNDMSG((hwnd), TCM_SETUNICODEFORMAT, (WPARAM)(fUnicode), 0)
#define TCM_GETUNICODEFORMAT CCM_GETUNICODEFORMAT
#define TabCtrl_GetUnicodeFormat(hwnd) (BOOL) SNDMSG((hwnd), TCM_GETUNICODEFORMAT, 0, 0)
#endif

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_CREATE)
{
  HWND hwnd;
  HWND hbutton;
  LONG hFont;
  LONG style;
  style = ISNIL(6) ? 0 : (LONG)hb_parnl(6);
  hwnd = w32_par_HWND(1);
  hFont = SendMessage(hwnd, WM_GETFONT, 0, 0);
  hbutton = CreateWindowEx(0, WC_TABCONTROL, NULL, style, hb_parni(2), hb_parni(3), hb_parni(4), hb_parni(5), hwnd,
                           NULL, GetModuleHandle(NULL), NULL);
  SendMessage(hbutton, (UINT)WM_SETFONT, (WPARAM)hFont, 1);
  hb_retnl((LONG)hbutton);
}

//---------------------------------------------------------------------------//

// not an API

HB_FUNC(TABCTRL_ADDITEM)
{
  int iCount = TabCtrl_GetItemCount(w32_par_HWND(1));
  TC_ITEM item;

  item.mask = TCIF_TEXT | TCIF_IMAGE;
  item.iImage = ISNIL(3) ? -1 : (LONG)hb_parnl(3);
  item.pszText = (LPSTR)hb_parcx(2);

  hb_retni(TabCtrl_InsertItem(w32_par_HWND(1), iCount, &item));
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_INSERTITEM)
{
  TC_ITEM item;
  item.mask = TCIF_TEXT | TCIF_IMAGE;
  item.iImage = ISNIL(4) ? -1 : (LONG)hb_parnl(4);
  item.pszText = (LPSTR)hb_parcx(2);
  hb_retni(TabCtrl_InsertItem(w32_par_HWND(1), (INT)hb_parni(3), &item));
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_SETCURSEL)
{
  hb_retni(TabCtrl_SetCurSel(w32_par_HWND(1), hb_parni(2)));
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_GETCURSEL)
{
  hb_retni(TabCtrl_GetCurSel(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_GETITEM)
{
  TC_ITEM item;
  hb_retl(TabCtrl_GetItem(w32_par_HWND(1), (int)hb_parni(2), &item));

  // assign  item to param 3
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_GETITEMCOUNT)
{
  hb_retni(TabCtrl_GetItemCount(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_GETITEMRECT)
{
  RECT rc;
  PHB_ITEM aRect = hb_itemArrayNew(4);
  PHB_ITEM temp;

  TabCtrl_GetItemRect(w32_par_HWND(1), hb_parni(2), &rc);

  temp = hb_itemPutNL(NULL, rc.left);
  hb_arraySet(aRect, 1, temp);
  hb_itemRelease(temp);

  temp = hb_itemPutNL(NULL, rc.top);
  hb_arraySet(aRect, 2, temp);
  hb_itemRelease(temp);

  temp = hb_itemPutNL(NULL, rc.right);
  hb_arraySet(aRect, 3, temp);
  hb_itemRelease(temp);

  temp = hb_itemPutNL(NULL, rc.bottom);
  hb_arraySet(aRect, 4, temp);
  hb_itemRelease(temp);

  hb_itemReturn(aRect);
  hb_itemRelease(aRect);
}

//---------------------------------------------------------------------------//

HB_FUNC(TABCTRL_GETROWCOUNT)
{
  hb_retni(TabCtrl_GetRowCount(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// TabCtrl_GetImageList(hwnd)
// (HIMAGELIST)SNDMSG((hwnd), TCM_GETIMAGELIST, 0, 0L)

HB_FUNC(TABCTRL_GETIMAGELIST)
{
  hb_retnl((LONG)TabCtrl_GetImageList(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// #define TabCtrl_SetImageList(hwnd, himl)
// (HIMAGELIST)SNDMSG((hwnd), TCM_SETIMAGELIST, 0, (LPARAM)(HIMAGELIST)(himl))

HB_FUNC(TABCTRL_SETIMAGELIST)
{
  hb_retnl((LONG)TabCtrl_SetImageList(w32_par_HWND(1), (LPARAM)w32_par_HIMAGELIST(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetItem(hwnd, iItem, pitem)
// (BOOL)SNDMSG((hwnd), TCM_SETITEM, (WPARAM)(int)(iItem), (LPARAM)(TC_ITEM FAR*)(pitem))

HB_FUNC(TABCTRL_SETITEM)
{
  TC_ITEM item;
  item.mask = TCIF_TEXT | TCIF_IMAGE;
  item.iImage = -1;
  item.pszText = (LPSTR)hb_parcx(3);
  hb_retl(TabCtrl_SetItem(w32_par_HWND(1), hb_parni(2), &item));
}

//---------------------------------------------------------------------------//
// TabCtrl_DeleteAllItems(hwnd)
// (BOOL)SNDMSG((hwnd), TCM_DELETEALLITEMS, 0, 0L)

HB_FUNC(TABCTRL_DELETEALLITEMS)
{
  hb_retl(TabCtrl_DeleteAllItems(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// TabCtrl_DeleteItem(hwnd, i)
// (BOOL)SNDMSG((hwnd), TCM_DELETEITEM, (WPARAM)(int)(i), 0L)

HB_FUNC(TABCTRL_DELETEITEM)
{
  hb_retl(TabCtrl_DeleteItem(w32_par_HWND(1), (WPARAM)hb_parni(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_HitTest(hwndTC, pinfo)
// (int)SNDMSG((hwndTC), TCM_HITTEST, 0, (LPARAM)(TC_HITTESTINFO FAR*)(pinfo))

// waiting for structures

HB_FUNC(TABCTRL_HITTEST)
{
  TCHITTESTINFO tcht;

  hb_parni(TabCtrl_HitTest(w32_par_HWND(1), &tcht));

  // assign to structure in param 2
}

//---------------------------------------------------------------------------//
// TabCtrl_SetItemExtra(hwndTC, cb)
// (BOOL)SNDMSG((hwndTC), TCM_SETITEMEXTRA, (WPARAM)(cb), 0L)

HB_FUNC(TABCTRL_SETITEMEXTRA)
{
  hb_retl(TabCtrl_SetItemExtra(w32_par_HWND(1), (int)hb_parni(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_AdjustRect(hwnd, bLarger, prc)
// (int)SNDMSG(hwnd, TCM_ADJUSTRECT, (WPARAM)(BOOL)(bLarger), (LPARAM)(RECT FAR *)prc)

HB_FUNC(TABCTRL_ADJUSTRECT)
{
  RECT rc;

  if (ISARRAY(3)) {
    rc.left = hb_parnl(3, 1);
    rc.top = hb_parnl(3, 2);
    rc.right = hb_parnl(3, 3);
    rc.bottom = hb_parnl(3, 4);

    TabCtrl_AdjustRect(w32_par_HWND(1), (BOOL)hb_parl(2), &rc);

    hb_stornl(rc.left, 3, 1);
    hb_stornl(rc.top, 3, 2);
    hb_stornl(rc.right, 3, 3);
    hb_stornl(rc.bottom, 3, 4);
  }
}

//---------------------------------------------------------------------------//
// TabCtrl_SetItemSize(hwnd, x, y)
// (DWORD)SNDMSG((hwnd), TCM_SETITEMSIZE, 0, MAKELPARAM(x,y))

HB_FUNC(TABCTRL_SETITEMSIZE)
{
  hb_retnl(TabCtrl_SetItemSize(w32_par_HWND(1), (int)hb_parni(2), (int)hb_parni(3)));
}

//---------------------------------------------------------------------------//
// TabCtrl_RemoveImage(hwnd, i)
// (void)SNDMSG((hwnd), TCM_REMOVEIMAGE, i, 0L)

HB_FUNC(TABCTRL_REMOVEIMAGE)
{
  TabCtrl_RemoveImage(w32_par_HWND(1), (int)hb_parni(2));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetPadding(hwnd,  cx, cy)
// (void)SNDMSG((hwnd), TCM_SETPADDING, 0, MAKELPARAM(cx, cy))

HB_FUNC(TABCTRL_SETPADDING)
{
  TabCtrl_SetPadding(w32_par_HWND(1), (int)hb_parni(2), (int)hb_parni(3));
}

//---------------------------------------------------------------------------//
// TabCtrl_GetToolTips(hwnd)
// (HWND)SNDMSG((hwnd), TCM_GETTOOLTIPS, 0, 0L)

HB_FUNC(TABCTRL_GETTOOLTIPS)
{
  hb_retnl((LONG)TabCtrl_GetToolTips(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetToolTips(hwnd, hwndTT)
// (void)SNDMSG((hwnd), TCM_SETTOOLTIPS, (WPARAM)(hwndTT), 0L)

HB_FUNC(TABCTRL_SETTOOLTIPS)
{
  TabCtrl_SetToolTips(w32_par_HWND(1), w32_par_HWND(2));
}

//---------------------------------------------------------------------------//
// TabCtrl_GetCurFocus(hwnd)
// (int)SNDMSG((hwnd), TCM_GETCURFOCUS, 0, 0)

HB_FUNC(TABCTRL_GETCURFOCUS)
{
  hb_retni(TabCtrl_GetCurFocus(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetCurFocus(hwnd, i)
// SNDMSG((hwnd),TCM_SETCURFOCUS, i, 0)

HB_FUNC(TABCTRL_SETCURFOCUS)
{
  TabCtrl_SetCurFocus(w32_par_HWND(1), (int)hb_parni(2));
  hb_ret();
}

//---------------------------------------------------------------------------//
// TabCtrl_SetMinTabWidth(hwnd, x)
// (int)SNDMSG((hwnd), TCM_SETMINTABWIDTH, 0, x)

HB_FUNC(TABCTRL_SETMINTABWIDTH)
{
  hb_retni(TabCtrl_SetMinTabWidth(w32_par_HWND(1), (int)hb_parni(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_DeselectAll(hwnd, fExcludeFocus)
// (void)SNDMSG((hwnd), TCM_DESELECTALL, fExcludeFocus, 0)

HB_FUNC(TABCTRL_DESELECTALL)
{
  TabCtrl_DeselectAll(w32_par_HWND(1), (UINT)hb_parni(2));
}

//---------------------------------------------------------------------------//
// TabCtrl_HighlightItem(hwnd, i, fHighlight)
// (BOOL)SNDMSG((hwnd), TCM_HIGHLIGHTITEM, (WPARAM)(i), (LPARAM)MAKELONG (fHighlight, 0))

HB_FUNC(TABCTRL_HIGHLIGHTITEM)
{
  hb_retl(TabCtrl_HighlightItem(w32_par_HWND(1), (int)hb_parni(2), (WORD)hb_parni(3)));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetExtendedStyle(hwnd, dw)
// (DWORD)SNDMSG((hwnd), TCM_SETEXTENDEDSTYLE, 0, dw)

HB_FUNC(TABCTRL_SETEXTENDEDSTYLE)
{
  hb_retnl(TabCtrl_SetExtendedStyle(w32_par_HWND(1), w32_par_DWORD(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_GetExtendedStyle(hwnd)
// (DWORD)SNDMSG((hwnd), TCM_GETEXTENDEDSTYLE, 0, 0)

HB_FUNC(TABCTRL_GETEXTENDEDSTYLE)
{
  hb_retnl(TabCtrl_GetExtendedStyle(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//
// TabCtrl_SetUnicodeFormat(hwnd, fUnicode)
// (BOOL)SNDMSG((hwnd), TCM_SETUNICODEFORMAT, (WPARAM)(fUnicode), 0)

HB_FUNC(TABCTRL_SETUNICODEFORMAT)
{
  hb_retl(TabCtrl_SetUnicodeFormat(w32_par_HWND(1), hb_parl(2)));
}

//---------------------------------------------------------------------------//
// TabCtrl_GetUnicodeFormat(hwnd)
// (BOOL)SNDMSG((hwnd), TCM_GETUNICODEFORMAT, 0, 0)

HB_FUNC(TABCTRL_GETUNICODEFORMAT)
{
  hb_retl(TabCtrl_GetUnicodeFormat(w32_par_HWND(1)));
}

//---------------------------------------------------------------------------//

#endif

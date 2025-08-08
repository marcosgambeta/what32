#undef _WIN32_IE
#define _WIN32_IE 0x0400

#include <windows.h>
#include <shlobj.h>
#include <commctrl.h>

#include "hbapi.h"

#include "what32.h"

//-------------------------------------------------------------------------//

HB_FUNC(TVINSERTITEM)
{
  TV_INSERTSTRUCT is;

  is.hParent = (HTREEITEM)hb_parnl(3);
  is.hInsertAfter = TVI_LAST;

#if defined(__BORLANDC__) && (__BORLANDC__ <= 1424)
  is.DUMMYUNIONNAME.item.pszText = (LPTSTR)hb_parcx(2);
  is.DUMMYUNIONNAME.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
  is.DUMMYUNIONNAME.item.iImage = hb_parnl(4);
  is.DUMMYUNIONNAME.item.iSelectedImage = hb_parnl(4);
#else
  is.item.pszText = (LPSTR)hb_parcx(2);
  is.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
  is.item.iImage = hb_parnl(4);
  is.item.iSelectedImage = hb_parnl(4);
#endif

  hb_retnl(SendMessage(w32_par_HWND(1), TVM_INSERTITEM, 0, (LPARAM)(LPTV_INSERTSTRUCT)(&is)));
}

//-------------------------------------------------------------------------//
HB_FUNC(TVDELETEITEM)
{
  TreeView_DeleteItem(w32_par_HWND(1), (HTREEITEM)hb_parnl(2));
}

//-------------------------------------------------------------------------//

HB_FUNC(TVSETIMAGELIST) // ( hWnd, hImageList, nType )
{
  hb_retnl((LONG)TreeView_SetImageList(w32_par_HWND(1), (HIMAGELIST)hb_parnl(2), hb_parnl(3)));
}

//-------------------------------------------------------------------------//

HB_FUNC(TVGETSELTEXT) // ( hWnd ) --> cText
{
  HWND hWnd = w32_par_HWND(1);
  HTREEITEM hItem = TreeView_GetSelection(hWnd);
  TV_ITEM tvi;
  BYTE buffer[100];
  if (hItem)
  {
    tvi.mask = TVIF_TEXT;
    tvi.hItem = hItem;
    tvi.pszText = (char *)buffer;
    tvi.cchTextMax = 100;
    TreeView_GetItem(hWnd, &tvi);
    hb_retc(tvi.pszText);
  }
  else
    hb_retc("");
}

//-------------------------------------------------------------------------//

HB_FUNC(TVGETSELECTED) // ( hWnd ) --> hItem
{
  hb_retnl((LONG)TreeView_GetSelection(w32_par_HWND(1)));
}

//-------------------------------------------------------------------------//

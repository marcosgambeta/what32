
// What32.Lib
// ToolBar functions

#define HB_OS_WIN_USED
#define _WIN32_WINNT 0x0400

// #include <shlobj.h>
#include <windows.h>
#include "item.api"
#include <commctrl.h>
// #include "hbapiitm.h"
#include "hbapi.h"
// #include "hbvm.h"
// #include "hbstack.h"

#define w32_par_HWND(n) (HWND)hb_parnl(n)
#define w32_par_HINSTANCE(n) (HINSTANCE)hb_parnl(n)
#define w32_par_DWORD(n) (DWORD)hb_parnl(n)
#define w32_par_UINT(n) (UINT)hb_parni(n)

extern PHB_ITEM Rect2Array(RECT *rc);
extern BOOL Array2Rect(PHB_ITEM aRect, RECT *rc);
extern PHB_ITEM Point2Array(POINT *pt);
extern BOOL Array2Point(PHB_ITEM aPoint, POINT *pt);
extern BOOL Array2Size(PHB_ITEM aSize, SIZE *siz);
extern PHB_ITEM Size2Array(SIZE *siz);
extern void Point2ArrayEx(POINT *pt, PHB_ITEM aPoint);
extern void Rect2ArrayEx(RECT *pt, PHB_ITEM aRect);
extern void Size2ArrayEx(SIZE *siz, PHB_ITEM aSize);

//-----------------------------------------------------------------------------
// HBITMAP CreateMappedBitmap(HINSTANCE hInstance, int idBitmap, UINT wFlags,
//                            LPCOLORMAP lpColorMap, int iNumMaps )

HB_FUNC(CREATEMAPPEDBITMAP)
{
  COLORMAP *cm = (COLORMAP *)hb_param(4, HB_IT_STRING)->item.asString.value;

  hb_retnl((LONG)CreateMappedBitmap(w32_par_HINSTANCE(1), (int)hb_parni(2), w32_par_UINT(3),
                                    ISNIL(4) ? NULL : (COLORMAP *)cm, (int)hb_parni(5)));
}

//-----------------------------------------------------------------------------
//  HWND CreateToolbarEx( HWND hwnd, DWORD ws, UINT wID,int nBitmaps,HINSTANCE hBMInst,
//                        UINT wBMID,LPCTBBUTTON lpButtons,int iNumButtons,int dxButton,
//                        int dyButton, int dxBitmap, int dyBitmap,UINT uStructSize );

HB_FUNC(CREATETOOLBAREX)
{

  hb_retnl((LONG)CreateToolbarEx(w32_par_HWND(1),                        // parent
                                 w32_par_DWORD(2),                       // style
                                 w32_par_UINT(3),                        // id,
                                 (int)hb_parni(4),                         // number of btn images in bmp
                                 ISNIL(5) ? NULL : w32_par_HINSTANCE(5), // hInst of bmp
                                 (UINT)hb_parnl(6),                        // resource id, or hBmp handle
                                 (LPCTBBUTTON)hb_parcx(7),                 // array of button structures
                                 (int)hb_parni(8),                         // number of buttons to add
                                 (int)hb_parni(9),                         // width of button
                                 (int)hb_parni(10),                        // height of button
                                 (int)hb_parni(11),                        // width of bitmap
                                 (int)hb_parni(12),                        // height of bitmap
                                 w32_par_UINT(13)));                     // size of TBBUTTON
}

//-----------------------------------------------------------------------------

HB_FUNC(GETTOOLBARITEMRECT)
{
  RECT rc = {0, 0, 0, 0};
  PHB_ITEM aRect;
  SendMessage(w32_par_HWND(1), TB_GETITEMRECT, hb_parni(2), (LPARAM)&rc);
  //   MapWindowPoints((HWND) hb_parnl(1), HWND_DESKTOP, (POINT*)&rc, 2);
  aRect = Rect2Array(&rc);
  _itemReturn(aRect);
  _itemRelease(aRect);
}

//-----------------------------------------------------------------------------

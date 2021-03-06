/* HARBOUR C FUNCTIONS: */

/*
* Low Level C Routines
*/

//#define _WIN32_WINNT   0x0400

#include <windows.h>
#include <shlobj.h>
//#include <commctrl.h>

#include "hbapi.h"
#include "hbvm.h"
#include "hbstack.h"
#include "hbapiitm.h"

LRESULT CALLBACK __WndProc   (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc2  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc3  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc4  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc5  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc6  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc7  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc8  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc9  (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;
LRESULT CALLBACK __WndProc10 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;

BOOL CALLBACK __DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) ;

int nCopyAnsiToWideChar (LPWORD lpWCStr, LPSTR lpAnsiIn);
LPWORD lpwAlign ( LPWORD lpIn);

//-----------------------------------------------------------------------------

// note: specifying (HBRUSH) COLOR_WINDOW+1 makes it not working
//       (the window background stays black)
//       but not specifying it generates a WARNING, but it works OK.

HB_FUNC( _REGISTERCLASS )
{
   WNDCLASS *wndclass = ( WNDCLASS *) hb_param( 1, HB_IT_STRING )->item.asString.value ;
   wndclass->lpfnWndProc   = __WndProc ;

   /*
   wndclass.style         = (ISNIL(1) ? (CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS)  : hb_parni(1) );
   wndclass.lpfnWndProc   = __WndProc ;
   wndclass.cbClsExtra    = ( ISNIL(2)  ? 0 : hb_parni(2)) ;
   wndclass.cbWndExtra    = ( ISNIL(3)  ? 0 : hb_parni(3)) ;
   wndclass.hInstance     = ( ISNIL(4)  ? GetModuleHandle(NULL) : (HANDLE) hb_parnl(4) ) ;
   wndclass.hIcon         = ( ISNIL(5)  ? LoadIcon(GetModuleHandle(NULL),"") : (HICON) hb_parnl(5) ) ;
   wndclass.hCursor       = ( ISNIL(6)  ? LoadCursor (NULL, IDC_ARROW) : (HCURSOR) hb_parnl(6) ) ;
   wndclass.hbrBackground = ( ISNIL(7)  ? (INT) COLOR_WINDOW  + 1 :  (HBRUSH) hb_parnl(7) ) ;
   wndclass.lpszMenuName  = (LPCSTR) ( !ISNIL(8) ? hb_parc(8) : NULL ) ;
   wndclass.lpszClassName = (LPCSTR) hb_parc(9) ;
   */

   hb_retl( RegisterClass (wndclass));
}


//-----------------------------------------------------------------------------

HB_FUNC( _UNREGISTERCLASS )
{
   HANDLE hInst = ( ISNIL(2) ? GetModuleHandle(NULL) : (HANDLE) hb_parnl(2) ) ;

   hb_retl( UnregisterClass( hb_parc(1), (HINSTANCE) hInst ) ) ;
}

//-----------------------------------------------------------------------------

// hMenu or Window ID are interchangable
// casting Id to hMenu stops it from working correctly

HB_FUNC( _CREATEWINDOWEX )
{
   DWORD  dwExStyle  = (ISNIL(1)  ? 0 : hb_parnl(1)) ;
   LPCSTR cClass     = (LPCSTR) hb_parc(2);
   LPCSTR cTitle     = (LPCSTR) hb_parc(3);
   DWORD  nStyle     = (ISNIL(4)  ? 0 : (DWORD) hb_parnd(4) );
   int    x          = (ISNIL(5)  ? CW_USEDEFAULT : hb_parni(5));
   int    y          = (ISNIL(6)  ? CW_USEDEFAULT : hb_parni(6));
   int    nWidth     = (ISNIL(7)  ? CW_USEDEFAULT : hb_parni(7));
   int    nHeight    = (ISNIL(8)  ? CW_USEDEFAULT : hb_parni(8));
   HWND   hWndParent = (ISNIL(9)  ? (HWND) NULL : (HWND) hb_parnl(9)) ;
   HMENU  hMenu      = (ISNIL(10) ? (HMENU) NULL : (HMENU) hb_parni(10));
   HANDLE hInstance  = (ISNIL(11) ? GetModuleHandle( NULL ) : (HANDLE) hb_parnl(11));
   LPVOID lParam     = (ISNIL(12) ? NULL : (LPVOID) hb_parnl(12));

   HWND hWnd = CreateWindowEx( dwExStyle, cClass, cTitle,
                               nStyle, x, y, nWidth, nHeight,
                               hWndParent, hMenu, (HINSTANCE) hInstance, lParam )  ;

   hb_retnl( (LONG) hWnd );
}

//-----------------------------------------------------------------------------

//  Creates child MDI window

HB_FUNC( _CREATEMDIWINDOW )
{
   LPCSTR cClass     = (LPCSTR) hb_parc(1);
   LPCSTR cTitle     = (LPCSTR) hb_parc(2);
   DWORD  nStyle     = (ISNIL(3)  ? WS_MAXIMIZE : (DWORD) hb_parnd(3) );
   int    x          = (ISNIL(4)  ? CW_USEDEFAULT : hb_parni(4));
   int    y          = (ISNIL(5)  ? CW_USEDEFAULT : hb_parni(5));
   int    nWidth     = (ISNIL(6)  ? CW_USEDEFAULT : hb_parni(6));
   int    nHeight    = (ISNIL(7)  ? CW_USEDEFAULT : hb_parni(7));
   HWND   hWndParent = (ISNIL(8)  ? (HWND) NULL : (HWND) hb_parnl(8)) ;
   HANDLE hInstance  = (ISNIL(9)  ? GetModuleHandle( NULL ) : (HANDLE) hb_parnl(9));
   LPARAM lParam     = (ISNIL(10) ? 0 : (LPARAM) hb_parnl(10));

   HWND hWnd = CreateMDIWindow( (char*) cClass, (char*) cTitle,nStyle,
                                x, y, nWidth, nHeight,
                                hWndParent, (HINSTANCE) hInstance, lParam ) ;
   hb_retnl( (LONG) hWnd );
}


//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 1 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc2 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 2 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc3 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 3 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc4 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 4 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc5 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 5 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}
//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc6 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 6 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc7 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 7 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc8 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 8 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc9 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 9 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}

//-----------------------------------------------------------------------------

LRESULT CALLBACK __WndProc10 (HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   long int res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmPushLong( 10 );
      hb_vmDo( 5 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return( DefWindowProc( hWnd, message, wParam, lParam ));
}


//-----------------------------------------------------------------------------

// called once, on start-up

HB_FUNC( GETWNDPROC )
{

  switch ( hb_parni(1) )
  {
    case 10:
      hb_retnl( (ULONG) __WndProc10 ) ;
      return ;

    case 9:
      hb_retnl( (ULONG) __WndProc9 ) ;
      return ;

    case 8:
      hb_retnl( (ULONG) __WndProc8 ) ;
      return ;

    case 7:
      hb_retnl( (ULONG) __WndProc7 ) ;
      return ;

    case 6:
      hb_retnl( (ULONG) __WndProc6 ) ;
      return ;

    case 5:
      hb_retnl( (ULONG) __WndProc5 ) ;
      return ;

    case 4:
      hb_retnl( (ULONG) __WndProc4 ) ;
      return ;

    case 3:
      hb_retnl( (ULONG) __WndProc3 ) ;
      return ;

    case 2:
      hb_retnl( (ULONG) __WndProc2 ) ;
      return ;

    case 1:
      hb_retnl( (ULONG) __WndProc ) ;
      return ;

    default:
      hb_retnl( (ULONG) 0 ) ;
      return ;

  }

}


//----------------------------------------------------------------------------

HB_FUNC( _GETDLGPROC )
{
    hb_retnl( (ULONG) __DlgProc ) ;
}

//-----------------------------------------------------------------------------

BOOL CALLBACK __DlgProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
   static PHB_DYNS pSymTest = 0 ;
   BOOL res;

   if ( !pSymTest )
     pSymTest = hb_dynsymFind( "_PROCESSDLGMSG" );

   if ( pSymTest )
   {
      hb_vmPushSymbol( pSymTest->pSymbol );
      hb_vmPushNil();
      hb_vmPushLong( (LONG ) hWnd );
      hb_vmPushLong( (LONG ) message );
      hb_vmPushLong( (LONG ) wParam );
      hb_vmPushLong( (LONG ) lParam );
      hb_vmDo( 4 );
      res = hb_itemGetNL( hb_param( -1, HB_IT_ANY ) );

      return res;
   }
   else // shouldn't happen
      return FALSE ;
}



//----------------------------------------------------------------------------

HB_FUNC( _DIALOGBOX )
{


  hb_retni( DialogBox( (ISNIL(1)  ? GetModuleHandle(NULL) : (HINSTANCE) hb_parnl(1) )  ,
                       (hb_parinfo(2)==HB_IT_STRING ? hb_parc(2) : MAKEINTRESOURCE( (WORD) hb_parni(2))) ,
                       (ISNIL(3) ?  NULL : (HWND) hb_parnl(3) )        ,
                       (DLGPROC) hb_parnl(4)
                     ));

}


//----------------------------------------------------------------------------

HB_FUNC( _DIALOGBOXINDIRECT )
{

   hb_retni( DialogBoxIndirect( (ISNIL(1)  ? GetModuleHandle(NULL) : (HINSTANCE) hb_parnl(1) )   ,
                               (LPDLGTEMPLATE) hb_parc(2)  ,
                               (ISNIL(3) ?  NULL : (HWND) hb_parnl(3) )        ,
                               (DLGPROC) hb_parnl(4)
                             ));
}


//----------------------------------------------------------------------------

HB_FUNC( _CREATEDIALOG )
{

  hb_retnl( (ULONG) CreateDialog( (ISNIL(1)  ? GetModuleHandle(NULL) : (HINSTANCE) hb_parnl(1) )  ,
                                  (hb_parinfo(2)==HB_IT_STRING ? hb_parc(2) : MAKEINTRESOURCE( (WORD) hb_parni(2))) ,
                                  (ISNIL(3) ?  NULL : (HWND) hb_parnl(3) )        ,
                                  (DLGPROC) hb_parnl(4)
                                ) );
}


//----------------------------------------------------------------------------

HB_FUNC( _CREATEDIALOGINDIRECT )
{
  hb_retnl(
   (ULONG) CreateDialogIndirect(
            (ISNIL(1)  ? GetModuleHandle(NULL) : (HINSTANCE) hb_parnl(1) )   ,
            (LPDLGTEMPLATE) hb_parc(2) ,
            (ISNIL(3) ?  NULL : (HWND) hb_parnl(3) )        ,
            (DLGPROC) hb_parnl(4)
          ));
}


//-----------------------------------------------------------------------------

// Create dynamic dialog from the Harbour array

HB_FUNC( _MAKEDLGTEMPLATE )

{
   WORD *p, *pdlgtemplate ;
   WORD  nItems = ( WORD ) hb_parni( 1, 4 ) ;
   int   i, nchar ;
   DWORD lStyle ;

   // Parameters: 12 arrays
   // 1 for DLG template
   // 11 for item properties

   pdlgtemplate = p = (PWORD) LocalAlloc (LPTR, 65534)  ; // 64k allow to build up to 255 items on the dialog

   //---------------

    lStyle = hb_parnl(1,3) ;

    // start to fill in the dlgtemplate information.  addressing by WORDs

    *p++ = 1                   ; // version
    *p++ = 0xFFFF                   ; // signature
    *p++ = LOWORD ( hb_parnl(1,1) ) ; // Help Id
    *p++ = HIWORD ( hb_parnl(1,1) ) ;

    *p++ = LOWORD ( hb_parnl(1,2) ) ; // ext. style
    *p++ = HIWORD ( hb_parnl(1,2) ) ;

    *p++ = LOWORD (lStyle)          ;
    *p++ = HIWORD (lStyle)          ;

    *p++ = (WORD)   nItems        ;  // NumberOfItems
    *p++ = (short)  hb_parni(1,5) ;  // x
    *p++ = (short)  hb_parni(1,6) ;  // y
    *p++ = (short)  hb_parni(1,7) ;  // cx
    *p++ = (short)  hb_parni(1,8) ;  // cy
    *p++ = (short)  0             ;  // Menu (ignored for now.)
    *p++ = (short)  0x00          ;  // Class also ignored

    if ( hb_parinfa(1,11) == HB_IT_STRING ) {
        nchar = nCopyAnsiToWideChar( p, TEXT( (LPTSTR) hb_parc(1,11) ) ) ;
        p += nchar   ;
      }
    else
      *p++ =0 ;

    // add in the wPointSize and szFontName here iff the DS_SETFONT bit on

    if ( (lStyle & DS_SETFONT ) ) {
      *p++ = (short) hb_parni(1,12) ;
      *p++ = (short) hb_parni(1,13) ;
      *p++ = (short) hb_parni(1,14) ;

      nchar = nCopyAnsiToWideChar( p, TEXT( (LPTSTR) hb_parc(1,15) ) ) ;
      p += nchar ;

    } ;

    //---------------
    // Now, for the items

   for ( i = 1 ; i <= nItems ; i++ ) {

      // make sure each item starts on a DWORD boundary
      p = lpwAlign (p) ;


      *p++ = LOWORD ( hb_parnl(2,i) ) ;    // help id
      *p++ = HIWORD ( hb_parnl(2,i) ) ;

      *p++ = LOWORD ( hb_parnl(3,i) ) ; // ext. style
      *p++ = HIWORD ( hb_parnl(3,i) ) ;

      *p++ = LOWORD ( hb_parnl(4,i) ) ; // style
      *p++ = HIWORD ( hb_parnl(4,i) ) ;

      *p++ = (short)  hb_parni(5,i)   ;  // x
      *p++ = (short)  hb_parni(6,i)   ;  // y
      *p++ = (short)  hb_parni(7,i)   ;  // cx
      *p++ = (short)  hb_parni(8,i)   ;  // cy

      *p++ = LOWORD ( hb_parnl(9,i) ) ;  // id
      *p++ = HIWORD ( hb_parnl(9,i) ) ;  // id   // 0;

      if ( hb_parinfa(10,i) == HB_IT_STRING ) {
          nchar = nCopyAnsiToWideChar(p, TEXT ( (LPTSTR) hb_parc(10,i)) ) ; // class
          p += nchar ;
         }
      else
         {
         *p++ = 0xFFFF ;
         *p++ = (WORD) hb_parni(10,i) ;
         }

      if ( hb_parinfa(11,i) == HB_IT_STRING ) {
         nchar = nCopyAnsiToWideChar(p, (LPSTR) hb_parc(11,i) ) ;  // text
         p += nchar ;
         }
      else
         {
         *p++ = 0xFFFF ;
         *p++ = (WORD) hb_parni(11,i) ;
         }


      *p++ = 0x00 ;  // extras ( in array 12 )


    } ;
    p = lpwAlign (p)  ;


    hb_retclen( (LPSTR) pdlgtemplate, ( (ULONG) p - (ULONG) pdlgtemplate ) ) ;

    LocalFree (LocalHandle (pdlgtemplate) ) ;

}

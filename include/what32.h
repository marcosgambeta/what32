// WHAT32
// macros
// Copyright (c) 2025 Marcos Antonio Gambeta (marcosgambeta@outlook.com)

#define w32_par_BOOL(n) (BOOL) hb_parl(n)
#define w32_par_COLORREF(n) (COLORREF) hb_parnl(n)
#define w32_par_DWORD(n) (DWORD) hb_parnl(n)
#define w32_par_HANDLE(n) (HANDLE) hb_parnl(n)
#define w32_par_HBITMAP(n) (HBITMAP) hb_parnl(n)
#define w32_par_HBRUSH(n) (HBRUSH) hb_parnl(n)
#define w32_par_HCURSOR(n) (HCURSOR) hb_parnl(n)
#define w32_par_HDC(n) (HDC) hb_parnl(n)
#define w32_par_HGDIOBJ(n) (HGDIOBJ) hb_parnl(n)
#define w32_par_HINSTANCE(n) (HINSTANCE) hb_parnl(n)
#define w32_par_HRGN(n) (HRGN) hb_parnl(n)
#define w32_par_HWND(n) (HWND) hb_parnl(n)
#define w32_par_UINT(n) (UINT) hb_parni(n)
#define w32_par_LPCSTR(n) (LPCSTR) hb_parcx(n)

#define w32_ret_BOOL(x) hb_retl(x)
#define w32_ret_DWORD(x) hb_retnl(x)
#define w32_ret_HWND(x) hb_retnl(x)
#define w32_ret_UINT(x) hb_retni(x)

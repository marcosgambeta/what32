
// WHAT32.LIB
// disk, directory and file functions

#define HB_OS_WIN_USED
// #define _WIN32_WINNT   0x0500

#if (defined(__WATCOMC__) || defined(__GNUC__))
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0500
#endif

#include <windows.h>
#include "hbapi.h"

#include "what32.h"

#if defined(__DMC__)
WINBASEAPI
DWORD
WINAPI
GetLongPathName(LPCSTR lpszLongPath, LPSTR lpszShortPath, DWORD cchBuffer);
#endif
//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetLogicalDrives( VOID );

HB_FUNC(GETLOGICALDRIVES)
{
  w32_ret_DWORD((LONG)GetLogicalDrives());
}

//-----------------------------------------------------------------------------
// WINBASEAPI UINT WINAPI GetDriveTypeA( IN LPCSTR lpRootPathName );

HB_FUNC(GETDRIVETYPE)
{
  w32_ret_UINT(GetDriveType(w32_par_LPCSTR(1)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI UINT WINAPI GetSystemDirectoryA( OUT LPSTR lpBuffer, IN UINT uSize );

// modified
// GetSystemDirectory() -> cDir

HB_FUNC(GETSYSTEMDIRECTORY)
{

  char szBuffer[MAX_PATH + 1] = {0};
  GetSystemDirectory(szBuffer, MAX_PATH);
  hb_retc(szBuffer);
}

//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetTempPathA( IN DWORD nBufferLength, OUT LPSTR lpBuffer );

// modified
// GetTempPath() -> cPath

HB_FUNC(GETTEMPPATH)
{
  char szBuffer[MAX_PATH + 1] = {0};
  GetTempPath(MAX_PATH, szBuffer);
  hb_retc(szBuffer);
}

//-----------------------------------------------------------------------------
// WINBASEAPI UINT WINAPI GetTempFileNameA( IN LPCSTR lpPathName, IN LPCSTR lpPrefixString, IN UINT uUnique, OUT LPSTR
// lpTempFileName );

// modified
// GetTempFileName(cPath,cPrefix,nUnique) -> cFileName

HB_FUNC(GETTEMPFILENAME)
{
  char cPath[MAX_PATH] = {0};

  GetTempFileName(w32_par_LPCSTR(1), w32_par_LPCSTR(2), (UINT)(ISNIL(3) ? 0 : hb_parni(3)), (LPSTR)cPath);
  hb_retc(cPath);
}

//-----------------------------------------------------------------------------
// WINBASEAPI UINT WINAPI GetWindowsDirectoryA( OUT LPSTR lpBuffer, IN UINT uSize );

HB_FUNC(GETWINDOWSDIRECTORY)
{
  char szBuffer[MAX_PATH + 1] = {0};
  GetWindowsDirectory(szBuffer, MAX_PATH);
  hb_retc(szBuffer);
}

//-----------------------------------------------------------------------------
// WINBASEAPI UINT WINAPI GetSystemWindowsDirectoryA( OUT LPSTR lpBuffer, IN UINT uSize );

/*
// NT only
#if (_WIN32_WINNT >= 0x0500)

HB_FUNC( GETSYSTEMWINDOWSDIRECTORY )
{
   char cPath[ MAX_PATH +1 ] = {0};

   GetSystemWindowsDirectory( (LPSTR) cPath, MAX_PATH ) ;

   hb_retc( cPath );
}
#endif
*/

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI SetCurrentDirectoryA( IN LPCSTR lpPathName );

HB_FUNC(SETCURRENTDIRECTORY)
{

  w32_ret_BOOL(SetCurrentDirectory(w32_par_LPCSTR(1)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetCurrentDirectoryA( IN DWORD nBufferLength, OUT LPSTR lpBuffer );

HB_FUNC(GETCURRENTDIRECTORY)
{
  char cPath[MAX_PATH + 1] = {0};
  GetCurrentDirectory(MAX_PATH, (LPSTR)cPath);
  hb_retc(cPath);
}

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI SetFileAttributesA( IN LPCSTR lpFileName, IN DWORD dwFileAttributes );

HB_FUNC(SETFILEATTRIBUTES)
{
  w32_ret_BOOL(SetFileAttributes(w32_par_LPCSTR(1), w32_par_DWORD(2)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetFileAttributesA( IN LPCSTR lpFileName );

HB_FUNC(GETFILEATTRIBUTES)
{
  w32_ret_DWORD((LONG)GetFileAttributes(w32_par_LPCSTR(1)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI DeleteFileA( IN LPCSTR lpFileName );

HB_FUNC(DELETEFILE)
{
  w32_ret_BOOL(DeleteFile(w32_par_LPCSTR(1)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI SetVolumeLabelA( IN LPCSTR lpRootPathName, IN LPCSTR lpVolumeName );

HB_FUNC(SETVOLUMELABEL)
{
  w32_ret_BOOL(SetVolumeLabel(w32_par_LPCSTR(1), w32_par_LPCSTR(2)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI CreateDirectoryA( IN LPCSTR lpPathName, IN LPSECURITY_ATTRIBUTES lpSecurityAttributes );

HB_FUNC(CREATEDIRECTORY)
{
  SECURITY_ATTRIBUTES *sa = NULL;

  if (ISCHAR(2))
    sa = (SECURITY_ATTRIBUTES *)hb_param(2, HB_IT_STRING)->item.asString.value;

  w32_ret_BOOL(CreateDirectoryA(w32_par_LPCSTR(1), sa));
}

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI CreateDirectoryExA( IN LPCSTR lpTemplateDirectory, IN LPCSTR lpNewDirectory, IN
// LPSECURITY_ATTRIBUTES lpSecurityAttributes );

/*

HB_FUNC( CREATEDIRECTORYEX )
{
   LPSECURITY_ATTRIBUTES lpSecurityAttributes ;

   // Your code goes here

   hb_retl( CreateDirectoryExA( (LPCSTR) hb_parcx( 1 ),
                                (LPCSTR) hb_parcx( 2 ),
                                lpSecurityAttributes
                                ) ) ;
}

*/

//-----------------------------------------------------------------------------
// WINBASEAPI BOOL WINAPI RemoveDirectoryA( IN LPCSTR lpPathName );

HB_FUNC(REMOVEDIRECTORY)
{
  w32_ret_BOOL(RemoveDirectory(w32_par_LPCSTR(1)));
}

//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetFullPathNameA( IN LPCSTR lpFileName, IN DWORD nBufferLength, OUT LPSTR lpBuffer, OUT LPSTR
// *lpFilePart );

// See MSDN first

HB_FUNC(GETFULLPATHNAME)
{
  char *szBuffRet = NULL;
  char buffer[MAX_PATH + 1] = {0};
  const char *szIn = hb_parcx(1);
  // DWORD dwSize  = hb_parnl( 2 );
  DWORD dwReq;
  dwReq = GetFullPathName((LPCSTR)szIn, MAX_PATH, (LPSTR)buffer, &szBuffRet);
  w32_ret_DWORD(dwReq);
  hb_storc(szBuffRet, 4);
  hb_storc(buffer, 3);
}

//  No prototype ?

//-----------------------------------------------------------------------------
// GetVolumeFileName(szPath,@szPathReturn)
// WINBASEAPI BOOL WINAPI GetVolumePathNameA( LPCSTR lpszFileName, LPSTR lpszVolumePathName, DWORD cchBufferLength );
// #if (_WIN32_WINNT >= 0x0500)

HB_FUNC(GETVOLUMEPATHNAME)
{
  typedef BOOL(WINAPI * P_GVPN)(LPCTSTR, LPTSTR, DWORD);
  BOOL bResult = FALSE;
  char buffer[MAX_PATH + 1] = {0};
  P_GVPN pGVPN;
  pGVPN = (P_GVPN)GetProcAddress(GetModuleHandle("kernel32.dll"), "GetVolumePathNameA");
  if (pGVPN) {
    bResult = pGVPN((LPCSTR)hb_parcx(1), buffer, MAX_PATH);
  }
  hb_retl(bResult);
  if (ISBYREF(2)) {
    hb_storc(buffer, 2);
  }
}

// #endif

//-----------------------------------------------------------------------------

HB_FUNC(GETSHORTPATHNAME)
{
  char buffer[MAX_PATH + 1] = {0};
  int iRet;

  iRet = GetShortPathName(hb_parcx(1), buffer, MAX_PATH);
  hb_storc(buffer, 2);
  hb_stornl(iRet, 3);
}

//-----------------------------------------------------------------------------
// WINBASEAPI DWORD WINAPI GetLongPathNameA( IN LPCSTR lpszShortPath, OUT LPSTR lpszLongPath, IN DWORD cchBuffer );

HB_FUNC(GETLONGPATHNAME)
{
  w32_ret_DWORD((LONG)GetLongPathName(w32_par_LPCSTR(1), (LPSTR)hb_parcx(2), w32_par_DWORD(3)));
}

//-----------------------------------------------------------------------------

/*
BOOL GetVolumeInformation(
  LPCTSTR lpRootPathName,        // address of root directory of the
                                 // file system
  LPTSTR lpVolumeNameBuffer,     // address of name of the volume
  DWORD nVolumeNameSize,         // length of lpVolumeNameBuffer
  LPDWORD lpVolumeSerialNumber,  // address of volume serial number
  LPDWORD lpMaximumComponentLength,
                                 // address of system's maximum
                                 // filename length
  LPDWORD lpFileSystemFlags,     // address of file system flags
  LPTSTR lpFileSystemNameBuffer, // address of name of file system
  DWORD nFileSystemNameSize      // length of lpFileSystemNameBuffer
);
*/

// Syntax:
// GetVolumeInformation([cPath],[@cVolName],[@nSerNum],[@nMaxName],[@nFlags],[@cFATName] )

HB_FUNC(GETVOLUMEINFORMATION)
{
  char *VolumeNameBuffer = (char *)hb_xgrab(MAX_PATH);
  DWORD VolumeSerialNumber;
  DWORD MaximumComponentLength;
  DWORD FileSystemFlags;
  char *FileSystemNameBuffer = (char *)hb_xgrab(MAX_PATH);
  BOOL bRet;

  bRet = GetVolumeInformation(ISNIL(1) ? NULL : (LPCTSTR)hb_parcx(1), (LPTSTR)VolumeNameBuffer, MAX_PATH,
                              &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags,
                              (LPTSTR)FileSystemNameBuffer, MAX_PATH);
  if (bRet) {
    if (ISBYREF(2))
      hb_storc((char *)VolumeNameBuffer, 2);
    if (ISBYREF(3))
      hb_stornl((LONG)VolumeSerialNumber, 3);
    if (ISBYREF(4))
      hb_stornl((LONG)MaximumComponentLength, 4);
    if (ISBYREF(5))
      hb_stornl((LONG)FileSystemFlags, 5);
    if (ISBYREF(6))
      hb_storc((char *)FileSystemNameBuffer, 6);
  }

  hb_retl(bRet);
  hb_xfree(VolumeNameBuffer);
  hb_xfree(FileSystemNameBuffer);
}

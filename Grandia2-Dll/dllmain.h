#include <windows.h>
#include <ddraw.h>


typedef HWND(_stdcall* type_CreateWindowEx)(DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
typedef HRESULT(_stdcall* type_DirectDrawCreate) (GUID * guid, IDirectDraw ** directDrawObject, IUnknown * pUnknown);
typedef HRESULT(_stdcall* type_SetDisplayMode) (IDirectDraw * _this, DWORD width, DWORD height, DWORD bpp, DWORD refresh, DWORD flags);
typedef DWORD(_stdcall* type_GetLogicalDrives) ();

HWND _stdcall ProxyCreateWindowEx (DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
HRESULT _stdcall ProxyDirectDrawCreate (GUID * guid, IDirectDraw ** directDrawObject, IUnknown * pUnknown);
HRESULT _stdcall ProxySetDisplayMode (IDirectDraw * _this, DWORD width, DWORD height, DWORD bpp, DWORD refresh, DWORD flags);
DWORD _stdcall ProxyGetLogicalDrives();

HRESULT InitHooks();
void PatchNoCD();
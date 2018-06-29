#include <windows.h>

typedef HWND (_stdcall* type_CreateWindowEx) (DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
typedef DWORD (_stdcall* type_GetLogicalDrives) ();

HWND _stdcall ProxyCreateWindowEx (DWORD, LPCTSTR, LPCTSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID);
DWORD _stdcall ProxyGetLogicalDrives ();

HRESULT InitHooks ();
void PatchNoCD ();
void ResolutionPatch ();
void WriteByte (void * destination, unsigned char * byte);
void WriteWord (void * destination, unsigned short * word);
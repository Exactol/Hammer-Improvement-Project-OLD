// HammerSyncInjected.cpp : Defines the initialization routines for the DLL.
//
#include "stdafx.h"
#include "Resource.h"
#include "HammerSyncInjected.h"
#include "SyncMenu.h"
#include "HIPToolBarButton.h"
#include <winuser.h>
#include "HIPWindow.h"

//tuple<LPWSTR, vector<LPWSTR>> Menus = {L"Debug", vector<LPWSTR>{L"Call Function", L"Test"}};

#ifdef _DEBUG
	#define new DEBUG_NEW
	bool debugEnabled = true;
#else
	bool debugEnabled = false;
#endif

#define CheckItem(id, uFlags) CMenu::FromHandle(GetMenu(hWnd))->CheckMenuItem(id, uFlags);
#define EnableItem(id, uFlags) CMenu::FromHandle(GetMenu(hWnd))->EnableMenuItem(id, uFlags);
//#define IDB_COMPILEPAL 1007


HWND hWnd;
CToolBar hammerToolbar;
HMENU hMenuHammer;
SyncMenu smDebug;

int hammerTopToolbarID = 0;

//Default hammer toolbar styles
const DWORD dwDefStyles = WS_CHILD | WS_VISIBLE | CBRS_TOP;

//CMenu *cSyncMenu;
long OldWndProc;


//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

// CHammerSyncInjectedApp

BEGIN_MESSAGE_MAP(CHammerSyncInjectedApp, CWinApp)
END_MESSAGE_MAP()


// CHammerSyncInjectedApp construction

CHammerSyncInjectedApp::CHammerSyncInjectedApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CHammerSyncInjectedApp::~CHammerSyncInjectedApp()
{
	
}
// The one and only CHammerSyncInjectedApp object

CHammerSyncInjectedApp theApp;
//HIPWindow *hipSettings;



LRESULT CALLBACK SyncWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case(WM_COMMAND):
			switch(wParam)
			{
				case(ID_DEBUG_CALLFUNCTION):
					Utils::PrintError(L"Source: WndProc");
					break;

				case(ID_HAMMERSYNC_DEBUGENABLE):
					//Acts as toggle
					debugEnabled = !debugEnabled;
					
					if (debugEnabled)
					{
						CheckItem(ID_HAMMERSYNC_DEBUGENABLE, MF_CHECKED);

						smDebug.Create(hMenuHammer);
						DrawMenuBar(hWnd);
					} else
					{
						CheckItem(ID_HAMMERSYNC_DEBUGENABLE, MF_UNCHECKED);
						smDebug.Destroy(hWnd);
						DrawMenuBar(hWnd);
					}
					break;

				case(ID_HAMMERSYNC_SETTINGS):
					{
/*					HINSTANCE hInstance = (HINSTANCE)::GetModuleHandle(NULL);
					HWND hwndButton = CreateWindow(
						L"MDIClient",
						L"Ok",
						WS_VISIBLE | WS_CHILD,
						10,
						10,
						75,
						24,
						hWnd,
						NULL, hInstance, NULL);				*/		
					}
					break;

				case(ID_HAMMERSYNC_EXIT):
					break;
			}
			break;
		case(WM_INITMENU):
			//CMenu::FromHandle(GetMenu(hWnd))->EnableMenuItem(ID_DEBUG_CALLFUNCTION, MF_ENABLED | MF_BYCOMMAND);
			break;
			//return 0;
			
		//Makes our menu items work, but breaks recent items menu. Hammer disables our menus by default, but blocking that command seems to be a workaround
		case(WM_INITMENUPOPUP):
			//EnableItem(ID_DEBUG_CALLFUNCTION, MF_ENABLED);
			//break;
			//TODO figure out a less hacky workaround. Creating inside subclassed message handler could work?
			return 0;	
	}
	return CallWindowProc((WNDPROC)OldWndProc, hwnd, message, wParam, lParam);
}

bool keepMakingToolbars = true;

//Enumerate through child hWnds
BOOL CALLBACK GetChildWndCallback(HWND hWndChild, LPARAM lParam)
{
	if (hWndChild != NULL)
	{
		//Get class name
		LPWSTR className = new TCHAR[MAX_CLASS_NAME];
		GetClassName(hWndChild, className, MAX_CLASS_NAME);


		//convert to wstring becuase LPWSTR doesnt seem to want to compare correctly (probably because of extra space in the buffer)
		std::wstring wClassName = std::wstring(className);

		//Kindof works, the target toolbar is the first one that appears, but this is a hacky implementation TODO do something cleaner
		if(wClassName == L"ToolbarWindow32")
		{
			if(keepMakingToolbars)
			{
				HIPToolBarButtonSeperator::Append(hWndChild);

				HIPToolBarButton compilePalBtn(IDB_COMPILEPAL, STD_FILENEW, TBSTATE_ENABLED, TBSTYLE_BUTTON);
				compilePalBtn.AppendBitmap(hWndChild);
				compilePalBtn.Append(hWndChild);			
				keepMakingToolbars = false;
			}
		}

		/////////Gets top control bar
		//Check if hwnd is handle for toolbar
		//if(wClassName == L"AfxControlBar140s")
		//{
		//	//Get window's style
		//	long winStyle = GetWindowLongPtr(hWndChild, GWL_STYLE);

		//	//Check if the winstyle contains align top style
		//	if (winStyle & CBRS_ALIGN_TOP)
		//		hammerTopToolbarID = GetDlgCtrlID(hWndChild);
		//	
		//}
		
		//OutputDebugString(className);
		//OutputDebugString(L"\n");
	}

	
	return TRUE;
}


// CHammerSyncInjectedApp initialization
BOOL CHammerSyncInjectedApp::InitInstance()
{
	CWinApp::InitInstance();
	//this->m_pszAppName = L"HIP";
	

	//Get handle to hammer window
	hWnd = FindWindow(L"VALVEWORLDCRAFT", NULL);

	//DWORD ThreadId;
	//GetWindowThreadProcessId(hWnd, &ThreadId);
	//threadHandle = OpenThread(THREAD_SET_INFORMATION, TRUE, ThreadId);
	//if (threadHandle == NULL)
	//{
	//	Utils::PrintError(L"Thread handle is null");

	//	DWORD errorMessageID = GetLastError();
	//	LPSTR messageBuffer = nullptr;
	//	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	//								 NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	//	std::string message(messageBuffer, size);
	//	OutputDebugString((LPWSTR)message.c_str());
	//}

	//Enum child windows to get the top toolbar ID
	EnumChildWindows(hWnd, GetChildWndCallback, NULL);

	CWnd *hammer = CWnd::FromHandle(hWnd);
	CMDIFrameWnd *hammerMainFrm = (CMDIFrameWnd*)CWnd::FromHandle(hWnd);
	//CFrameWnd *hammerFrm = (CFrameWnd*)CWnd::FromHandle(hWnd);
	
	//HIPWindow *hipSettings = new HIPWindow();
	////hipSettings->AssertValid();
	//theApp.m_pMainWnd = hipSettings;
	//hipSettings->ShowWindow(SW_SHOW);
	//hipSettings->UpdateWindow();
	////system("pause");			


	//Subclass hammer window and reroute messages to SyncWndProc TODO rename to HIP
	OldWndProc = SetWindowLong(hWnd, GWL_WNDPROC, (long)SyncWndProc);
	
	//Get Hammer's menu
	hMenuHammer = GetMenu(hWnd);
	
	CreateMenus();
	return TRUE;	
}

void CHammerSyncInjectedApp::CreateMenus()
{
	//create main HammerSync menus
	SyncSubMenu ssmSettings(ID_HAMMERSYNC_SETTINGS, L"Settings");
	SyncSubMenu ssmDebug(ID_HAMMERSYNC_DEBUGENABLE, L"EnableDebug", MF_STRING | MF_CHECKED);
	SyncSubMenu ssmSeperator(NULL, NULL, MF_SEPARATOR);
	SyncSubMenu ssmExit(ID_HAMMERSYNC_EXIT, L"Exit HammerSync");
	SyncMenu smHammerSync(L"HammerSync", ssmSettings, ssmDebug, ssmSeperator, ssmExit);
	smHammerSync.Create(hMenuHammer);

	//Create debug menu, but only show if debug has been enabled
	SyncSubMenu ssmCallFunction(ID_DEBUG_CALLFUNCTION, L"Call Function");
	smDebug = SyncMenu(L"Debug", ssmCallFunction);
	if (debugEnabled)
	{
		smDebug.Create(hMenuHammer);
		CheckItem(ID_HAMMERSYNC_DEBUGENABLE, MF_CHECKED);
	}

	DrawMenuBar(hWnd);
}

void CHammerSyncInjectedApp::CreateToolbars(HWND hWndparent)
{
	HIPToolBarButtonSeperator::Append(hWndparent);

	HIPToolBarButton compilePalBtn(IDB_COMPILEPAL, STD_FILENEW, TBSTATE_ENABLED, TBSTYLE_BUTTON);
	compilePalBtn.AppendBitmap(hWndparent);
	compilePalBtn.Append(hWndparent);

	//HIPToolBarButton testBtn(31, STD_FILENEW, TBSTATE_ENABLED, TBSTYLE_BUTTON, false);
	//testBtn.Append(hWndparent);
	
	//SendMessage(hWndparent, TB_AUTOSIZE, 0, 0);
	
	//Redraws the window so the new toolbar items are added
	//CFrameWnd *hammer = (CFrameWnd*)CWnd::FromHandle(hWnd);
	//hammer->DockControlBar(nullptr, nullptr, NULL);
	//hammer->RedrawWindow();
	//hammer->InvalidateRect(NULL);
	//hammer->RedrawWindow(NULL, NULL, RDW_FRAME | RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	//hammer->RedrawWindow(NULL, NULL, RDW_NOFRAME | RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
	//hammer->UpdateWindow();
	//RedrawWindow(hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

//TODO read/write in main window thread process
	//LPDWORD winId = nullptr;
	//GetWindowThreadProcessId(hWndparent, winId);
	//if (winId != nullptr)
	//{
	//	HANDLE proc = OpenProcess(PROCESS_ALL_ACCESS, true, *winId);
	//	LPVOID addr = VirtualAllocEx(proc, NULL, sizeof(TBBUTTON), MEM_RESERVE | MEM_COMMIT, NULL);
	//	LPVOID bfr;

	//	TCHAR buffer[512];
	//	ReadProcessMemory(proc, addr, &buffer, sizeof(buffer), 0);
	//	OutputDebugString(buffer);
	//}


	//int btnCount = (int)SendMessage(hWndparent, TB_BUTTONCOUNT, 0, 0);


//////////////////////OLD way of adding toolbar buttons
	//HIMAGELIST list = (HIMAGELIST)SendMessage(hWndparent, TB_GETIMAGELIST,0, 0);

	//CBitmap bit;
	//bit.LoadBitmap(IDB_COMPILEPAL);
	//BITMAP bmp;
	//bit.GetBitmap(&bmp);

	//HBITMAP hbmMask = CreateCompatibleBitmap(GetDC(NULL), bmp.bmWidth, bmp.bmHeight);
	//ICONINFO ii = {0};
	//ii.fIcon = TRUE;
	//ii.hbmColor = bit;
	//ii.hbmMask = hbmMask;

	//HICON hicon = CreateIconIndirect(&ii);
	//DeleteObject(hbmMask);

	//ImageList_RemoveAll(list);
	//ImageList_AddIcon(list, hicon);
	//

	//SendMessage(hWndparent, TB_SETIMAGELIST, 0, (LPARAM)list);
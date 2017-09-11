// HammerSyncInjected.h : main header file for the HammerSyncInjected DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

//#include "resource.h"		// main symbols


// CHammerSyncInjectedApp
// See HammerSyncInjected.cpp for the implementation of this class
//

class CHammerSyncInjectedApp : public CWinApp
{
public:
	CHammerSyncInjectedApp();
	~CHammerSyncInjectedApp();

public:
	virtual BOOL InitInstance();
	void CreateMenus();
	void CreateToolbars(HWND hWndparent);
	void CreateToolbars();

	DECLARE_MESSAGE_MAP()
};

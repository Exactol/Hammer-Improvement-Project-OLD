// HammerSyncInjected.h : main header file for the HammerSyncInjected DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "HIPToolBarButton.h"

//#include "resource.h"		// main symbols


// CHammerSyncInjectedApp
// See HammerSyncInjected.cpp for the implementation of this class
//

class CHammerSyncInjectedApp : public CWinApp
{
public:
	CHammerSyncInjectedApp();
	~CHammerSyncInjectedApp();

	virtual BOOL InitInstance();
	void CreateMenus();
	void CreateToolbars(HWND hWndparent);
	//void CreateToolbars();

protected:
	//{{AFX_MSG(CHammerSyncInjectedApp)
	//afx_msg void OnCompilePalBtnPressed();
	//afx_msg void OnUpdateCompilePalBtnPressed(CCmdUI* pCmdUI);

	//afx_msg void OnTest1(){};
	//afx_msg void OnUpdateTest1(CCmdUI* pCmdUI);

	//afx_msg void OnTest2(){};
	//afx_msg void OnUpdateTest2(CCmdUI* pCmdUI);

	//afx_msg void OnOverrideBtn();
	//afx_msg void OnOverrideBtnUpdate(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//HIPToolBarButton compilePalBtn;
};

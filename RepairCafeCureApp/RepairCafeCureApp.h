// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// RepairCafeCureApp.h : main header file for the RepairCafeCureApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "CDatabaseConnection.h"


// CRepairCafeCureApp:
// See RepairCafeCureApp.cpp for the implementation of this class
//

class CRepairCafeCureApp : public CWinAppEx
{
private:
		artvabas::rcc::database::CDatabaseConnection m_dbConnection;
		CView* m_pAppView;
		CView* m_pCustomerView;
		CView* m_pWorkorderView;

public:
	CRepairCafeCureApp() noexcept;

private:
	enum ViewType { VIEW_APP, VIEW_CUSTOMER, VIEW_WORKORDER };
	CView* SwitchView(ViewType vtView);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnCustomerView();
	afx_msg void OnAppView();
	afx_msg void OnWorkorderView();
};

extern CRepairCafeCureApp theApp;

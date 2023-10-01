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

// RepairCafeCureAppView.h : interface of the CRepairCafeCureAppView class
//

#pragma once


class CRepairCafeCureAppView : public CFormView
{
protected: // create from serialization only
	CRepairCafeCureAppView() noexcept;
	DECLARE_DYNCREATE(CRepairCafeCureAppView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_REPAIRCAFECUREAPP_FORM };
#endif

// Attributes
public:
	CRepairCafeCureAppDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRepairCafeCureAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in RepairCafeCureAppView.cpp
inline CRepairCafeCureAppDoc* CRepairCafeCureAppView::GetDocument() const
   { return reinterpret_cast<CRepairCafeCureAppDoc*>(m_pDocument); }
#endif


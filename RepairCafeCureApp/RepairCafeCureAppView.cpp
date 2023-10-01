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

// RepairCafeCureAppView.cpp : implementation of the CRepairCafeCureAppView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RepairCafeCureApp.h"
#endif

#include "RepairCafeCureAppDoc.h"
#include "RepairCafeCureAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRepairCafeCureAppView

IMPLEMENT_DYNCREATE(CRepairCafeCureAppView, CFormView)

BEGIN_MESSAGE_MAP(CRepairCafeCureAppView, CFormView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CRepairCafeCureAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CRepairCafeCureAppView construction/destruction

CRepairCafeCureAppView::CRepairCafeCureAppView() noexcept
	: CFormView(IDD_REPAIRCAFECUREAPP_FORM)
{
	// TODO: add construction code here

}

CRepairCafeCureAppView::~CRepairCafeCureAppView()
{
}

void CRepairCafeCureAppView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CRepairCafeCureAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CRepairCafeCureAppView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}


// CRepairCafeCureAppView printing


void CRepairCafeCureAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CRepairCafeCureAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRepairCafeCureAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRepairCafeCureAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CRepairCafeCureAppView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CRepairCafeCureAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CRepairCafeCureAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CRepairCafeCureAppView diagnostics

#ifdef _DEBUG
void CRepairCafeCureAppView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRepairCafeCureAppView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRepairCafeCureAppDoc* CRepairCafeCureAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRepairCafeCureAppDoc)));
	return (CRepairCafeCureAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CRepairCafeCureAppView message handlers

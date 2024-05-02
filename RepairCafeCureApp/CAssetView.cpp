/*
Copyright(C) 2023 / 24  artvabas

This program is free software : you can redistribute it and /or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.If not, see < https://www.gnu.org/licenses/>

To see the license for this source code, please visit :
< https ://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit :
< https ://artvabas.com>
	< https ://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at :
< https ://artvabas.com/contact>

	*/

	/*
	* This file is part of RepairCafeCureApp.
	* File: CAssetView.h, implements class CAssetView
	*
	* This class is the view of the Asset form
	*
	* Target: Windows 10/11 64bit
	* Version: 0.0.1.0 (alpha)
	* Created: 04-11-2023, (dd-mm-yyyy)
	* Updated: 30-04-2024, (dd-mm-yyyy)
	* Creator: artvabasDev / artvabas
	*
	* Description: Database connection class
	* License: GPLv3
	*/

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "RepairCafeCureAppDoc.h"
#include "CAssetView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace artvabas::rcc::ui;

IMPLEMENT_DYNCREATE(CAssetView, CFormView)

CAssetView::CAssetView() noexcept
	: CFormView(IDD_ASSET_FORM)
{}

CAssetView::~CAssetView()
{}

/* message handle binders */
BEGIN_MESSAGE_MAP(CAssetView, CFormView)
	ON_COMMAND(ID_FILE_PRINT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CAssetView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

/* Override methods */

// PreCreateWindow: Modify the Window class or styles here by modifying the CREATESTRUCT cs
// - cs: The create structure
BOOL CAssetView::PreCreateWindow(CREATESTRUCT& cs)
{
	//cs.style |= WS_HSCROLL | WS_VSCROLL;
	return CFormView::PreCreateWindow(cs);
}

// DoDataExchange: Exchange and validate form data between controls and class members.
// - pDX: The data exchange object
void CAssetView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

// OnInitialUpdate: Called after the view is first attached to the document
void CAssetView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
}

// OnPreparePrinting: Prepare the printing process
// - pInfo: The print information
BOOL CAssetView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

// OnBeginPrinting: Initialize the printing process
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

// OnEndPrinting: Clean up after printing
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

// OnPrint: Print the view
// - pDC: The device context
// - pInfo: The print information
void CAssetView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

#ifdef _DEBUG
// AssertValid: Assert the view is valid
void CAssetView::AssertValid() const
{
	CFormView::AssertValid();
}

// Dump: Dump the view
// - dc: The dump context
void CAssetView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif

/* Message handlers methods */

// OnFilePrintPreview: Print preview the view
void CAssetView::OnFilePrintPreview() noexcept
{
	AFXPrintPreview(this);
}

// OnRButtonUp: Handle right button up event
// - nFlags: The flags of the event
// - point: The point where the event occured
void CAssetView::OnRButtonUp(UINT /* nFlags */, CPoint point) noexcept
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

// OnContextMenu: Handle context menu event
// - pWnd: The window that the context menu is for
// - point: The point where the context menu is to be displayed
void CAssetView::OnContextMenu(CWnd* /* pWnd */, CPoint point) noexcept
{
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
}

/* Member methods */
#ifdef _DEBUG
// GetDocument: Get the document of this applicaton
CRepairCafeCureAppDoc* CAssetView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRepairCafeCureAppDoc)));
	return (CRepairCafeCureAppDoc*)m_pDocument;
}
#endif //_DEBUG
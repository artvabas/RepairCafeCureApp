/*
	Copyright (C) 2023  artvabas

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Affero General Public License as published
	by the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Affero General Public License for more details.

	You should have received a copy of the GNU Affero General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>

	To see the license for this source code, please visit:
		<https://github.com/artvabas/RepairCafeCureApp/blob/master/LICENSE.txt>

	For more information, please visit:
		<https://artvabas.com>
		<https://github.com/artvabas/RepairCafeCureApp>

	For contacts, please use the contact form at:
		<https://artvabas.com/contact>

*/

/*
* This file is part of RepairCafeCureApp.
* File: CWorkorderTab.ccp, implementation of the class CWorkorderTab
*
* This class is the view of the workorder tab (IDD_WORKORDER_TAB) of the asset/workorder dialog (CAssetDialog)
* which is the view of the CListCtrl created on the asset/workorder dialog (CAssetDialog)
*
* With this form the user can create a new workorder for the selected asset.
* The user can also view the history of the workorders of the selected asset.
*
* Controls are enabled and disabled depending on the state of the form.
*
* Target: Windows 10/11 64bit
* Version: 1.0.230.0
* Created: 04-11-2023, (dd-mm-yyyy)
* Updated: 10-11-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Database connection class
* License: GPLv3
*/ 

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "CWorkorderTab.h"

using namespace artvabas::rcc::ui::dialogs;
using namespace artvabas::sql;

IMPLEMENT_DYNAMIC(CWorkorderTab, CDialogEx)

CWorkorderTab::CWorkorderTab(CTabCtrlAssetWorkorder* pTabControl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_WORKORDER_TAB, pParent)
	, m_pTabControl(pTabControl)
	, m_uiCustomerID(0)
	, m_uiAssetID(0)
	, m_strCustomerSurname(_T(""))
	, m_strCustomerName(_T(""))
	, m_strAssetDescription(_T(""))
	, m_strAssetModelNumber(_T(""))
	, m_strAssetBrand(_T(""))
	, m_strWorkorderDescription(_T(""))
	, m_strHistoryWorkorderDescription(_T(""))
	, m_strHistoryWorkorderLog(_T(""))
{
	m_pAssetDetailsRecords = &(m_pTabControl->m_assetDetailsRecords);
}

CWorkorderTab::~CWorkorderTab()
{
}

// CWorkorderTab message handlers
BEGIN_MESSAGE_MAP(CWorkorderTab, CDialogEx)
	ON_BN_CLICKED(IDC_WOTAB_CREATE, &CWorkorderTab::OnBnClickedWoTabCreate)
	ON_EN_CHANGE(IDC_WOTAB_DESCRIPTION, &CWorkorderTab::OnEnChangeWoTabDescription)
	ON_NOTIFY(NM_CLICK, IDC_WOTAB_WORKORDERS_HISTORY_LIST, &CWorkorderTab::OnNMClickWoTabWorkordersHistoryList)
END_MESSAGE_MAP()

/// <summary>
/// Called when user changes the workorder description.
/// If the description is empty, the create workorder button is disabled.
/// otherwise the create workorder button is enabled.
/// </summary>
/// <returns></returns>
void CWorkorderTab::OnEnChangeWoTabDescription()
{
	UpdateData(TRUE);
	if (m_strWorkorderDescription.IsEmpty())
	{
		m_btnCreateWorkorder.EnableWindow(FALSE);
	}
	else
	{
		m_btnCreateWorkorder.EnableWindow(TRUE);
	}
}

/// <summary>
/// Called when user clicks the create workorder button.
/// The workorder is created in the database.
/// After the workorder is created, the tab view is changed for receiving another new workorder.
/// </summary>
/// <returns></returns>
void CWorkorderTab::OnBnClickedWoTabCreate()
{
	UpdateData(TRUE);
	/*
	CString strQuery;

	// Build the fields value for the query.
	auto buildFieldValue = [](CString str) -> CString
		{
			CString strResult;
			if (str.IsEmpty())
				return  _T("NULL");
			strResult.Format(_T("N\'%s\'"), static_cast<LPCTSTR>(str));
			return strResult;
		};

	strQuery.Format(_T("INSERT INTO [WORKORDER] ([WORKORDER_ASSET_ID], [WORKORDER_CUSTOMER_ID], [WORKORDER_INVOICE_ID], [WORKORDER_CREATE_DATE], [WORKORDER_CREATE_BY], [WORKORDER_DESCRIPTION], [WORKORDER_RESPONSIBLE], [WORKORDER_STATUS], [WORKORDER_LOG], [WORKORDER_HISTORY]) VALUES (%d, %d, NULL, %s, %s, %s, NULL, %s, NULL, NULL)"),
		m_uiAssetID, m_uiCustomerID,
		static_cast<LPCTSTR>(buildFieldValue(COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")))),
		static_cast<LPCTSTR>(buildFieldValue(theApp.GetSelectedEmployeeName())),
		static_cast<LPCTSTR>(buildFieldValue(m_strWorkorderDescription)),
		static_cast<LPCTSTR>(buildFieldValue(_T("Open"))));

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);

	// Execute the query.
	CSqlNativeAVB sql(theApp.GetDatabaseConnection()->ConnectionString());

	if (!sql.ExecuteQuery(strQuery.GetBuffer()))
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_FAIL);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_INSERT_OK);
	}
	strQuery.ReleaseBuffer();

	*/
	CPrintDialog dlg(FALSE);
	if (dlg.DoModal() == IDOK)
	{
		// Local variables
		typedef unsigned int pixel;
		const pixel pixMargin = 100; // 1 inch margin
		const pixel pixFontHeightHeader = 70;
		const pixel pixFontHeightBody = 50;

		// Lambda functions
		auto TotalTabInPixels = [pixMargin](unsigned int nTotalTabs) -> pixel
			{
				return pixMargin * nTotalTabs;
			};

		auto HeaderTextLineDown = [pixFontHeightHeader](unsigned int nTotalLines) -> pixel
			{
				return pixFontHeightHeader * nTotalLines;
			};

		auto BodyTextLineDown = [pixFontHeightBody](unsigned int nTotalLines) -> pixel
			{
				return pixFontHeightBody * nTotalLines;
			};

		// Set printer settings
		DEVMODEW dm;
		ZeroMemory(&dm, sizeof(dm));
		dm.dmSize = sizeof(dm);
		dm.dmFields = DM_ORIENTATION | DM_PAPERSIZE;
		dm.dmOrientation = DMORIENT_LANDSCAPE;
		dm.dmPaperSize = DMPAPER_A4;
		CDC* pDC = new CDC;
		HDC hDC = CreateDC(dlg.GetDriverName(), dlg.GetDeviceName(), NULL, &dm);
		BOOL c = pDC->Attach(hDC);
		const int nHorRes = pDC->GetDeviceCaps(HORZRES);	// get printable width
		const int nVerRes = pDC->GetDeviceCaps(VERTRES);	// get printable height
		const int nLogPixelsX = pDC->GetDeviceCaps(LOGPIXELSX);	// get device resolution along X
		const int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);	// get device resolution along Y

		// Create printer DC
		//HDC hDC = CreateDC(dlg.GetDriverName(), dlg.GetDeviceName(), NULL, &dm);
		//BOOL c = pDC->Attach(hDC);
		pDC->m_bPrinting = TRUE;

		// Create fonts
		CFont fontPlain;
		VERIFY(fontPlain.CreateFont(
			pixFontHeightHeader, 	  // nHeight
			0,                        // nWidth
			0,                        // nEscapement
			0,                        // nOrientation
			FW_NORMAL,                // nWeight
			FALSE,                    // bItalic
			FALSE,                    // bUnderline
			0,                        // cStrikeOut
			ANSI_CHARSET,             // nCharSet
			OUT_DEFAULT_PRECIS,       // nOutPrecision
			CLIP_DEFAULT_PRECIS,      // nClipPrecision
			DEFAULT_QUALITY,          // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			_T("Cascadia Mono")));    // lpszFacename

		CFont fontBold;
		VERIFY(fontBold.CreateFont(
			pixFontHeightHeader,      // nHeight
			0,                        // nWidth
			0,                        // nEscapement
			0,                        // nOrientation
			FW_BOLD,				  // nWeight
			FALSE,                    // bItalic
			FALSE,                    // bUnderline
			0,                        // cStrikeOut
			ANSI_CHARSET,             // nCharSet
			OUT_DEFAULT_PRECIS,       // nOutPrecision
			CLIP_DEFAULT_PRECIS,      // nClipPrecision
			DEFAULT_QUALITY,          // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			_T("Cascadia Mono")));    // lpszFacename

		CFont fontItalic;
		VERIFY(fontItalic.CreateFont(
			pixFontHeightBody,        // nHeight
			0,                        // nWidth
			0,                        // nEscapement
			0,                        // nOrientation
			FW_NORMAL,                // nWeight
			TRUE,                     // bItalic
			FALSE,                    // bUnderline
			0,                        // cStrikeOut
			ANSI_CHARSET,             // nCharSet
			OUT_DEFAULT_PRECIS,       // nOutPrecision
			CLIP_DEFAULT_PRECIS,      // nClipPrecision
			DEFAULT_QUALITY,          // nQuality
			DEFAULT_PITCH | FF_SWISS, // nPitchAndFamily
			_T("Cascadia Mono")));    // lpszFacename

		CString strTitle;
		BOOL isValid = strTitle.LoadStringW(AFX_IDS_APP_TITLE);
		ASSERT(isValid);

		// Start printing
		DOCINFO di;
		::ZeroMemory(&di, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = strTitle;
		pDC->m_hDC = dlg.GetPrinterDC();


		if (pDC->StartDoc(&di))
		{
			pDC->StartPage();

			// Print border
			CRect rctBorder(0, 0, nHorRes, nVerRes);
			rctBorder.DeflateRect(nLogPixelsX / 2, nLogPixelsY / 2);
			pDC->Draw3dRect(rctBorder, RGB(0, 0, 0), RGB(0, 0, 0));

			// Set print start position
			int nPosX1 = rctBorder.TopLeft().x + 10;
			int nPosY1 = rctBorder.TopLeft().y + 10;

			// Print logo
			CImage imgLogo;
			imgLogo.Load(_T("Screenshot.bmp"));
			imgLogo.StretchBlt(pDC->m_hDC, nPosX1, nPosY1, imgLogo.GetWidth() * 4, imgLogo.GetHeight() * 4, 0, 0, imgLogo.GetWidth(), imgLogo.GetHeight(), SRCCOPY);

			// Calculate new start print position
			nPosY1 += imgLogo.GetHeight() * 4 + 10;

			// Print Header rectangle
			CRect rctHeader(nPosX1 + 60, nPosY1, nPosX1 + (imgLogo.GetWidth() * 4) - 60, nPosY1 + HeaderTextLineDown(2));
			pDC->FillRect(rctHeader, &CBrush(RGB(0, 102, 255)));
			pDC->Draw3dRect(rctHeader, RGB(0, 102, 255), RGB(0, 102, 255));

			// Set font
			CFont* pFont = &fontBold;
			pDC->SelectObject(pFont);

			// Print header text
			pDC->SetTextColor(RGB(255, 255, 255));
			pDC->DrawText(_T(" ") + m_strAssetDescription + _T(": ") + m_strWorkorderDescription + _T("     Workorder: 12345\n") +
				_T(" Model: ") + m_strAssetModelNumber + _T("     Brand: ") + m_strAssetBrand
				, rctHeader, DT_CENTER | DT_TABSTOP);

			auto middleDocBody = (((rctHeader.BottomRight().x - rctHeader.TopLeft().x) + (nPosX1 + 60)) / 2) - TotalTabInPixels(2);

			// Calculate new start print position
			nPosX1 += TotalTabInPixels(1);
			nPosY1 = rctHeader.BottomRight().y + HeaderTextLineDown(1);

			// Set body values
			CString strTextLine1a = _T("Date: ");// +COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
			CString strTextLine1b = _T("Customer: ");// +m_strCustomerSurname + _T(" ") + m_strCustomerName;
			CString strTextLine2a = _T("Employee: ");// +theApp.GetSelectedEmployeeName();
			CString strTextLine2b = _T("Mobile: ");// 06 - 3456789");
			CString strTextLine3 = _T("Phone: ");// 070 - 5647864");
			CString strTextLine4 = _T("Email: ");// zuurtje@zuiker.com");

			// Set font color
			pDC->SetBkColor(RGB(255, 255, 255));
			pDC->SetTextColor(RGB(0, 0, 0));

			// Print body text, SubHeaders
			// Line 1 Date and Custemer
			pDC->TextOut(nPosX1, nPosY1, strTextLine1a);
			pDC->TextOut(nPosX1 + middleDocBody, nPosY1, strTextLine1b);
			//Line 2 Employee and mobile
			pDC->TextOut(nPosX1, nPosY1 + HeaderTextLineDown(1), strTextLine2a);
			pDC->TextOut(nPosX1 + middleDocBody, nPosY1 + HeaderTextLineDown(1), strTextLine2b);
			//Line 3 Phone
			pDC->TextOut(nPosX1 + middleDocBody, nPosY1 + HeaderTextLineDown(2), strTextLine3);
			//Line 4 Email
			pDC->TextOut(nPosX1 + middleDocBody, nPosY1 + HeaderTextLineDown(3), strTextLine4);

			pFont = &fontPlain;
			pDC->SelectObject(pFont);

			CSize textSizeCustomer = pDC->GetTextExtent(strTextLine1b);
			CSize TextSizeEmployee = pDC->GetTextExtent(strTextLine2a);

			// Print body text, Content
			pDC->TextOut(nPosX1 + TextSizeEmployee.cx, nPosY1, COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")));
			pDC->TextOut(nPosX1 + middleDocBody + textSizeCustomer.cx, nPosY1, m_strCustomerSurname + _T(" ") + m_strCustomerName);
			pDC->TextOut(nPosX1 + TextSizeEmployee.cx, nPosY1 + HeaderTextLineDown(1), theApp.GetSelectedEmployeeName());
			pDC->TextOut(nPosX1 + middleDocBody + textSizeCustomer.cx, nPosY1 + HeaderTextLineDown(1), _T("06 - 3456789"));
			pDC->TextOut(nPosX1 + middleDocBody + textSizeCustomer.cx, nPosY1 + HeaderTextLineDown(2), _T("070 - 5647864"));
			pDC->TextOut(nPosX1 + middleDocBody + textSizeCustomer.cx, nPosY1 + HeaderTextLineDown(3), _T("zuurtje@zuiker.com"));

			nPosY1 += HeaderTextLineDown(5);

			pFont = &fontItalic;
			pDC->SelectObject(pFont);

			// Print body text, SubHeaders
			pDC->TextOutW(nPosX1, nPosY1, _T("HUISREGELS"));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De activiteiten van NME Repair worden gratis en op vrijwillige basis uitgevoerd door de"));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aanwezige reparatiedeskundigen."));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Noch de organisatie van het NME Repair, noch de reparateurs zijn aansprakelijk voor schade"));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("als gevolg van verstrekte reparatieadviezen of instructies, voor schade aan de ter reparatie"));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aangeboden voorwerpen, voor gevolgdchade of voor andere schade die het gevolg is van"));
			pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("activiteiten van het NME Repair."));

			/*
			CImage image2;
			image2.Load(_T("Screenshot.bmp"));
			int nWidth2 = image2.GetWidth();
			int nImageHeight2 = image2.GetHeight();
			int nX2 = rect.Width() * 3 / 4 - nWidth2 / 2;
			int nY2 = rect.Height() / 2 + nHeight / 2;

			hdc = image2.GetDC();
			dc1.Attach(hdc);
			dc.StretchBlt(nX2, nY2, nWidth2, nHeight, &dc1, 0, 0, image2.GetWidth(), image2.GetHeight(), SRCCOPY);
			dc1.Detach();
			image2.ReleaseDC();

			CString strText4 = _T("Regel 4");
			CString strText5 = _T("Regel 5");
			CString strText6 = _T("Regel 6");
			dc.TextOut(nX2, nY2 - 50, strText4);
			dc.TextOut(nX2, nY2 - 30, strText5);
			dc.TextOut(nX2, nY2 - 10, strText6);
			*/
		}

		fontPlain.DeleteObject();
		fontBold.DeleteObject();
		fontItalic.DeleteObject();
		pDC->EndPage();
		pDC->EndDoc();
		pDC->Detach();
		ReleaseDC(pDC);
		delete pDC;
	}

	m_pTabControl->ChangeTabView(true);
}

/// <summary>
/// Called when user clicks on a workorder in the workorder history list.
/// The workorder description and log is displayed in the workorder history description and log edit controls.
/// </summary>
/// <returns></returns>
void CWorkorderTab::OnNMClickWoTabWorkordersHistoryList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	if (pNMItemActivate->iItem != -1)
	{
		m_strHistoryWorkorderDescription = m_ctrWorkordersHistoryList.GetItemText(pNMItemActivate->iItem, 6);
		m_strHistoryWorkorderLog = m_ctrWorkordersHistoryList.GetItemText(pNMItemActivate->iItem, 10);
		UpdateData(FALSE);
	}
	*pResult = 0;
}

/* Override methods */

/// <summary>
/// Called when the dialog is initialized.
/// The workorder history list control is initialized.
/// </summary>
/// <returns></returns>
BOOL CWorkorderTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrWorkordersHistoryList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_ctrWorkordersHistoryList.InsertColumn(0, _T("WORKORDER ID"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(1, _T("ASSET ID"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(2, _T("CUSTOMER ID"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(3, _T("INVOICE ID"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(4, _T("CREATION DATE"), LVCFMT_LEFT, 100);
	m_ctrWorkordersHistoryList.InsertColumn(5, _T("CREATED BY"), LVCFMT_LEFT, 200);
	m_ctrWorkordersHistoryList.InsertColumn(6, _T("DESCRIPTION"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(7, _T("RESPOSIBLE"), LVCFMT_LEFT, 200);
	m_ctrWorkordersHistoryList.InsertColumn(8, _T("STATUS"), LVCFMT_LEFT, 100);
	m_ctrWorkordersHistoryList.InsertColumn(9, _T("LOG"), LVCFMT_LEFT, 0);
	m_ctrWorkordersHistoryList.InsertColumn(10, _T("HISTORY"), LVCFMT_LEFT, 0);

	return TRUE;  
}

/// <summary>
/// Called when controls in the dialog are to be exchanged with data.
/// </summary>
/// <param name="pDX">The pDX.</param>
/// <returns></returns>
void CWorkorderTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WOTAB_CUSTOMER_SURNAME, m_strCustomerSurname);
	DDX_Text(pDX, IDC_WOTAB_CUSTOMER_NAME, m_strCustomerName);
	DDX_Text(pDX, IDC_WOTAB_ASSET_DESCRIPTION, m_strAssetDescription);
	DDX_Text(pDX, IDC_WOTAB_ASSET_MODEL_NUMBER, m_strAssetModelNumber);
	DDX_Text(pDX, IDC_WOTAB_ASSET_BRAND, m_strAssetBrand);
	DDX_Text(pDX, IDC_WOTAB_DESCRIPTION, m_strWorkorderDescription);
	DDX_Text(pDX, IDC_WOTAB_WORKORDER_HISTORY_DESCRIPTION, m_strHistoryWorkorderDescription);
	DDX_Text(pDX, IDC_WOTAB_WORKORDER_HISTORY_LOG, m_strHistoryWorkorderLog);
	DDX_Control(pDX, IDC_WOTAB_CREATE, m_btnCreateWorkorder);
	DDX_Control(pDX, IDC_WOTAB_WORKORDERS_HISTORY_LIST, m_ctrWorkordersHistoryList);
}

/* Custom methods */

/// <summary>
/// Initializes the workorder tab with the asset details records.
/// And fills the workorder history list control with the existing workorders of the selected asset.
/// </summary>
/// <returns></returns>
void CWorkorderTab::InitWithAssetDetailsRecords()
{
	m_uiCustomerID = m_pAssetDetailsRecords->m_nAssetCustomerID;
	m_uiAssetID = m_pAssetDetailsRecords->m_nAssetID;
	m_strCustomerSurname = m_pAssetDetailsRecords->m_strCustomerSurname;
	m_strCustomerName = m_pAssetDetailsRecords->m_strCustomerName;
	m_strAssetDescription = m_pAssetDetailsRecords->m_strDescription;
	m_strAssetModelNumber = m_pAssetDetailsRecords->m_strModelNumber;
	m_strAssetBrand = m_pAssetDetailsRecords->m_strBrand;

	m_ctrWorkordersHistoryList.DeleteAllItems();

	int nIndex;			// Index of the list control item.
	int row(0);			// Row of the list control item.
	CString strQuery;

	theApp.SetStatusBarText(IDS_STATUSBAR_LOADING);
	
	CRecordset* rs = new CRecordset();
	strQuery.Format(_T("SELECT WORKORDER.*, WORKORDER_ASSET_ID AS Expr1, WORKORDER_CUSTOMER_ID AS Expr2 FROM WORKORDER WHERE(WORKORDER_ASSET_ID = N\'%d\') AND (WORKORDER_CUSTOMER_ID = N\'%d\')"),
		m_uiAssetID, m_uiCustomerID);

	if (theApp.GetDatabaseConnection()->OpenQuery(rs, strQuery))
	{
		// Fill the existing customers list control with the found customers from the database.
		while (!rs->IsEOF())
		{
			CString strValue = _T("");
			rs->GetFieldValue(_T("WORKORDER_ID"), strValue);
			nIndex = m_ctrWorkordersHistoryList.InsertItem(row++, strValue);

			rs->GetFieldValue(_T("WORKORDER_ASSET_ID"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 1, strValue);

			rs->GetFieldValue(_T("WORKORDER_CUSTOMER_ID"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 2, strValue);

			rs->GetFieldValue(_T("WORKORDER_INVOICE_ID"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 3, strValue);

			rs->GetFieldValue(_T("WORKORDER_CREATE_DATE"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 4, strValue);

			rs->GetFieldValue(_T("WORKORDER_CREATE_BY"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 5, strValue);

			rs->GetFieldValue(_T("WORKORDER_DESCRIPTION"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 6, strValue);

			rs->GetFieldValue(_T("WORKORDER_RESPONSIBLE"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 7, strValue);

			rs->GetFieldValue(_T("WORKORDER_STATUS"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 8, strValue);

			rs->GetFieldValue(_T("WORKORDER_LOG"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 9, strValue);

			rs->GetFieldValue(_T("WORKORDER_HISTORY"), strValue);
			m_ctrWorkordersHistoryList.SetItemText(nIndex, 10, strValue);

			rs->MoveNext();
		}
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_OK);
	}
	else
	{
		theApp.SetStatusBarText(IDS_STATUSBAR_SELECT_FAIL);
	}
	theApp.GetDatabaseConnection()->CloseQuery(rs);
	delete rs;
	UpdateData(FALSE);
}

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

	PrintReceiptAndWorkorder();

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

void CWorkorderTab::PrintReceiptAndWorkorder()
{
	// Set printer settings
	CPrintDialog dlg(false);
	dlg.m_pd.hDevMode = ::GlobalAlloc(GHND, sizeof(DEVMODE));

	ASSERT(dlg.m_pd.hDevMode != NULL);

	if (dlg.m_pd.hDevMode != NULL)
	{
		DEVMODE* devMode = (DEVMODE*)GlobalLock(dlg.m_pd.hDevMode);
		devMode->dmSize = sizeof(DEVMODE);
		devMode->dmOrientation = DMORIENT_LANDSCAPE;
		devMode->dmFields = DM_ORIENTATION;
		GlobalUnlock(devMode);
	}

	if (dlg.DoModal() == IDOK)
	{
		// Local variables
		typedef unsigned int pixel;
		const pixel pixMargin = 100; // 1 inch margin
		const pixel pixFontHeightHeader = 110;
		const pixel pixFontHeightBody = 70;

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

		// Create printer device context
		CDC* pDC = new CDC;
		// Attach the printer device context handle to the printer device context
		HDC hDC = CreateDCW(dlg.GetDriverName(), dlg.GetDeviceName(), NULL, dlg.GetDevMode());

		// Attach the printer device context handle to the printer device context, init pDC
		if (pDC->Attach(hDC))
		{
			// Lambda functions for calculating pixels movements
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

			// Get printer device resolutions
			const int nHorRes = pDC->GetDeviceCaps(HORZRES);	// get printable width
			const int nVerRes = pDC->GetDeviceCaps(VERTRES);	// get printable height
			const int nLogPixelsX = pDC->GetDeviceCaps(LOGPIXELSX);	// get device resolution along X
			const int nLogPixelsY = pDC->GetDeviceCaps(LOGPIXELSY);	// get device resolution along Y

			// Set printer title
			CString strTitle = _T("RCCA - Combibon");

			DOCINFO di;
			::ZeroMemory(&di, sizeof(DOCINFO));
			di.cbSize = sizeof(DOCINFO);
			di.lpszDocName = strTitle;
			pDC->m_hDC = dlg.GetPrinterDC();

			// Start printing
			if (pDC->StartDoc(&di))
			{
				CImage imgLogo;
				imgLogo.Load(_T("Screenshot.bmp"));
				CFont* pFont = nullptr;
				unsigned long middleDocBody = 0UL;

				pDC->m_bPrinting = TRUE;
				pDC->StartPage();

				// Print border
				CRect rctBorder(0, 0, nHorRes, nVerRes);
				rctBorder.DeflateRect(nLogPixelsX / 2, nLogPixelsY / 2);
				pDC->Draw3dRect(rctBorder, RGB(0, 0, 0), RGB(0, 0, 0));

				/* Common print jobs */
				auto PrintLogo = [pDC, imgLogo](int nPosX, int nPosY)
					{
						imgLogo.StretchBlt(pDC->m_hDC, nPosX, nPosY, imgLogo.GetWidth() * 5, imgLogo.GetHeight() * 5, 0, 0, imgLogo.GetWidth(), imgLogo.GetHeight(), SRCCOPY);
					};

				auto PrintHeader = [pDC, HeaderTextLineDown, TotalTabInPixels, imgLogo](CFont* font, CString& strAssetDescription, CString& strWorkorderDescription,
					CString& strAssetModelNumber, CString& strAssetBrand, CString& strCustomerSurname, CString& strCustomerName,
					int nPosX, int nPosY) -> CRect
					{
						// Print Header rectangle
						CRect rctHeader(nPosX + 60, nPosY, nPosX + (imgLogo.GetWidth() * 5) - 60, nPosY + HeaderTextLineDown(2));
						pDC->FillRect(rctHeader, &CBrush(RGB(0, 102, 255)));
						pDC->Draw3dRect(rctHeader, RGB(0, 102, 255), RGB(0, 102, 255));

						// Print header text
						pDC->SelectObject(font);
						pDC->SetTextColor(RGB(255, 255, 255));
						pDC->DrawText(_T(" ") + strAssetDescription + _T(": ") + strWorkorderDescription + _T("\n") +
							_T("Workorder: 12345") + _T(" | Model: ") + strAssetModelNumber + _T(" | Brand: ") + strAssetBrand
							, rctHeader, DT_CENTER | DT_TABSTOP);

						return rctHeader;
					};

				auto PrintDetails = [pDC, HeaderTextLineDown, &fontPlain](CFont* font, CString& strCustomerSurname, CString& strCustomerName,
					unsigned long ulMiddle, int nPosX, int nPosY) -> int
					{
						int nLineDown = 0;
						// Set details values
						CString strTextLine1a = _T("Date: ");// +COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y"));
						CString strTextLine1b = _T("Customer: ");// +m_strCustomerSurname + _T(" ") + m_strCustomerName;
						CString strTextLine2a = _T("Employee: ");// +theApp.GetSelectedEmployeeName();
						CString strTextLine2b = _T("Mobile: ");// 06 - 3456789");
						CString strTextLine3 = _T("Phone: ");// 070 - 5647864");
						CString strTextLine4 = _T("Email: ");// zuurtje@zuiker.com");

						// Set font color
						pDC->SetBkColor(RGB(255, 255, 255));
						pDC->SetTextColor(RGB(0, 0, 0));

						// Print details text
						// Line 1 Date and Custemer
						pDC->TextOut(nPosX, nPosY, strTextLine1a);
						pDC->TextOut(nPosX + ulMiddle, nPosY, strTextLine1b);
						//Line 2 Employee and mobile
						pDC->TextOut(nPosX, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2a);
						pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 1), strTextLine2b);
						//Line 3 Phone
						pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 2), strTextLine3);
						//Line 4 Email
						pDC->TextOut(nPosX + ulMiddle, nPosY + HeaderTextLineDown(nLineDown + 3), strTextLine4);

						font = &fontPlain;
						pDC->SelectObject(font);

						CSize textSizeCustomer = pDC->GetTextExtent(strTextLine1b);
						CSize TextSizeEmployee = pDC->GetTextExtent(strTextLine2a);

						// Print detals value text
						pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY, COleDateTime::GetCurrentTime().Format(_T("%m/%d/%Y")));
						pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY, strCustomerSurname + _T(" ") + strCustomerName);
						pDC->TextOut(nPosX + TextSizeEmployee.cx, nPosY + HeaderTextLineDown(++nLineDown), theApp.GetSelectedEmployeeName());
						pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(nLineDown), _T("06 - 3456789"));
						pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), _T("070 - 5647864"));
						pDC->TextOut(nPosX + ulMiddle + textSizeCustomer.cx, nPosY + HeaderTextLineDown(++nLineDown), _T("zuurtje@zuiker.com"));
						
						return nLineDown;
					};

				/***** print left half (A5) on A4 - Customer receipt *****/

				// Set print start position
				int nPosX1 = rctBorder.TopLeft().x + 10;
				int nPosY1 = rctBorder.TopLeft().y + 10;

				// Print logo
				PrintLogo(nPosX1, nPosY1);

				// Calculate new start print position
				nPosY1 += imgLogo.GetHeight() * 5 + 10;

				// Print header
				pFont = &fontBold;
				auto rctHeaderLeft = PrintHeader(pFont, m_strAssetDescription, m_strWorkorderDescription, m_strAssetModelNumber, 
					m_strAssetBrand, m_strCustomerSurname, m_strCustomerName, nPosX1, nPosY1);

				middleDocBody = (((rctHeaderLeft.BottomRight().x - rctHeaderLeft.TopLeft().x) + (nPosX1 + 60)) / 2) - TotalTabInPixels(2);

				// Calculate new start print position
				nPosX1 += TotalTabInPixels(1);
				nPosY1 = rctHeaderLeft.BottomRight().y + HeaderTextLineDown(1);

				// Print customer workorder details
				auto nLinesDown = PrintDetails(pFont, m_strCustomerSurname, m_strCustomerName, middleDocBody, nPosX1, nPosY1);

				nPosY1 += HeaderTextLineDown(nLinesDown + 2);

				pFont = &fontItalic;
				pDC->SelectObject(pFont);

				// Print body text (receipt)
				pDC->TextOutW(nPosX1, nPosY1, _T("HUISREGELS"));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De activiteiten van NME Repair worden gratis en op vrijwillige basis uitgevoerd door de"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aanwezige reparatiedeskundigen."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Noch de organisatie van het NME Repair, noch de reparateurs zijn aansprakelijk voor schade"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("als gevolg van verstrekte reparatieadviezen of instructies, voor schade aan de ter reparatie"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("aangeboden voorwerpen, voor gevolgdchade of voor andere schade die het gevolg is van"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("activiteiten van het NME Repair."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Voor het gebruik van nieuwe materialen zoals; snoeren, stekkers, zekeringen, onderdelen, etc,"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("dient appart te worden betaald."));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("Bezoekers die defecte voorwerpen aanbieden ter reparatie, doen dat op eigen risico."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair behouden zich het recht voor om voorwerpen te weigeren en/of"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("niet te repareren."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De reparateurs van het NME Repair zijn niet verplicht om gedomonteerde voorwerpen die niet"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("gerepareerd kunnen worden, weer te monteren."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("De bezoekers van het MNE Repair zijn zelfs verantwoorderdelijk voor ht netjes afvoeren van"));
				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(1), _T("kapotte voorwerpen die niet konden worden gerepareerd."));

				pDC->TextOutW(nPosX1, nPosY1 += BodyTextLineDown(2), _T("Een vrijwilge bijdrage wordt op prijs gesteld."));

				/***** print right half (A5) on A4 - Workorder *****/

				// Set print start position
				nPosX1 = rctBorder.TopLeft().x + (rctBorder.Width() / 2) + 10;
				nPosY1 = rctBorder.TopLeft().y + 10;

				PrintLogo(nPosX1, nPosY1);

				// Calculate new start print position
				nPosY1 += imgLogo.GetHeight() * 5 + 10;

				// Print header
				pFont = &fontBold;
				auto rctHeaderRight = PrintHeader(pFont, m_strAssetDescription, m_strWorkorderDescription, m_strAssetModelNumber, m_strAssetBrand, m_strCustomerSurname, m_strCustomerName, nPosX1, nPosY1);
				
				// Calculate new start print position
				nPosX1 += TotalTabInPixels(1);
				nPosY1 = rctHeaderRight.BottomRight().y + HeaderTextLineDown(1);

				// Print customer workorder details
				nLinesDown = PrintDetails(pFont, m_strCustomerSurname, m_strCustomerName, middleDocBody, nPosX1, nPosY1);
				
				// Calculate new start print position
				nPosY1 += HeaderTextLineDown(nLinesDown +2);

				// Print body text (workorder)
				pDC->TextOut(nPosX1 + TotalTabInPixels(2), nPosY1, _T("Status: In Progress"));

				// calculate new start print position
				nPosY1 += HeaderTextLineDown(2);

				// print work comment area
				pDC->TextOut(nPosX1, nPosY1, _T("Werkzaamheden"));
				nPosY1 += HeaderTextLineDown(1);
				CRect rctWork(nPosX1, nPosY1, rctHeaderRight.right, nPosY1 + HeaderTextLineDown(8));
				pDC->Draw3dRect(rctWork, RGB(0, 102, 255), RGB(0, 102, 255));

				// calculate new start print position
				nPosY1 += HeaderTextLineDown(10);

				// Set font color
				pDC->SetBkColor(RGB(255, 255, 255));
				pDC->SetTextColor(RGB(0, 0, 0));

				// print general comment area
				pDC->TextOut(nPosX1, nPosY1, _T("Diversen"));
				nPosY1 += HeaderTextLineDown(1);
				CRect rctComment(nPosX1, nPosY1, rctHeaderRight.right, nPosY1 + HeaderTextLineDown(4));
				pDC->Draw3dRect(rctComment, RGB(0, 102, 255), RGB(0, 102, 255));

				// Destroy image
				imgLogo.Destroy();
			}
		}

		// Delete fonts
		fontPlain.DeleteObject();
		fontBold.DeleteObject();
		fontItalic.DeleteObject();

		// End printing
		pDC->EndPage();
		pDC->EndDoc();
		// Detach the printer handle
		pDC->Detach();
		// Delete the printer device context
		ReleaseDC(pDC);
		delete pDC;
		// Free the memory allocated for the DEVMODE structure
		GlobalFree(dlg.m_pd.hDevMode);
	}
}

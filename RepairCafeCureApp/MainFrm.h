/*
	Copyright (C) 2023/24  artvabas

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
* File: MainFrm.h is the header file for the main application class.
*
* This class is responsible for the main window of the application.
* It also contains the main menu, the ribbon bar and the status bar.
* The main menu is created in the resource file.
* The ribbon bar is created in the OnCreate method.
* The status bar is created in the OnCreate method.
* The caption bar is created in the OnCreate method.
*
* Target: Windows 10/11 64bit
* Version: 1.0.0.5 (Alpha)
* Created: 18-10-2023, (dd-mm-yyyy)
* Updated: 21-07-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

#pragma once

class CMainFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMFCRibbonStatusBar  m_wndStatusBar;
private:
	CComboBox* m_pCmbCaptionBarEmployeeName;
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCCaptionBar    m_wndCaptionBar;
	std::vector<std::pair<CString, bool>> m_vecEmployeeList;

	bool m_bHighlightCategories;
	bool m_bHighlightAll;

protected:
	CMainFrame() noexcept;
public:
	virtual ~CMainFrame();

	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif
private:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnOptions();
	afx_msg void OnGeneralAccessibilityHighlight();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();

	afx_msg void OnUpdateCustomerView(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkorderViewOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkorderViewProgress(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkorderViewRepaired(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSearchHistory(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportViewFinanceTax(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportWorkorderPinTransaction(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReportWorkorderClosed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateIsPrintable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkorderExtraCombi(CCmdUI* pCmdUI);
	afx_msg void OnUpdateWorkorderExtraInvoice(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGeneralShowLoginBarCheck(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAppAdmin(CCmdUI* pCmdUI);
public:
	afx_msg void OnCaptionBarComboBoxEmployeeNameChange();

private:
	BOOL CreateCaptionBar();
	void LoadSettings() noexcept;
	void SaveSettings(const int& nHighLight) const noexcept;
public:
	CString GetSelectedEmployee() const noexcept;
	inline void EmployeeIdle() const noexcept { m_pCmbCaptionBarEmployeeName->SetCurSel(0); }
	void GetEmployeeList() noexcept;
};
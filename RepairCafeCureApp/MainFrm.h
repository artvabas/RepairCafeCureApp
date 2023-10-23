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
* Version: 1.0.230.0
* Created: 11-10-2023, (dd-mm-yyyy)
* Updated: 23-10-2023, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* Description: Main application class for RepairCafeCureApp
* License: GPLv3
*/

#pragma once

class CMainFrame : public CFrameWndEx
{
	
protected: // create from serialization only
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)
public:
	virtual ~CMainFrame();

// Members
private:
	CComboBox* m_pCmbCaptionBarEmployeeName;
protected:
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CMFCCaptionBar    m_wndCaptionBar;

// Overrides
public:
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

// Message Handlers
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCaptionBar();
	afx_msg void OnUpdateViewCaptionBar(CCmdUI* pCmdUI);
	afx_msg void OnOptions();
	afx_msg void OnFilePrint();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
public:
	afx_msg void OnCaptionBarComboBoxEmployeeNameChange();
	DECLARE_MESSAGE_MAP()

// General methods
public:
	BOOL CreateCaptionBar();
};



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
	* File: CAssetView.h, defines class CAssetView
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

#pragma once
namespace artvabas::rcc::ui {

	class CAssetView : public CFormView
	{
		DECLARE_DYNCREATE(CAssetView)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_REPAIRCAFECUREAPP_FORM };
#endif
	public:
		CAssetView() noexcept;
		virtual ~CAssetView();

	private:
		BOOL PreCreateWindow(CREATESTRUCT& cs) override;
		void DoDataExchange(CDataExchange* pDX) override;
		void OnInitialUpdate() override;
		BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
		void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif
	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnFilePrintPreview() noexcept;
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point) noexcept;
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point) noexcept;

	public:
		CRepairCafeCureAppDoc* GetDocument() const;
	};

#ifndef _DEBUG  // debug version in CAssetView.cpp
	inline CRepairCafeCureAppDoc* CAssetView::GetDocument() const
	{
		return reinterpret_cast<CRepairCafeCureAppDoc*>(m_pDocument);
	}
#endif
}

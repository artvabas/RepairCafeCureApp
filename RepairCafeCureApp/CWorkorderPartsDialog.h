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
* File: CWorkorderPartsDialog.h, defines class CWorkorderPartsDialog
*
* This class is the dialog for adding parts to workorder
*
* Target: Windows 10/11 64bit
* Version: 0.0.1.0 (Alpha)
* Created: 15-11-2023, (dd-mm-yyyy)
* Updated: 29-04-2024, (dd-mm-yyyy)
* Creator: artvabasDev / artvabas
*
* License: GPLv3
*/
#pragma once
#include "afxdialogex.h"

namespace artvabas::rcc::ui::dialogs {

	class CWorkorderPartsDialog : public CDialogEx
	{
		DECLARE_DYNAMIC(CWorkorderPartsDialog)
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_WORKORDER_PARTS };
#endif

	private:
		bool m_bIsAddedPartListSelected;
		unsigned int m_unWorkorderID;

		CListCtrl m_lscWorkorderStockPartList;
		CListCtrl m_lscWorkorderAddedPartList;

		CString m_strWorkorderPartDescription;
		CString m_strWorkorderPartAmount;
		CString m_strWorkorderPartUnitPrice;
		CString m_strWorkorderPartTotalPrice;

		CButton m_btnWorkorderPartAdd;
		CButton m_btnWorkorderPartDelete;
		CButton m_btnWorkorderPartChange;

	public:
		CWorkorderPartsDialog(const unsigned int& nWorkorderID, CWnd* pParent = nullptr) noexcept;
		virtual ~CWorkorderPartsDialog();

	private:
		void DoDataExchange(CDataExchange* pDX) override;
		BOOL OnInitDialog() override;
		void OnOK() override;

	private:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnEnChangeWorkorderAddParts() noexcept;
		afx_msg void OnNMDoubleClickWorkorderStockPartsList(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnNMClickWorkorderAddedPartsList(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnBnClickedWorkorderAddPart() noexcept;
		afx_msg void OnBnClickedWorkorderDeleteAddedPart()  noexcept;
		afx_msg void OnLvnItemChangedWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnNMKillFocusWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult) noexcept;
		afx_msg void OnBnClickedWorkorderChange() noexcept;

	private:
		bool InitStockPartList();
		bool InitAddedPartList();
		void CalculateTotalPrice() noexcept;
		void SetChangeDeleteButtonState(BOOL bFlag = TRUE) noexcept;
		void ClearPartInputFields() noexcept;
	};
}
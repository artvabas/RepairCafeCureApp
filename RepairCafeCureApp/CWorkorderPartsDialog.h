#pragma once
#include "afxdialogex.h"


// CWorkorderPartsDialog dialog

class CWorkorderPartsDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkorderPartsDialog)
private:
	unsigned int m_unWorkorderID;
	CListCtrl m_lscWorkorderStockPartList;
	CListCtrl m_lscWorkorderAddedPartList;
	CString m_strWorkorderPartDescription;
	CString m_strWorkorderPartAmount;
	CString m_strWorkorderPartUnitPrice;
	CString m_strWorkorderPartTotalPrice;
	CButton m_btnWorkorderPartAdd;
	CButton m_btnWorkorderPartDelete;

public:
	CWorkorderPartsDialog(const unsigned int& nWorkorderID, CWnd* pParent = nullptr);
	virtual ~CWorkorderPartsDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORKORDER_PARTS };
#endif

	DECLARE_MESSAGE_MAP()

private:
	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	void OnOK() override;

private:
	afx_msg void OnEnChangeWorkorderAddParts();
	afx_msg void OnNMDoubleClickWorkorderStockPartsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMClickWorkorderAddedPartsList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedWorkorderAddPart();
	afx_msg void OnBnClickedWorkorderDeleteAddedPart();
	afx_msg void OnLvnItemChangedWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMKillFocusWorkorderAddedParts(NMHDR* pNMHDR, LRESULT* pResult);

private:
	bool InitStockPartList();
	bool InitAddedPartList();
	void CalculateTotalPrice();
};

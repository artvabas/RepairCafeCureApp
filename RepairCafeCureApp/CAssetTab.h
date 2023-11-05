#pragma once
#include "afxdialogex.h"


// CAssetTab dialog

class CAssetTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetTab)

public:
	CAssetTab(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAssetTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSET_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strCustomerSurname;
	CString m_strCustomerName;
	unsigned int m_nCustomerID;
public:
	virtual BOOL OnInitDialog();
private:
	bool m_bIsDirtyAssetDetails;
	bool m_bIsSelectedFromAssetList;
	CString m_strDescription;
	CString m_strModelNumber;
	CString m_strBrand;
	CString m_strHistoryLog;
	CButton m_btnUpdateAsset;
	CButton m_btnNewAsset;
	CButton m_btnCreateWorkorder;
	CListCtrl m_ctrExistingAssetList;
private:
	afx_msg void OnEnChangeAssetDetails();
	afx_msg void OnDoubleClickAssetTabAssetList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedAssetTabUpdate();
	afx_msg void OnBnClickedAssetTabNew();
};

#pragma once
#include "afxdialogex.h"


// CAssetTab dialog

class CAssetTab : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetTab)

public:
	CAssetTab(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAssetTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSET_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strCustomerSurename;
	CString m_strCustomerName;
	unsigned int m_nCustomerID;
};

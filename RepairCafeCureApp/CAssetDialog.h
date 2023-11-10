#pragma once
#include "afxdialogex.h"
#include "CTabCtrlAssetWorkorder.h"

// CAssetDialog dialog
using namespace artvabas::rcc::ui::controls;

class CAssetDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CAssetDialog)

public:
	CAssetDialog(CString strCustomerSurname, CString strCustomerName, unsigned int nCustomerID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAssetDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASSET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrlAssetWorkorder m_ctrTabAssetWorkorder;


};

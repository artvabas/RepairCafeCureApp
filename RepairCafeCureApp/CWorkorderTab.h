#pragma once
//#include "afxdialogex.h"
#include "CTabCtrlAssetWorkorder.h"

// CWorkorderTab dialog

class CWorkorderTab : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkorderTab)

private:
	CTabCtrlAssetWorkorder* m_pTabControl;
	AssetDetailsRecords* m_pAssetDetailsRecords;

public:
	CWorkorderTab(CTabCtrlAssetWorkorder* pTabControl,CWnd* pParent = nullptr);   // standard constructor
	virtual ~CWorkorderTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORKORDER_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#pragma once
#include "afxdialogex.h"


// CWorkorderTab dialog

class CWorkorderTab : public CDialogEx
{
	DECLARE_DYNAMIC(CWorkorderTab)

public:
	CWorkorderTab(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CWorkorderTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WORKORDER_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

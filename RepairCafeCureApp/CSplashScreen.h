#pragma once
#include "afxdialogex.h"

class CSplashScreen : public CDialogEx {
	DECLARE_DYNAMIC(CSplashScreen)

public:
	CSplashScreen(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSplashScreen();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SPLASHSCREEN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

// CSplashScreen.cpp : implementation file
//

#include "pch.h"
#include "RepairCafeCureApp.h"
#include "afxdialogex.h"
#include "CSplashScreen.h"

IMPLEMENT_DYNAMIC(CSplashScreen, CDialogEx)

CSplashScreen::CSplashScreen(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SPLASHSCREEN, pParent){}

CSplashScreen::~CSplashScreen(){}

void CSplashScreen::DoDataExchange(CDataExchange* pDX) {
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSplashScreen, CDialogEx)
END_MESSAGE_MAP()


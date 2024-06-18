#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"


using namespace artvabas::rcc::ui::controls;

class CClosedWorkorderAssetDetailsTab : public CDialogEx
{
	DECLARE_DYNAMIC(CClosedWorkorderAssetDetailsTab)

private:
	CClosedWorkorderDetailsTabCtrl* m_pTabControl;
	unsigned int m_unAssetID;

public:
	CClosedWorkorderAssetDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClosedWorkorderAssetDetailsTab();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSED_WORKORDER_ASSET_DETAILS_TAB };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

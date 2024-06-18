#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"


using namespace artvabas::rcc::ui::controls;

class CClosedWorkOrderDetailsTab : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSED_WORKORDER_DETAILS_TAB };
#endif
	DECLARE_DYNAMIC(CClosedWorkOrderDetailsTab)

private:
	CClosedWorkorderDetailsTabCtrl* m_pTabControl;
	unsigned int m_unWorkorderID;

public:
	CClosedWorkOrderDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClosedWorkOrderDetailsTab();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"

using namespace artvabas::rcc::ui::controls;

class CClosedWorkorderCustomerDetailsTab : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSED_WORKORDER_CUSTOMER_DETAILS_TAB };
#endif
	DECLARE_DYNAMIC(CClosedWorkorderCustomerDetailsTab)

private:
	CClosedWorkorderDetailsTabCtrl* m_pTabControl;
	unsigned int m_unCustomerID;

public:
	CClosedWorkorderCustomerDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClosedWorkorderCustomerDetailsTab();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

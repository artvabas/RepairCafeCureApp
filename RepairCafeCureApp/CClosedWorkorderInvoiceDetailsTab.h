#pragma once
#include "CClosedWorkorderDetailsTabCtrl.h"

using namespace artvabas::rcc::ui::controls;

class CClosedWorkorderInvoiceDetailsTab : public CDialogEx
{
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLOSED_WORKORDER_INVOICE_DETAILS_TAB };
#endif
	DECLARE_DYNAMIC(CClosedWorkorderInvoiceDetailsTab)

private:
	CClosedWorkorderDetailsTabCtrl* m_pTabControl;
	unsigned int m_unInvoiceID;

public:
	CClosedWorkorderInvoiceDetailsTab(CClosedWorkorderDetailsTabCtrl* pTabControl, unsigned int unID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CClosedWorkorderInvoiceDetailsTab();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};

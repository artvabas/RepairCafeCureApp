#pragma once
#include "afxdialogex.h"


// CContributionPaymentDialog dialog

class CContributionPaymentDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CContributionPaymentDialog)

private:
	CString m_strContributionPaymentInvoice;
	CString m_strContributionPaymentPaid;
	CString m_strContributionPaymentContribution;
	CString m_strContributionPaymentReturn;

public:
	CContributionPaymentDialog(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CContributionPaymentDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CONTRIBUTION };
#endif

private:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	afx_msg void OnEnChangeContributionPayment();
	afx_msg void OnBnClickedContributionPaymentClear();
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
private:
	BOOL m_blPinTransaction;
};

#pragma once
#include "afxdialogex.h"


// CContributionPaymentDialog dialog

class CContributionPaymentDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CContributionPaymentDialog)

	struct InvoiceData
	{
		unsigned int unInvoiceNumber;
		unsigned int unCustomerID;
		unsigned int unAssetID;
		unsigned int unWorkOrderID;
		bool bPaymentWithPin;
		CString InvoiceCreationDate;
		CString strSubTotal;
		CString strTotal;
	};

	/*
	* achtenaam, datum 
	gereed, home telefoon, mobiel 
	telefoon en natuurlijk het bedrag 
	*/
	struct ContributionData
	{
		unsigned int unContributionID;
		unsigned int unCustomerID;
		unsigned int unWorkOrderID;
		unsigned int unAssetID;
		CString strContribution;
	};
	

private:
	CString m_strContributionPaymentInvoice;
	CString m_strContributionPaymentPaid;
	CString m_strContributionPaymentContribution;
	CString m_strContributionPaymentReturn;
	InvoiceData* m_pInvoiceData;
	ContributionData* m_pContributionData;

public:
	CContributionPaymentDialog(const InvoiceData* pInvoiceData, const ContributionData* pContributionData, CWnd* pParent = nullptr);   // standard constructor
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

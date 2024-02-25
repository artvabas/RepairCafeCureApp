#pragma once
#include "afxdialogex.h"


namespace artvabas {

	namespace rcc {

		namespace ui {

			namespace dialogs {
				class CContributionPaymentDialog : public CDialogEx
				{
					DECLARE_DYNAMIC(CContributionPaymentDialog)

					struct InvoiceData
					{
						unsigned int unCustomerID{};
						unsigned int unAssetID{};
						unsigned int unWorkOrderID{};
						CString strTotal;
					};

					/*
					* achtenaam, datum
					gereed, home telefoon, mobiel
					telefoon en natuurlijk het bedrag
					*/
					struct ContributionData
					{
						double dContribution;
						double dPaid;
						bool bPaymentWithPin;
					};


				private:
					CString m_strContributionPaymentInvoice;
					CString m_strContributionPaymentPaid;
					CString m_strContributionPaymentContribution;
					CString m_strContributionPaymentReturn;
					CButton m_btnContributionPaymentOK;
					InvoiceData* m_stuInvoiceData;
					ContributionData* m_stuContributionData;
					BOOL m_blPinTransaction;

				public:
					CContributionPaymentDialog(InvoiceData& pInvoiceData, ContributionData& pContributionData, CWnd* pParent = nullptr);   // standard constructor
					virtual ~CContributionPaymentDialog();

				private:
					void Calculate(const double& dPaid, const double& dInvoice, const double& dContribution);
					inline void SetOKButtonState() {
						m_strContributionPaymentPaid.IsEmpty() ?
							m_btnContributionPaymentOK.EnableWindow(FALSE) : m_btnContributionPaymentOK.EnableWindow(TRUE);
					}
					// Dialog Data
#ifdef AFX_DESIGN_TIME
					enum { IDD = IDD_CONTRIBUTION };
#endif

				private:
					void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
					BOOL OnInitDialog() override;

					DECLARE_MESSAGE_MAP()
				private:
					afx_msg void OnEnChangeContributionPayment();
					afx_msg void OnBnClickedContributionPaymentCalculate();
					afx_msg void OnBnClickedContributionPaymentClear();
					afx_msg void OnBnClickedOk();
				};
			}
		}
	}
}


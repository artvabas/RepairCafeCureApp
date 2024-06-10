#pragma once

class CReportWorkorderClosedView : public CFormView
{
	DECLARE_DYNCREATE(CReportWorkorderClosedView)
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REPORT_WORKOREER_CLOSED_FORM };
#endif

private:
	PrinterOrientation m_ePrinterOrientation;
	CListCtrl m_lstWorkorderClosedReport;

public:
	CReportWorkorderClosedView();           // protected constructor used by dynamic creation
	virtual ~CReportWorkorderClosedView();

private:
	void DoDataExchange(CDataExchange* pDX) override;
	void OnInitialUpdate() override;
	BOOL PreTranslateMessage(MSG* pMsg) override;
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnPrint(CDC* pDC, CPrintInfo* pInfo) override;
#ifdef _DEBUG
	void AssertValid() const override;
#ifndef _WIN32_WCE
	void Dump(CDumpContext& dc) const override;
#endif
#endif
	
private:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnFilePrintPreview() noexcept;
	afx_msg void OnNMDoubleClickWorkorderClosedReport(NMHDR* pNMHDR, LRESULT* pResult) noexcept;

public:
	bool SetPrinterOrientation(HANDLE h, CDC* dc = NULL) const  noexcept;
};
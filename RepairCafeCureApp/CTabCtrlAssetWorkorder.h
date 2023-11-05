#pragma once


// CTabCtrlAssetWorkorder

class CTabCtrlAssetWorkorder : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlAssetWorkorder)

private:
	CDialog* m_tabPages[2];
	int m_tabCurrent;
	int m_nNumberOfTabs;

public:
	CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID);
	virtual ~CTabCtrlAssetWorkorder();

protected:
	
public:
	void Init();
private:
	void SetRectangle();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};



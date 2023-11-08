#pragma once
struct AssetDetailsRecords
{
	CString m_strCustomerSurname;
	CString m_strCustomerName;

	unsigned int m_nAssetID;
	unsigned int m_nAssetCustomerID;
	CString m_strDescription;
	CString m_strModelNumber;
	CString m_strBrand;
};

// CTabCtrlAssetWorkorder

class CTabCtrlAssetWorkorder : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlAssetWorkorder)

private:
	CDialog* m_tabPages[2];
	int m_tabCurrent;
	int m_nNumberOfTabs;
public:
	AssetDetailsRecords m_assetDetailsRecords;

public:
	CTabCtrlAssetWorkorder(CString& strCustomerSurname, CString& strCustomerName, unsigned int& nCustomerID);
	virtual ~CTabCtrlAssetWorkorder();

protected:
	
public:
	void Init();
	void ChangeTabView();
private:
	void SetRectangle();

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};



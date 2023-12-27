#pragma once
class CPrintWorkorder
{
public:
	
	struct WorkorderData
	{
		CString strWorkorderID;
		CString strCustomerSurname;
		CString strCustomerName;
		CString strCustomerCellPhone;
		CString strCustomerPhone;
		CString strCustomerEmail;
		CString strCustomerComments;
		CString strAssetDescription;
		CString strAssetModelNumber;
		CString strAssetBrand;
		CString strWorkorderCreatedDate;
		CString strEmployeeResponsible;
		CString strWorkorderStatus;
		CString strWorkorderDescription;
		CString strWordorderTotalPartsPrice;

		struct SparePart
		{
			CString strSparePartDescription;
			CString strSparePartPrice;
			CString strSparePartQuantity;
			CString strSparePartSubTotalPrice;
		}m_structSparePArts;

		std::vector<SparePart> m_vecSpareParts;

		struct WorkorderLog
		{
			CString strWorkorderLog;
			CString strWorkorderRepairedDate;
		} m_structWorkorderLog;
	};

private:
	WorkorderData* m_pStructWorkorderData;
public:
	CPrintWorkorder(WorkorderData* pworkData);
	virtual ~CPrintWorkorder();

	void PrintCombi(CDC* pDC) const;

};


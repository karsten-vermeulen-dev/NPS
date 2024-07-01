#pragma once
#include <afxdialogex.h>
#include <unordered_map>
#include <string>

class CustomerInfo : public CDialogEx
{

public:

	struct CustomerData
	{
		CString sidCode;
		CString email;
		CString latestKeyUsed;
		CString latestExpiryDate;
	};

	CustomerInfo(CWnd* parent = nullptr);
	virtual ~CustomerInfo() {}

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CUSTOMER_INFO };
#endif

private:

	virtual void DoDataExchange(CDataExchange* pDX);    

	bool newKeyGenerated;
	std::unordered_map<std::string, CustomerData> m_customerMatrix;
	
	CString m_email;
	CString m_newKey;
	CString m_sidCode;
	CString m_customer;
	CString m_latestKeyUsed;
	
	CTime m_newExpiryDate;
	CTime m_latestExpiryDate;
	CComboBox m_comboBoxCtrl;

	DECLARE_MESSAGE_MAP()
	DECLARE_DYNAMIC(CustomerInfo)
};